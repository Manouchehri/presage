
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>           *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.*
 *                                                                           *
\*****************************************************************************/


#include "plugins/smoothedUniBiTrigramPlugin.h"

#ifdef DEBUG
# define LOG(x) std::cout << x << std::endl
#else
# define LOG(x) /* x */
#endif

SmoothedUniBiTrigramPlugin::SmoothedUniBiTrigramPlugin(HistoryTracker &ht, Profile* profile)
    : Plugin(ht,
	     profile,
             "SmoothedUniBiTrigramPlugin",
             "SmoothedUniBiTrigramPlugin, a linear interpolating unigram bigram trigram plugin",
             "SmoothedUniBiTrigramPlugin, long description." )
{
    LOG("[SmoothedUniBiTriGramPlugin] Entering SmoothedUniBiTrigramPlugin::SmoothedUniBiTrigramPlugin()");

    Variable variable;
    variable.push_back("Soothsayer");
    variable.push_back("Plugins");
    variable.push_back("SmoothedUniBiTrigramPlugin");

    Value value;

    try {
	variable.push_back("UNIGRAM_WEIGHT");
	value = profile->getConfig(variable);
	LOG("[SmoothedUniBiTriGramPlugin] UNIGRAM_WEIGHT: " + value);
	UNIGRAM_WEIGHT = toDouble(value);
	variable.pop_back();

	variable.push_back("BIGRAM_WEIGHT");
	value = profile->getConfig(variable);
	LOG("[SmoothedUniBiTriGramPlugin] BIGRAM_WEIGHT: " + value);
	BIGRAM_WEIGHT = toDouble(value);
	variable.pop_back();

	variable.push_back("TRIGRAM_WEIGHT");
	value = profile->getConfig(variable);
	LOG("[SmoothedUniBiTriGramPlugin] TRIGRAM_WEIGHT: " + value);
	TRIGRAM_WEIGHT = toDouble(value);
	variable.pop_back();

	variable.push_back("MAX_PARTIAL_PREDICTION_SIZE");
	value = profile->getConfig(variable);
	LOG("[SmoothedUniBiTriGramPlugin] MAX_PARTIAL_PREDICTION_SIZE: " + value);
	MAX_PARTIAL_PREDICTION_SIZE = toInt(value);
	variable.pop_back();

	variable.push_back("DBFILENAME");
	value = profile->getConfig(variable);
	LOG("[SmoothedUniBiTriGramPlugin] DBFILENAME: " + value);
	DBFILENAME = value;
	variable.pop_back();
    } catch (Profile::ProfileException ex) {
	std::cerr << "[SmoothedUniBiTrigramPlugin] Caught ProfileException: " << ex.what() << std::endl;
    }

    // open database connector
    db = new SqliteDatabaseConnector(DBFILENAME);
    
    LOG("[SmoothedUniBiTriGramPlugin] Exiting SmoothedUniBiTrigramPlugin::SmoothedUniBiTrigramPlugin()");
}



SmoothedUniBiTrigramPlugin::~SmoothedUniBiTrigramPlugin()
{
    delete db;
}



Prediction SmoothedUniBiTrigramPlugin::predict() const
{
    //DEBUG
    LOG("[SmoothedUniBiTriGramPlugin] Entering SmoothedUniBiTrigramPlugin::predict()");

    // combined result of uni/bi/tri-gram predictions
    Prediction prediction;
	
    // get w_2, w_1, and prefix from HistoryTracker object
    std::string word_prefix = strtolower(historyTracker.getPrefix());
    std::string word_1      = strtolower(historyTracker.getToken(1));
    std::string word_2      = strtolower(historyTracker.getToken(2));

    // DEBUG
    LOG("[SmoothedUniBiTrigramPlugin] Prefix: " << word_prefix);
    LOG("[SmoothedUniBiTrigramPlugin] Word_1: " << word_1     );
    LOG("[SmoothedUniBiTrigramPlugin] Word_2: " << word_2     );

    // get smoothing parameters
    double alpha = TRIGRAM_WEIGHT;
    double beta = BIGRAM_WEIGHT;
    double gamma = UNIGRAM_WEIGHT;

    int max_partial_predictions_size = MAX_PARTIAL_PREDICTION_SIZE;
	
    // To estimate  P( w | w_2, w_1 ) we need the following counts
    int c_w2_w1_w = 0;  // c( w_2, w_1, w )  
    int c_w2_w1 = 0;    // c( w_2, w_1 )     
    int c_w1_w = 0;     // c( w_1, w )	     
    int c_w1 = 0;       // c( w_1 )	     
    int c_w = 0;        // c( w )	     
    int c = 0;          // c                 

    std::string word;   // w

    // get c
    c = db->getUnigramCountsSum();
    LOG("[SmoothedUniBiTrigramPlugin] c: " << c);

    // get table of possible prefix completitions
    Ngram prefix_gram;
    prefix_gram.push_back(word_prefix);
    db->beginTransaction();
    NgramTable prefixCompletionTable = db->getNgramLikeTable(prefix_gram);
    db->endTransaction();

    // let's retrieve all the other counts I need
    db->beginTransaction();
    for (int i = 0; (i < prefixCompletionTable.size() && i < max_partial_predictions_size); i++) {
        // get w
	word = prefixCompletionTable[i][0];

	LOG("[SmoothedUniBiTriGramPlugin] word is: " << word);
	//std::cin.get();

	// get c_w
	c_w = atoi(prefixCompletionTable[i][1].c_str());

        // get c( w_1 )
	{
	    Ngram ngram;
	    ngram.push_back(word_1);
	    c_w1 = db->getNgramCount(ngram);
	}

        // get c( w_1, w )
	{
	    Ngram ngram;
	    ngram.push_back(word_1);
	    ngram.push_back(word);
	    c_w1_w = db->getNgramCount(ngram);
	}

        // get c( w_2, w_1 )
	{
	    Ngram ngram;
	    ngram.push_back(word_2);
	    ngram.push_back(word_1);
	    c_w2_w1 = db->getNgramCount(ngram);
	}

        // get c( w_2, w_1, w )
	{
	    Ngram ngram;
	    ngram.push_back(word_2);
	    ngram.push_back(word_1);
	    ngram.push_back(word);
	    c_w2_w1_w = db->getNgramCount(ngram);
	}

        // compute smoothed probability
        double f_w2_w1_w = ( c_w2_w1 > 0 ? ( static_cast< double >( c_w2_w1_w ) / c_w2_w1 ) : 0 );
        double f_w1_w = ( c_w1 > 0 ? ( static_cast< double >( c_w1_w ) / c_w1 ) : 0 );
        double f_w = ( c > 0 ? ( static_cast< double >( c_w ) / c ) : 0 );

        double probability = 
            alpha * f_w2_w1_w +
            beta * f_w1_w +
            gamma * f_w;


        // compute smoothed log probability
        //		double f_w2_w1_w = 0;
        //		double f_w1_w = 0;
        //		double f_w = 0;
        //
        //		if( c_w2_w1 > 0 && c_w2_w1_w > 0 )
        //			f_w2_w1_w = -1 * log( static_cast<double>(c_w2_w1_w) / c_w2_w1 );
        //		if( c_w1_w > 0 && c_w1 )
        //			f_w1_w = -1 * log( static_cast<double>(c_w1_w) / c_w1 );
        //		if( c_w > 0 && c > 0 )
        //			f_w = -1 * log(static_cast<double>(c_w) / c );
        //
        //		double probability = 
        //			alpha * f_w2_w1_w +
        //			beta * f_w1_w +
        //			gamma * f_w;

	LOG("[SmoothedUniBiTrigramPlugin] Word  : " << word                                                        );
	LOG("[SmoothedUniBiTrigramPlugin] Prefix: " << word_prefix                                                 );
	LOG("[SmoothedUniBiTrigramPlugin] Word_1: " << word_1                                                      );
	LOG("[SmoothedUniBiTrigramPlugin] Word_2: " << word_2                                                      );
	LOG("[SmoothedUniBiTrigramPlugin] c( " << word_2 << ", " << word_1 << ", " << word << " ) = " << c_w2_w1_w );
	LOG("[SmoothedUniBiTrigramPlugin] c( " << word_2 << ", " << word_1 << " ) = " << c_w2_w1                   );
	LOG("[SmoothedUniBiTrigramPlugin] c( " << word_1 << ", " << word << " ) = " << c_w1_w                      );
	LOG("[SmoothedUniBiTrigramPlugin] c( " << word_1 << " ) = " << c_w1                                        );
	LOG("[SmoothedUniBiTrigramPlugin] c( " << word << " ) = " << c_w                                           );
	LOG("[SmoothedUniBiTrigramPlugin] c = " << c                                                               );
	LOG("[SmoothedUniBiTrigramPlugin] f( " << word << " | " << word_1 << ", " << word_2 << " ) = " << f_w2_w1_w);
	LOG("[SmoothedUniBiTrigramPlugin] f( " << word << " | " << word_1 << " ) = " << f_w1_w                     );
	LOG("[SmoothedUniBiTrigramPlugin] f( " << word << " ) = " << f_w                                           );
	LOG("[SmoothedUniBiTrigramPlugin] Smoothed probability = " << probability                                  );

	
        assert( c_w2_w1_w <= c_w2_w1 );
        assert( c_w1_w <= c_w1 );
        assert( c_w <= c );
		
        if( c_w2_w1 > 0 )
            assert( ( c_w2_w1_w / c_w2_w1 ) <= 1 );
        if( c_w1 > 0 )
            assert( ( c_w1_w / c_w1 ) <= 1 );
        if( c > 0 )
            assert( ( c_w / c ) <= 1 );

	//        if( c_w2_w1 > 0 && c_w2_w1 > 0 )		
	//            assert( log( c_w2_w1_w / c_w2_w1 ) <= 0 );
	//        if( c_w1_w > 0 && c_w1 > 0 )
	//            assert( log( c_w1_w / c_w1 ) <= 0 );
	//        if( c_w > 0 && c > 0 )
	//            assert( log( c_w / c ) <= 0 );

	
        // add computed suggestion to prediction
        prediction.addSuggestion( Suggestion( word, probability ) );
    }
    db->endTransaction();

    LOG("[SmoothedUniBiTrigramPlugin] "           );
    LOG("[SmoothedUniBiTrigramPlugin] Prediction:");
    LOG("[SmoothedUniBiTrigramPlugin] ===========");
    LOG(prediction                                );
	
    return prediction; // Return combined prediction
}


void SmoothedUniBiTrigramPlugin::learn()
{
    LOG("[SmoothedUniBiTrigramPlugin] learn() method called");
    LOG("[SmoothedUniBiTrigramPlugin::learn() method exited");
}



void SmoothedUniBiTrigramPlugin::extract()
{
    LOG("SmoothedUniBiTrigramPlugin::extract() method called");
    LOG("SmoothedUniBiTrigramPlugin::extract() method exited");
}



void SmoothedUniBiTrigramPlugin::train()
{
    LOG("SmoothedUniBiTrigramPlugin::train() method called");
    LOG("SmoothedUniBiTrigramPlugin::train() method exited");
}



extern "C" SmoothedUniBiTrigramPlugin* create(HistoryTracker& ht, Profile* profile)
{
    return new SmoothedUniBiTrigramPlugin(ht, profile);
}

extern "C" void destroy(SmoothedUniBiTrigramPlugin* plugin)
{
    delete plugin;
}


