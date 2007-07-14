
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


#include "plugins/smoothedCountPlugin.h"


SmoothedCountPlugin::SmoothedCountPlugin(Profile* profile, HistoryTracker* ht)
	: Plugin(profile,
		 ht,
		 "SmoothedCountPlugin",
		 "SmoothedCountPlugin, a linear interpolating unigram bigram trigram plugin",
		 "SmoothedCountPlugin, long description." )
{

    Variable variable;
    variable.push_back("Soothsayer");
    variable.push_back("Plugins");
    variable.push_back("SmoothedCountPlugin");

    Value value;

    variable.push_back("UNIGRAM_WEIGHT");
    value = profile->getConfig(variable);
    UNIGRAM_WEIGHT = toDouble(value);
    variable.pop_back();

    variable.push_back("BIGRAM_WEIGHT");
    value = profile->getConfig(variable);
    BIGRAM_WEIGHT = toDouble(value);
    variable.pop_back();

    variable.push_back("TRIGRAM_WEIGHT");
    value = profile->getConfig(variable);
    TRIGRAM_WEIGHT = toDouble(value);
    variable.pop_back();

    variable.push_back("");
    value = profile->getConfig(variable);
    MAX_PARTIAL_PREDICTION_SIZE = toInt(value);
    variable.pop_back();

    variable.push_back("DBFILENAME");
    value = profile->getConfig(variable);
    DBFILENAME = value;
    variable.pop_back();

    // open database
#if defined(HAVE_SQLITE_H)
    db = sqlite_open(DBFILENAME.c_str(), 0777, NULL);
    assert(db != NULL);
#elif (HAVE_SQLITE3_H)
    int result = sqlite3_open(DBFILENAME.c_str(), &db);
    assert(result != SQLITE_OK);
#endif
}



SmoothedCountPlugin::~SmoothedCountPlugin()
{
#if defined(HAVE_SQLITE_H)
    sqlite_close(db);
#elif defined(HAVE_SQLITE3_H)
    sqlite3_close(db);
#endif
}



Prediction SmoothedCountPlugin::predict() const
{
    // get w_2, w_1, and prefix from HistoryTracker object
    std::string prefix = strtolower( historyTracker->getPrefix() );
    std::string word_1 = strtolower( historyTracker->getToken(1) );
    std::string word_2 = strtolower( historyTracker->getToken(2) );
    
    std::string query; // string used to build sql query
    int result;        // database interrogation diagnostic
    CallbackData data; // data to pass through to callback function
	

    // get most likely unigrams whose w contains prefix
    Prediction predUnigrams;
    
    data.predPtr = &predUnigrams;
    data.predSize = MAX_PARTIAL_PREDICTION_SIZE;
    
    query = 
	"SELECT word, count "
	"FROM unigram "
	"WHERE word LIKE \"" + prefix + "%\" "
	"ORDER BY count DESC;";
    
#if defined(HAVE_SQLITE_H)
    result = sqlite_exec(
#elif defined(HAVE_SQLITE3_H)
    result = sqlite3_exec(
#endif
	db,
	query.c_str(),
	buildPrediction,
	&data,
	NULL
    );
    assert(result == SQLITE_OK);


    // get most likely bigrams having matching w_1 whose w contains prefix
    Prediction predBigrams;
    
    data.predPtr = &predBigrams;
    
    query = 
    "SELECT word, count "
    "FROM bigram "
    "WHERE word_1 = \"" + word_1 + "\" "
    "AND word LIKE \"" + prefix + "\" "
    "ORDER BY count DESC;";
    
#if defined(HAVE_SQLITE_H)
    result = sqlite_exec(
#elif defined(HAVE_SQLITE3_H)
    result = sqlite3_exec(
#endif
	db,
	query.c_str(),
	buildPrediction,
	&data,
	NULL
    );
    assert(result == SQLITE_OK);


    // get most likely trigrams having matching w_2, w_1 whose w contains prefix
    Prediction predTrigrams;
    
    data.predPtr = &predTrigrams;
    
    query = 
    "SELECT word, count "
    "FROM trigram "
    "WHERE word_2 = \"" + word_2 + "\" "
    "AND word_1 = \"" + word_1 + "\" "
    "AND word LIKE \"" + prefix + "\" "
    "ORDER BY count DESC;";
    
#if defined(HAVE_SQLITE_H)
    result = sqlite_exec(
#elif defined(HAVE_SQLITE3_H)
    result = sqlite3_exec(
#endif
	db,
	query.c_str(),
	buildPrediction,
	&data,
	NULL
    );
    assert(result == SQLITE_OK);
	
    
    Prediction p;     // combined result of uni/bi/tri gram predictions
    std::string word; // pivot unigram word (used in next for loop)
    double ccount;    // combined count
    
    // compute smoothed probability estimation
    
    // TODO !!!!!!!! Everything should be scaled down to probabilities!!!
    // TODO That means that counts should be scaled down to values between
    // TODO 0 and 1. We need total word count to do that.
    
    // TODO : after correct word has been found in inner loops, execution
    // TODO : can break out of it.
    for( int i = 0; i < predUnigrams.size(); i++ ) {

	word   = predUnigrams.getSuggestion( i ).getWord();
	ccount = UNIGRAM_WEIGHT *
	    predUnigrams.getSuggestion( i ).getProbability();
	
	for( int j = 0; j < predBigrams.size(); j++ ) {

	    if( predBigrams.getSuggestion(j).getWord() == word ) {
		
		for( int k = 0; k < predTrigrams.size(); k++ ) {
		    
		    if( predTrigrams.getSuggestion(k).getWord() == word ) {
			
			ccount += TRIGRAM_WEIGHT *
			    predTrigrams.getSuggestion(k).getProbability();
			
		    }
		}
		
		ccount += BIGRAM_WEIGHT *
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

	if( predictionPtr->size() > maxPredictionSize ) {
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







extern "C" SmoothedCountPlugin* create(Profile* prof, HistoryTracker* ht)
{
    return new SmoothedCountPlugin(prof, ht);
}

extern "C" void destroy(SmoothedCountPlugin *p)
{
    delete p;
}
