
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


#include "plugins/smoothedNgramPlugin.h"
#include <sstream>

#ifdef DEBUG
# define LOG(x) std::cerr << x << std::endl
#else
# define LOG(x) /* x */
#endif

SmoothedNgramPlugin::SmoothedNgramPlugin(Profile* profile, ContextTracker* ct)
    : Plugin(profile,
	     ct,
             "SmoothedNgramPlugin",
             "SmoothedNgramPlugin, a linear interpolating unigram bigram trigram plugin",
             "SmoothedNgramPlugin, long description." )
{
    LOG("[SmoothedNgramPlugin] Entering SmoothedNgramPlugin::SmoothedNgramPlugin()");

    Variable variable;
    variable.push_back("Soothsayer");
    variable.push_back("Plugins");
    variable.push_back("SmoothedNgramPlugin");

    Value value;

    try {
	variable.push_back("MAX_PARTIAL_PREDICTION_SIZE");
	value = profile->getConfig(variable);
	LOG("[SmoothedNgramPlugin] MAX_PARTIAL_PREDICTION_SIZE: " + value);
	max_partial_prediction_size = toInt(value);
	variable.pop_back();

	variable.push_back("DBFILENAME");
	value = profile->getConfig(variable);
	LOG("[SmoothedNgramPlugin] DBFILENAME: " + value);
	dbfilename = value;
	variable.pop_back();

	variable.push_back("DELTAS");
	value = profile->getConfig(variable);
	LOG("[SmoothedNgramPlugin] DELTAS: " + value);
	std::stringstream ss_deltas(value);
	std::string delta;
	while (ss_deltas >> delta) {
	    LOG("[SmoothedNgramPlugin] Pushing delta: " + delta);
	    deltas.push_back(toDouble(delta));
	}
	variable.pop_back();

    } catch (Profile::ProfileException ex) {
	std::cerr << "[SmoothedNgramPlugin] Caught ProfileException: " << ex.what() << std::endl;
    }

    // open database connector
    db = new SqliteDatabaseConnector(dbfilename);
    
    LOG("[SmoothedNgramPlugin] Exiting SmoothedNgramPlugin::SmoothedNgramPlugin()");
}



SmoothedNgramPlugin::~SmoothedNgramPlugin()
{
    delete db;
}

/** \brief Builds the required n-gram and returns its count.
 *
 * \param tokens tokens[i] contains ContextTracker::getToken(i)
 * \param offset entry point into tokens, must be a non-positive number
 * \param ngram_size size of the ngram whose count is returned, must not be greater than tokens size
 * \return count of the ngram built based on tokens, offset and ngram_size
 *
 * \verbatim
   Let tokens = [ "how", "are", "you", "today" ];

   count(tokens,  0, 3) returns the count associated with 3-gram [ "are", "you", "today" ].
   count(tokens, -1, 2) returns the count associated with 2-gram [ "are", "you" ];
 * \endverbatim
 *
 */
unsigned int SmoothedNgramPlugin::count(const std::vector<std::string>& tokens, int offset, int ngram_size) const
{
    assert(offset <= 0); // TODO: handle this better
    assert(ngram_size >= 0);

    if (ngram_size > 0) {
	Ngram ngram(ngram_size);
	copy(tokens.end() - ngram_size + offset , tokens.end() + offset, ngram.begin());

#ifdef DEBUG
	std::cerr << "[SmoothedNgramPlugin] ngram: ";
	for (int j = 0; j < ngram.size(); j++) {
	    std::cerr << ngram[j] << ' ';
	}
	std::cerr << std::endl;
#endif

	return db->getNgramCount(ngram);
    } else {
	return db->getUnigramCountsSum();
    }
}

Prediction SmoothedNgramPlugin::predict() const
{
    LOG("[SmoothedNgramPlugin] Entering SmoothedNgramPlugin::predict()");

    // Result prediction
    Prediction prediction;

    // n-gram cardinality (i.e. what is the n in n-gram?)
    int cardinality = deltas.size();

    // Cache all the needed tokens.
    // tokens[k] corresponds to w_{i-k} in the generalized smoothed
    // n-gram probability formula
    //
    std::vector<std::string> tokens(cardinality);
    for (int i = 0; i < cardinality; i++) {
	tokens[cardinality - 1 - i] = strtolower(contextTracker->getToken(i));
	LOG("[SmoothedNgramPlugin] Cached tokens[" << cardinality - 1 - i << "] = " << tokens[cardinality - 1 - i]);
    }

    // Get table of possible prefix completitions. The possible prefix
    // completions are obtained from the _1_gram table because in a
    // well-constructed ngram database the _1_gram table will contain
    // all tokens contained in any other table. The _1_gram counts,
    // however, will take precedence over the higher-order counts.
    //
    // Perhaps it is worth investigating the effectiveness of sourcing
    // the initial prefix completion list from a different table. One
    // possible idea is to use the table that has the strongest
    // weight.
    //
    Ngram prefix_gram;
    prefix_gram.push_back(tokens[cardinality - 1]);
    db->beginTransaction();
    NgramTable prefixCompletionTable = db->getNgramLikeTable(prefix_gram);
    db->endTransaction();

    // let's retrieve all the other counts I need
    db->beginTransaction();
    for (int j = 0; (j < prefixCompletionTable.size() && j < max_partial_prediction_size); j++) {
        // get w_i
	tokens[cardinality - 1] = prefixCompletionTable[j][0];
	LOG("[SmoothedNgramPlugin] ------------------");
	LOG("[SmoothedNgramPlugin] w_i: " << tokens[cardinality - 1]);
    
	double probability = 0;
	for (int k = 0; k < cardinality; k++) {
	    double numerator = count(tokens, 0, k+1);
	    double denominator = count(tokens, -1, k);
	    LOG("[SmoothedNgramPlugin] numerator:   " << numerator);
	    LOG("[SmoothedNgramPlugin] denominator: " << denominator);
	    assert(numerator <= denominator);

	    double frequency = ((denominator > 0) ? (numerator / denominator) : 0);
	    LOG("[SmoothedNgramPlugin] frequency:   " << frequency);
	    assert(frequency <= 1);

	    LOG("[SmoothedNgramPlugin] delta:       " << deltas[k]);
	    probability += deltas[k] * frequency;
	}

	if (probability > 0) {
	    prediction.addSuggestion(Suggestion(tokens[cardinality - 1], probability));
	}
    }
    db->endTransaction();

    LOG("[SmoothedNgramPlugin] "           );
    LOG("[SmoothedNgramPlugin] Prediction:");
    LOG("[SmoothedNgramPlugin] -----------");
    LOG(                       prediction  );
	
    return prediction;
}


void SmoothedNgramPlugin::learn()
{
    LOG("[SmoothedNgramPlugin] learn() method called");
    LOG("[SmoothedNgramPlugin::learn() method exited");
}



void SmoothedNgramPlugin::extract()
{
    LOG("SmoothedNgramPlugin::extract() method called");
    LOG("SmoothedNgramPlugin::extract() method exited");
}



void SmoothedNgramPlugin::train()
{
    LOG("SmoothedNgramPlugin::train() method called");
    LOG("SmoothedNgramPlugin::train() method exited");
}
