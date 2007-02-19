
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

	
    // open sqlite library
//    libsqlite = lt_dlopen( "libsqlite.so" );
//    if( libsqlite == NULL )
//        std::cerr << lt_dlerror() << std::endl;
//    assert( libsqlite != NULL );
//
//    // 
//    sqlite_open_handle = (sqlite_open_t*)lt_dlsym( libsqlite, "sqlite_open" );
//    sqlite_exec_handle = (sqlite_exec_t*)lt_dlsym( libsqlite, "sqlite_exec" );
//    sqlite_close_handle = (sqlite_close_t*)lt_dlsym( libsqlite, "sqlite_close" );
//
//    assert( sqlite_open_handle != NULL );
//    assert( sqlite_exec_handle != NULL );
//    assert( sqlite_close_handle != NULL );


    // open database
//    db = (*sqlite_open_handle)( getOptionValue( "DBFILENAME" ).c_str(), 0777, NULL );
    db = sqlite_open( getOptionValue( "DBFILENAME" ).c_str(), 0777, NULL );
    assert( db != NULL );
    
    //DEBUG
    //std::cerr << "Exiting SmoothedUniBiTrigramPlugin::SmoothedUniBiTrigramPlugin()" << std::endl;
}



SmoothedUniBiTrigramPlugin::~SmoothedUniBiTrigramPlugin()
{
    // close database cleanly
//    (*sqlite_close_handle)( db );
    sqlite_close( db );

    // close sqlite library
//    lt_dlclose( libsqlite );
}



Prediction SmoothedUniBiTrigramPlugin::predict() const
{
    //DEBUG
    //std::cerr << "Entering SmoothedUniBiTrigramPlugin::predict()" << std::endl;

    // get w_2, w_1, and prefix from HistoryTracker object
    std::string word_prefix = strtolower( historyTracker.getPrefix() );
    //std::cout << "Prefix: " << word_prefix << std::endl;
    std::string word_1 = strtolower( historyTracker.getToken(1) );
    //std::cout << "Word_1: " << word_1 << std::endl;
    std::string word_2 = strtolower( historyTracker.getToken(2) );
    //std::cout << "Word_2: " << word_2 << std::endl;

    std::string query; // string used to build sql query
    int result;        // database interrogation diagnostic

    Prediction p;     // combined result of uni/bi/tri gram predictions
	
    // get smoothing parameters
    double alpha = atof( getOptionValue( "TRIGRAM_WEIGHT" ).c_str() );
    double beta = atof( getOptionValue( "BIGRAM_WEIGHT" ).c_str() );
    double gamma = atof( getOptionValue( "UNIGRAM_WEIGHT" ).c_str() );

	
    // To estimate  P( w | w_2, w_1 ) we need the following:
    int c_w2_w1_w = 0;  // c( w_2, w_1, w )  
    int c_w2_w1 = 0;    // c( w_2, w_1 )     
    int c_w1_w = 0;     // c( w_1, w )	     
    int c_w1 = 0;       // c( w_1 )	     
    int c_w = 0;        // c( w )	     
    int c = 0;          // c                 

    std::string w;      // w

    std::vector< WordCount > wordCount;

    // get c
    query = "SELECT SUM( count ) "
        "FROM unigram;";
//    result = (*sqlite_exec_handle)( db,
    result = sqlite_exec( db,
			  query.c_str(),
			  getCount,
			  &c,
			  NULL );
    assert( result == SQLITE_OK );
	
    // get most likely unigrams whose w contains prefix
    query =	"SELECT word, count "
        "FROM unigram "
        "WHERE word LIKE \"" + word_prefix + "%\" "
        "ORDER BY count DESC;";
//    result = (*sqlite_exec_handle)( db,
    result = sqlite_exec( db,
			  query.c_str(),
			  getWordCount,
			  &wordCount,
			  NULL );
    assert( result == SQLITE_OK || result == SQLITE_ABORT );
    // now I've got w and c( w )

    // let's retrieve all the other counts I need
    for( unsigned int i = 0; i < wordCount.size(); i++ ) {
		
        // get w
        w = wordCount[ i ].word;

        // get c( w )
        if( !isnan(c_w) )
            c_w = wordCount[ i ].count;
		
        // get c( w_1 )
        query = "SELECT count "
            "FROM unigram "
            "WHERE word = \"" + word_1 + "\" "
            "ORDER BY count DESC;";
//        result = (*sqlite_exec_handle)( db,
        result = sqlite_exec( db,
                                        query.c_str(),
                                        getCount,
                                        &c_w1,
                                        NULL );
        assert( result == SQLITE_OK );
        if( isnan(c_w1) )
            c_w1 = 0;

        // get c( w_1, w )
        query = "SELECT count "
            "FROM bigram "
            "WHERE word_1 = \"" + word_1 + "\" "
            "AND word = \"" + w + "\" "
            "ORDER BY count DESC;";
//        result = (*sqlite_exec_handle)( db,
        result = sqlite_exec( db,
                                        query.c_str(),
                                        getCount,
                                        &c_w1_w,
                                        NULL );
        assert( result == SQLITE_OK );
        if( isnan(c_w1_w) )
            c_w1_w = 0;

        // get c( w_2, w_1 )
        query = "SELECT count "
            "FROM bigram "
            "WHERE word_1 = \"" + word_2 + "\" "
            "AND word = \"" + word_1 + "\" "
            "ORDER BY count DESC;";
//        result = (*sqlite_exec_handle)( db,
        result = sqlite_exec( db,
                                        query.c_str(),
                                        getCount,
                                        &c_w2_w1,
                                        NULL );
        assert( result == SQLITE_OK );
        if( isnan(c_w2_w1) )
            c_w2_w1 = 0;

        // get c( w_2, w_1, w )
        query = "SELECT count "
            "FROM trigram "
            "WHERE word_2 = \"" + word_2 + "\" "
            "AND word_1 = \"" + word_1 + "\" "
            "AND word = \"" + w + "\" "
            "ORDER BY count DESC;";
//        result = (*sqlite_exec_handle)( db,
        result = sqlite_exec( db,
                                        query.c_str(),
                                        getCount,
                                        &c_w2_w1_w,
                                        NULL );
        assert( result == SQLITE_OK );
        if( isnan(c_w2_w1_w) )
            c_w2_w1_w = 0;

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

        //		std::cout << "Word  : " << w << std::endl
        //			  << "Prefix: " << word_prefix << std::endl
        //			  << "Word_1: " << word_1 << std::endl
        //			  << "Word_2: " << word_2 << std::endl		
        //			  << "c( " << word_2 << ", " << word_1 << ", " << w << " ) = " << c_w2_w1_w << std::endl
        //			  << "c( " << word_2 << ", " << word_1 << " ) = " << c_w2_w1 << std::endl
        //			  << "c( " << word_1 << ", " << w << " ) = " << c_w1_w << std::endl
        //			  << "c( " << word_1 << " ) = " << c_w1 << std::endl
        //			  << "c( " << w << " ) = " << c_w << std::endl
        //			  << "c = " << c << std::endl
        //			  << "f( " << w << " | " << word_1 << ", " << word_2 << " ) = " << f_w2_w1_w << std::endl
        //			  << "f( " << w << " | " << word_1 << " ) = " << f_w1_w << std::endl
        //			  << "f( " << w << " ) = " << f_w << std::endl;
        //		std::cout << "Smoothed probability = " << probability << std::endl;

		
		
        // add computed suggestion to prediction
        p.addSuggestion( Suggestion( w, probability ) );
    }

    //	std::cout << std::endl 
    //		  << "Prediction:" << std::endl
    //		  << "===========" << std::endl
    //		  << p << std::endl;
	
    return p; // Return combined prediction
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


/** SQLite callback function
    Builds prediction from query results.

*/
int getWordCount( void* wcPtr,
		  int argc,
		  char** argv,
		  char** column )
{
    // make sure this was invoked by the right query
    assert( argc = 2 );
    assert( strcmp( column[0], "word" ) == 0 );
    assert( strcmp( column[1], "count" ) == 0 );

    // cast pointer to void back to pointer to trigrams object
    std::vector< WordCount >* wordCountPtr = static_cast< std::vector< WordCount >* >( wcPtr );
	
    WordCount wc;
    wc.word = argv[ 0 ];
    wc.count = atoi( argv[ 1 ] );
    wordCountPtr->push_back( wc );

    if( wordCountPtr->size() < 100 )
        return 0;
    else
        return 1;
}


int getCount( void* cPtr,
	      int argc,
	      char** argv,
	      char** column )
{
    // make sure this was invoked by the right query
    assert( argc == 1 );
    //	assert( strcmp( column[ 0 ], "count" ) );
	
    // cast pointer to void back to pointer to integer
    int* countPtr = static_cast< int* >( cPtr );
	
    *countPtr = atoi( argv[ 0 ] );

    return 0;
}




extern "C" Plugin* create( HistoryTracker& ht )
{
    return new SmoothedUniBiTrigramPlugin( ht );
}

extern "C" void destroy( Plugin *p )
{
    delete p;
}


