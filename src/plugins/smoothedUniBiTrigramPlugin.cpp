
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>            *
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

SmoothedUniBiTrigramPlugin::SmoothedUniBiTrigramPlugin( HistoryTracker &ht )
    : Plugin( ht,
              "SmoothedUniBiTrigramPlugin",
              "SmoothedUniBiTrigramPlugin, a linear interpolating unigram bigram trigram plugin",
              "SmoothedUniBiTrigramPlugin, long description." )
{
    //DEBUG
    //std::cerr << "Entering SmoothedUniBiTrigramPlugin::SmoothedUniBiTrigramPlugin()" << std::endl;

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
    //std::cerr << "Exiting SmoothedUniBiTrigramPlugin::SmoothedUniBiTrigramPlugin()" << std::endl;
}



SmoothedUniBiTrigramPlugin::~SmoothedUniBiTrigramPlugin()
{
    delete db;
}



Prediction SmoothedUniBiTrigramPlugin::predict() const
{
    //DEBUG
    //std::cerr << "Entering SmoothedUniBiTrigramPlugin::predict()" << std::endl;

    // combined result of uni/bi/tri-gram predictions
    Prediction prediction;
	
    // get w_2, w_1, and prefix from HistoryTracker object
    std::string word_prefix = strtolower(historyTracker.getPrefix());
    std::string word_1      = strtolower(historyTracker.getToken(1));
    std::string word_2      = strtolower(historyTracker.getToken(2));

    // DEBUG
    std::cout << "[SmoothedUniBiTrigramPlugin] Prefix: " << word_prefix << std::endl;
    std::cout << "[SmoothedUniBiTrigramPlugin] Word_1: " << word_1      << std::endl;
    std::cout << "[SmoothedUniBiTrigramPlugin] Word_2: " << word_2      << std::endl;

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
    //std::cout << "[SmoothedUniBiTrigramPlugin] c: " << result[0][0].c_str() << std::endl;
    c = atoi(result[0][0].c_str());

    // get table of possible prefix completitions
    Ngram prefix_gram;
    prefix_gram.push_back(word_prefix);
    NgramTable prefixCompletionTable = db->getNgramLikeTable(prefix_gram);


    // let's retrieve all the other counts I need
    for (int i = 0; (i < prefixCompletionTable.size() && i < max_partial_predictions_size); i++) {
        // get w
	word = prefixCompletionTable[i][0];

	//std::cout << "[SmoothedUniBiTriGramPlugin] word is: " << word << std::endl;
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

	std::cout << "[SmoothedUniBiTrigramPlugin] Word  : " << word << std::endl
		  << "[SmoothedUniBiTrigramPlugin] Prefix: " << word_prefix << std::endl
		  << "[SmoothedUniBiTrigramPlugin] Word_1: " << word_1 << std::endl
		  << "[SmoothedUniBiTrigramPlugin] Word_2: " << word_2 << std::endl		
		  << "[SmoothedUniBiTrigramPlugin] c( " << word_2 << ", " << word_1 << ", " << word << " ) = " << c_w2_w1_w << std::endl
		  << "[SmoothedUniBiTrigramPlugin] c( " << word_2 << ", " << word_1 << " ) = " << c_w2_w1 << std::endl
		  << "[SmoothedUniBiTrigramPlugin] c( " << word_1 << ", " << word << " ) = " << c_w1_w << std::endl
		  << "[SmoothedUniBiTrigramPlugin] c( " << word_1 << " ) = " << c_w1 << std::endl
		  << "[SmoothedUniBiTrigramPlugin] c( " << word << " ) = " << c_w << std::endl
		  << "[SmoothedUniBiTrigramPlugin] c = " << c << std::endl
		  << "[SmoothedUniBiTrigramPlugin] f( " << word << " | " << word_1 << ", " << word_2 << " ) = " << f_w2_w1_w << std::endl
		  << "[SmoothedUniBiTrigramPlugin] f( " << word << " | " << word_1 << " ) = " << f_w1_w << std::endl
		  << "[SmoothedUniBiTrigramPlugin] f( " << word << " ) = " << f_w << std::endl;
	std::cout << "[SmoothedUniBiTrigramPlugin] Smoothed probability = " << probability << std::endl;
	
	
        // add computed suggestion to prediction
        prediction.addSuggestion( Suggestion( word, probability ) );
    }

    std::cout << "[SmoothedUniBiTrigramPlugin] " << std::endl 
	      << "[SmoothedUniBiTrigramPlugin] Prediction:" << std::endl
	      << "[SmoothedUniBiTrigramPlugin] ===========" << std::endl
	      << prediction << std::endl;
	
    return prediction; // Return combined prediction
}


void SmoothedUniBiTrigramPlugin::learn()
{
    std::cout << "SmoothedUniBiTrigramPlugin::learn() method called"
              << std::endl;
    std::cout << "SmoothedUniBiTrigramPlugin::learn() method exited"
              << std::endl;
}



void SmoothedUniBiTrigramPlugin::extract()
{
    std::cout << "SmoothedUniBiTrigramPlugin::extract() method called"
              << std::endl;
    std::cout << "SmoothedUniBiTrigramPlugin::extract() method exited"
              << std::endl;
}



void SmoothedUniBiTrigramPlugin::train()
{
    std::cout << "SmoothedUniBiTrigramPlugin::train() method called"
              << std::endl;
    std::cout << "SmoothedUniBiTrigramPlugin::train() method exited"
              << std::endl;
}



extern "C" Plugin* create( HistoryTracker& ht )
{
    return new SmoothedUniBiTrigramPlugin( ht );
}

extern "C" void destroy( Plugin *p )
{
    delete p;
}


