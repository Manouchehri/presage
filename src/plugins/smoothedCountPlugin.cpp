
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


#include "plugins/smoothedCountPlugin.h"


SmoothedCountPlugin::SmoothedCountPlugin( HistoryTracker &ht )
	: Plugin( ht,
		  "SmoothedCountPlugin",
		  "SmoothedCountPlugin, a linear interpolating unigram bigram trigram plugin",
		  "SmoothedCountPlugin, long description." )
{

	// build options and default values
	options.push_back( Option( ".05",
				   DOUBLE,
				   "UNIGRAM_WEIGHT",
				   "Weight given to unigram frequency" )
		);
			   
	options.push_back( Option( ".1",
				   DOUBLE,
				   "BIGRAM_WEIGHT",
				   "Weight given to bigram frequency" )
		);
	options.push_back( Option( ".5",
				   DOUBLE,
				   "TRIGRAM_WEIGHT",
				   "Weight given to trigram frequency" )
		);
	options.push_back( Option( "/home/matt/word_predictor/corpus/database_en.db",
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
//	libsqlite = lt_dlopen( "libsqlite.so" );
//	if( libsqlite == NULL )
//		std::cerr << lt_dlerror() << std::endl;
//	assert( libsqlite != NULL );
//
//	// 
//	sqlite_open_handle = (sqlite_open_t*)lt_dlsym( libsqlite, "sqlite_open" );
//	sqlite_exec_handle = (sqlite_exec_t*)lt_dlsym( libsqlite, "sqlite_exec" );
//	sqlite_close_handle = (sqlite_close_t*)lt_dlsym( libsqlite, "sqlite_close" );
//
//	assert( sqlite_open_handle != NULL );
//	assert( sqlite_exec_handle != NULL );
//	assert( sqlite_close_handle != NULL );


	// open database
        //	db = (*sqlite_open_handle)( getOptionValue( "DBFILENAME" ).c_str(), 0777, NULL );
	db = sqlite_open( getOptionValue( "DBFILENAME" ).c_str(), 0777, NULL );
	assert( db != NULL );

}



SmoothedCountPlugin::~SmoothedCountPlugin()
{
	// close database cleanly
//	(*sqlite_close_handle)( db );
	sqlite_close( db );

	// close sqlite library
//	lt_dlclose( libsqlite );
}



Prediction SmoothedCountPlugin::predict() const
{

	// get w_2, w_1, and prefix from HistoryTracker object
	std::string prefix = strtolower( historyTracker.getPrefix() );
	std::string word_1 = strtolower( historyTracker.getToken(1) );
	std::string word_2 = strtolower( historyTracker.getToken(2) );

	std::string query; // string used to build sql query
	int result;        // database interrogation diagnostic
	CallbackData data; // data to pass through to callback function
	

	// get most likely unigrams whose w contains prefix
	Prediction predUnigrams;
	
	data.predPtr = &predUnigrams;
	data.predSize = atoi(
		getOptionValue( "MAX_PARTIAL_PREDICTION_SIZE" ).c_str()
		);

	query =	"SELECT word, count "
		"FROM unigram "
		"WHERE word LIKE \"" + prefix + "%\" "
		"ORDER BY count DESC;";
	
        //	result = (*sqlite_exec_handle)( db,
	result = sqlite_exec( db,
					query.c_str(),
					buildPrediction,
					&data,
					NULL );

	assert( result == SQLITE_OK || result == SQLITE_ABORT );


	// get most likely bigrams having matching w_1 whose w contains prefix
	Prediction predBigrams;

	data.predPtr = &predBigrams;
//	data.predSize = atoi(
//		getOptionValue( "MAX_PARTIAL_PREDICTION_SIZE" ).c_str()
//		);

	query = "SELECT word, count "
		"FROM bigram "
		"WHERE word_1 = \"" + word_1 + "\" "
		"AND word LIKE \"" + prefix + "\" "
		"ORDER BY count DESC;";
		
        //	result = (*sqlite_exec_handle)( db,
	result = sqlite_exec( db,
					query.c_str(),
					buildPrediction,
					&data,
					NULL );

	assert( result == SQLITE_OK );


	// get most likely trigrams having matching w_2, w_1 whose w contains prefix
	Prediction predTrigrams;

	data.predPtr = &predTrigrams;
//	data.predSize = atoi(
//		getOptionValue( "MAX_PARTIAL_PREDICTION_SIZE" ).c_str()
//		);
	
	query = "SELECT word, count "
		"FROM trigram "
		"WHERE word_2 = \"" + word_2 + "\" "
		"AND word_1 = \"" + word_1 + "\" "
		"AND word LIKE \"" + prefix + "\" "
		"ORDER BY count DESC;";
        //	result = (*sqlite_exec_handle)( db,
	result = sqlite_exec( db,
					query.c_str(),
					buildPrediction,
					&data,
					NULL );
	assert( result == SQLITE_OK );
	

	Prediction p;     // combined result of uni/bi/tri gram predictions
	std::string word; // pivot unigram word (used in next for loop)
	double ccount;    // combined count

	// compute smoothed probability estimation

	// TODO !!!!!!!! Everything should be scaled down to probabilities!!!
	// TODO That means that counts should be scaled down to values between
	// TODO 0 and 1. We need total word count to do that.

	// TODO : after correct word has been found in inner loops, execution
	// TODO : can break out of it.
	for( int i = 0; i < predUnigrams.getSize(); i++ ) {

	    word   = predUnigrams.getSuggestion( i ).getWord();
	    ccount = atof( getOptionValue( "UNIGRAM_WEIGHT" ).c_str() ) *
		     predUnigrams.getSuggestion( i ).getProbability();

	    for( int j = 0; j < predBigrams.getSize(); j++ ) {

		if( predBigrams.getSuggestion(j).getWord() == word ) {

		    for( int k = 0; k < predTrigrams.getSize(); k++ ) {

		        if( predTrigrams.getSuggestion(k).getWord() == word ) {

			    ccount += atof( 
		                getOptionValue( "TRIGRAM_WEIGHT" ).c_str() ) *
				predTrigrams.getSuggestion(k).getProbability();
				
			}
		    }
		    
		    ccount += atof( getOptionValue("BIGRAM_WEIGHT").c_str() ) *
			    predBigrams.getSuggestion(j).getProbability();

		}
		
	    }
	    
	    p.addSuggestion( Suggestion( word, ccount ) );

	}

	return p; // Return combined prediction
}


void SmoothedCountPlugin::learn()
{
	std::cout << "SmoothedCountPlugin::learn() method called"
		  << std::endl;
	std::cout << "SmoothedCountPlugin::learn() method exited"
		  << std::endl;
}



void SmoothedCountPlugin::extract()
{
	std::cout << "SmoothedCountPlugin::extract() method called"
		  << std::endl;
	std::cout << "SmoothedCountPlugin::extract() method exited"
		  << std::endl;
}



void SmoothedCountPlugin::train()
{
	std::cout << "SmoothedCountPlugin::train() method called"
		  << std::endl;
	std::cout << "SmoothedCountPlugin::train() method exited"
		  << std::endl;
}


/** Convert string to lowercase.
    TODO: This method might be in a library.

*/
std::string SmoothedCountPlugin::strtolower(const std::string& str)const
{
//	char* lowstr = new char[ str.length() + 1 ];
//
//	std::string::const_iterator i = str.begin();
//	int j = 0;
//	while( i != str.end() ) {
//		lowstr[ j ] = tolower( *i );
//		i++;
//		j++;
//	}
//
//	return static_cast<std::string>( lowstr );

	std::string lowstr = str;
	for( std::string::iterator i = lowstr.begin();
	     i != lowstr.end();
	     i++ )
		*i = tolower( *i );

	return lowstr;

}



/** SQLite callback function
    Builds prediction from query results.

*/
int buildPrediction( void* callbackDataPtr,
		     int argc,
		     char** argv,
		     char** column )
{
	// cast pointer to void back to pointer to CallbackData object
	CallbackData* dataPtr = static_cast<CallbackData*>(callbackDataPtr);

	Prediction* predictionPtr = dataPtr->predPtr;
	int maxPredictionSize = dataPtr->predSize;

	if( predictionPtr->getSize() > maxPredictionSize ) {
		return 1;
	} else {

		if( argc == 2 &&
		    strcmp( "word", column[ 0 ] ) == 0 &&
		    strcmp( "count", column[ 1 ] ) == 0 ) {
			
			predictionPtr->addSuggestion( 
				Suggestion( argv[ argc - 2 ],
					    atof( argv[ argc - 1 ] )
					)
				);
			
		} else {
			std::cerr << "Invalid invocation of buildPrediction method!"
				  << std::endl;
			exit( 1 );
		}
	}
	return 0;
}







extern "C" Plugin* create( HistoryTracker& ht )
{
	return new SmoothedCountPlugin( ht );
}

extern "C" void destroy( Plugin *p )
{
	delete p;
}


