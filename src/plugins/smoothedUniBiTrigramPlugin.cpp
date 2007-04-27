
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>            *
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

SmoothedUniBiTrigramPlugin::SmoothedUniBiTrigramPlugin( HistoryTracker &ht )
    : Plugin( ht,
              "SmoothedUniBiTrigramPlugin",
              "SmoothedUniBiTrigramPlugin, a linear interpolating unigram bigram trigram plugin",
              "SmoothedUniBiTrigramPlugin, long description." )
{
    //DEBUG
    LOG("[SmoothedUniBiTriGram] Entering SmoothedUniBiTrigramPlugin::SmoothedUniBiTrigramPlugin()");

    // build options and default values
    options.push_back( Option( ".4",
                               DOUBLE,
                               "UNIGRAM_WEIGHT",
                               "Weight given to unigram frequency" )
    );
			   
    options.push_back( Option( ".3",
                               DOUBLE,
                               "BIGRAM_WEIGHT",
                               "Weight given to bigram frequency" )
    );
    options.push_back( Option( ".3",
                               DOUBLE,
                               "TRIGRAM_WEIGHT",
                               "Weight given to trigram frequency" )
    );
    database = static_cast<std::string>(DEFAULT_DATABASE_LOCATION) + "/" + DEFAULT_DATABASE_FILENAME;
    options.push_back( Option( database,
                               STRING,
                               "DBFILENAME",
                               "Database filename" )
    );
    options.push_back( Option( "40",
                               INT,
                               "MAX_PARTIAL_PREDICTION_SIZE",
                               "Maximum number of suggestions a partial prediction can contain."
                       )
    );

	
    // open database connector
    db = new SqliteDatabaseConnector(getOptionValue("DBFILENAME"));
    
    //DEBUG
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
    double alpha = atof( getOptionValue( "TRIGRAM_WEIGHT" ).c_str() );
    double beta = atof( getOptionValue( "BIGRAM_WEIGHT" ).c_str() );
    double gamma = atof( getOptionValue( "UNIGRAM_WEIGHT" ).c_str() );

    int max_partial_predictions_size = atoi( getOptionValue( "MAX_PARTIAL_PREDICTION_SIZE" ).c_str() );
	
    // To estimate  P( w | w_2, w_1 ) we need the following counts
    int c_w2_w1_w = 0;  // c( w_2, w_1, w )  
    int c_w2_w1 = 0;    // c( w_2, w_1 )     
    int c_w1_w = 0;     // c( w_1, w )	     
    int c_w1 = 0;       // c( w_1 )	     
    int c_w = 0;        // c( w )	     
    int c = 0;          // c                 

    std::string word;   // w

    // get c
    std::string query = "SELECT SUM( count ) FROM _1_gram;";
    NgramTable result = db->executeSql(query);
    LOG("[SmoothedUniBiTrigramPlugin] c: " << result[0][0].c_str());
    c = atoi(result[0][0].c_str());

    // get table of possible prefix completitions
    Ngram prefix_gram;
    prefix_gram.push_back(word_prefix);
    NgramTable prefixCompletionTable = db->getNgramLikeTable(prefix_gram);


    // let's retrieve all the other counts I need
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


        assert( c_w2_w1_w <= c_w2_w1 );
        assert( c_w1_w <= c_w1 );
        assert( c_w <= c );
		
        if( c_w2_w1 > 0 )
            assert( ( c_w2_w1_w / c_w2_w1 ) <= 1 );
        if( c_w1 > 0 )
            assert( ( c_w1_w / c_w1 ) <= 1 );
        if( c > 0 )
            assert( ( c_w / c ) <= 1 );

        if( c_w2_w1 > 0 && c_w2_w1 > 0 )		
            assert( log( c_w2_w1_w / c_w2_w1 ) <= 0 );
        if( c_w1_w > 0 && c_w1 > 0 )
            assert( log( c_w1_w / c_w1 ) <= 0 );
        if( c_w > 0 && c > 0 )
            assert( log( c_w / c ) <= 0 );


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
	
	
        // add computed suggestion to prediction
        prediction.addSuggestion( Suggestion( word, probability ) );
    }

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



extern "C" Plugin* create( HistoryTracker& ht )
{
    return new SmoothedUniBiTrigramPlugin( ht );
}

extern "C" void destroy( Plugin *p )
{
    delete p;
}


