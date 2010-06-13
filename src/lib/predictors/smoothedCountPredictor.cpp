
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
                                                                             *
                                                                **********(*)*/


#include "smoothedCountPredictor.h"

#ifdef HAVE_STRING_H
# include <string.h>
#endif

const char* SmoothedCountPredictor::LOGGER         = "Presage.Predictors.SmoothedCountPredictor.LOGGER";
const char* SmoothedCountPredictor::UNIGRAM_WEIGHT = "Presage.Predictors.SmoothedCountPredictor.UNIGRAM_WEIGHT";
const char* SmoothedCountPredictor::BIGRAM_WEIGHT  = "Presage.Predictors.SmoothedCountPredictor.BIGRAM_WEIGHT";
const char* SmoothedCountPredictor::TRIGRAM_WEIGHT = "Presage.Predictors.SmoothedCountPredictor.TRIGRAM_WEIGHT";
const char* SmoothedCountPredictor::DBFILENAME     = "Presage.Predictors.SmoothedCountPredictor.DBFILENAME";

SmoothedCountPredictor::SmoothedCountPredictor(Configuration* config, ContextTracker* ct)
    : Predictor(config,
		ct,
		"SmoothedCountPredictor",
		"SmoothedCountPredictor, a linear interpolating unigram bigram trigram predictor",
		"SmoothedCountPredictor, long description." 
	),
      dispatcher (this)
{
    // build notification dispatch map
    dispatcher.map (config->find (LOGGER), & SmoothedCountPredictor::set_logger);
    dispatcher.map (config->find (UNIGRAM_WEIGHT), & SmoothedCountPredictor::set_unigram_weight);
    dispatcher.map (config->find (BIGRAM_WEIGHT), & SmoothedCountPredictor::set_bigram_weight);
    dispatcher.map (config->find (TRIGRAM_WEIGHT), & SmoothedCountPredictor::set_trigram_weight);
    dispatcher.map (config->find (DBFILENAME), & SmoothedCountPredictor::set_dbfilename);


    // open database
#if defined(HAVE_SQLITE3_H)
    int result = sqlite3_open(dbfilename.c_str(), &db);
    assert(result == SQLITE_OK);
#elif defined(HAVE_SQLITE_H)
    db = sqlite_open(dbfilename.c_str(), 0777, NULL);
    assert(db != NULL);
#endif
}



SmoothedCountPredictor::~SmoothedCountPredictor()
{
#if defined(HAVE_SQLITE3_H)
    sqlite3_close(db);
#elif defined(HAVE_SQLITE_H)
    sqlite_close(db);
#endif
}


void SmoothedCountPredictor::set_unigram_weight (const std::string& value)
{
    unigram_weight = Utility::toDouble (value);
    logger << INFO << "UNIGRAM_WEIGHT: " << value << endl;
}

void SmoothedCountPredictor::set_bigram_weight (const std::string& value)
{
    bigram_weight = Utility::toDouble (value);
    logger << INFO << "BIGRAM_WEIGHT: " << value << endl;
}

void SmoothedCountPredictor::set_trigram_weight (const std::string& value)
{
    trigram_weight = Utility::toDouble (value);
    logger << INFO << "TRIGRAM_WEIGHT: " << value << endl;
}

void SmoothedCountPredictor::set_dbfilename (const std::string& value)
{
    dbfilename = value;
    logger << INFO << "DBFILENAME: " << value << endl;
}


Prediction SmoothedCountPredictor::predict(const size_t max_partial_predictions_size, const char** filter) const
{
    // get w_2, w_1, and prefix from HistoryTracker object
    std::string prefix = strtolower( contextTracker->getPrefix() );
    std::string word_1 = strtolower( contextTracker->getToken(1) );
    std::string word_2 = strtolower( contextTracker->getToken(2) );
    
    std::string query; // string used to build sql query
    int result;        // database interrogation diagnostic
    CallbackData data; // data to pass through to callback function
	

    // get most likely unigrams whose w contains prefix
    Prediction predUnigrams;
    
    data.predPtr = &predUnigrams;
    data.predSize = MAX_PARTIAL_PREDICTION_SIZE;
    
    query = 
	"SELECT word, count "
	"FROM _1_gram "
	"WHERE word LIKE \"" + prefix + "%\" "
	"ORDER BY count DESC;";
    
#if defined(HAVE_SQLITE3_H)
    result = sqlite3_exec(
#elif defined(HAVE_SQLITE_H)
	result = sqlite_exec(
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
	"FROM _2_gram "
	"WHERE word_1 = \"" + word_1 + "\" "
	"AND word LIKE \"" + prefix + "\" "
	"ORDER BY count DESC;";
    
#if defined(HAVE_SQLITE3_H)
	result = sqlite3_exec(
#elif defined(HAVE_SQLITE_H)
	    result = sqlite_exec(
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
	    "FROM _3_gram "
	    "WHERE word_2 = \"" + word_2 + "\" "
	    "AND word_1 = \"" + word_1 + "\" "
	    "AND word LIKE \"" + prefix + "\" "
	    "ORDER BY count DESC;";
    
#if defined(HAVE_SQLITE3_H)
	    result = sqlite3_exec(
#elif defined(HAVE_SQLITE_H)
		result = sqlite_exec(
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
		for (size_t i = 0; i < predUnigrams.size(); i++) {

		    word   = predUnigrams.getSuggestion( i ).getWord();
		    ccount = unigram_weight *
			predUnigrams.getSuggestion( i ).getProbability();
	
		    for (size_t j = 0; j < predBigrams.size(); j++) {

			if( predBigrams.getSuggestion(j).getWord() == word ) {
		
			    for (size_t k = 0; k < predTrigrams.size(); k++ ) {
		    
				if( predTrigrams.getSuggestion(k).getWord() == word ) {
			
				    ccount += trigram_weight *
					predTrigrams.getSuggestion(k).getProbability();
			
				}
			    }
		
			    ccount += bigram_weight *
				predBigrams.getSuggestion(j).getProbability();
		
			}
	    
		    }
	
		    p.addSuggestion( Suggestion( word, ccount ) );
	
		}

		return p; // Return combined prediction
		}


	    void SmoothedCountPredictor::learn(const std::vector<std::string>& change)
	    {
		logger << DEBUG << "learn() method called" << endl;
		logger << DEBUG << "learn() method exited" << endl;
	    }


/** Convert string to lowercase.
    TODO: This method might be in a library.

*/
	    std::string SmoothedCountPredictor::strtolower(const std::string& str)const
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
		size_t maxPredictionSize = dataPtr->predSize;

		if (predictionPtr->size() > maxPredictionSize) {
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
		    }
		}
		return 0;
	    }

	    void SmoothedCountPredictor::update (const Observable* var)
	    {
		logger << DEBUG << "About to invoke dispatcher: " << var->get_name () << " - " << var->get_value() << endl;
		dispatcher.dispatch (var);
	    }
