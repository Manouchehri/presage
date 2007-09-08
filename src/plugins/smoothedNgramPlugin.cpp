
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

Prediction SmoothedNgramPlugin::predict(const int max_partial_prediction_size) const
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

    // Generate list of prefix completition candidates.
    // 
    // The prefix completion candidates used to be obtained from the
    // _1_gram table because in a well-constructed ngram database the
    // _1_gram table (which contains all known tokens). However, this
    // inroduced a skew, since the unigram counts will take precedence
    // over the higher-order counts.
    //
    // The current solution retrieves candidates from the highest
    // n-gram table, falling back on lower order n-gram tables if
    // initial completion set is smaller than required.
    //
    std::vector<std::string> prefixCompletionCandidates;
    for (int k = cardinality; (k > 0 && prefixCompletionCandidates.size() < max_partial_prediction_size); k--) {
        LOG("[SmoothedNgramPlugin] Building partial prefix completion table of cardinality: " << k);
        // create n-gram used to retrieve initial prefix completion table
        Ngram prefix_ngram(k);
        copy(tokens.end() - k, tokens.end(), prefix_ngram.begin());
#ifdef DEBUG
        std::cerr << "[SmoothedNgramPlugin] prefix_ngram: ";
        for (int r = 0; r < prefix_ngram.size(); r++) {
            std::cerr << prefix_ngram[r] << ' ';
        }
        std::cerr << std::endl;
#endif
        
        // obtain initial prefix completion candidates
        db->beginTransaction();
        NgramTable partial = db->getNgramLikeTable(prefix_ngram,
						   max_partial_prediction_size - prefixCompletionCandidates.size());
        db->endTransaction();

#ifdef DEBUG
        std::cerr << "[SmoothedNgramPlugin] partial prefixCompletionCandidates" << std::endl
                  << "[SmoothedNgramPlugin] ----------------------------------" << std::endl;
        for (int j = 0; j < partial.size(); j++) {
            std::cerr << "[SmoothedNgramPlugin] ";
            for (int k = 0; k < partial[j].size(); k++) {
                std::cerr << partial[j][k] << " ";
            }
            std::cerr << std::endl;
        }
#endif
        LOG("[SmoothedNgramPlugin] Partial prefix completion table contains " << partial.size() << " potential completions.");

        // append newly discovered potential completions to prefix
        // completion candidates array to fill it up to
        // max_partial_prediction_size
        //
        std::vector<Ngram>::const_iterator it = partial.begin();
        while (it != partial.end() && prefixCompletionCandidates.size() < max_partial_prediction_size) {
            // only add new candidates, iterator it points to Ngram,
            // it->end() - 2 points to the token candidate
            //
            if (find(prefixCompletionCandidates.begin(),
                     prefixCompletionCandidates.end(),
                     *(it->end() - 2)) == prefixCompletionCandidates.end()) {
                prefixCompletionCandidates.push_back(*(it->end() - 2));
            }
            it++;
        }
    }
    
#ifdef DEBUG
    std::cerr << "[SmoothedNgramPlugin] prefixCompletionCandidates" << std::endl
              << "[SmoothedNgramPlugin] --------------------------" << std::endl;
    for (int j = 0; j < prefixCompletionCandidates.size(); j++) {
        LOG("[SmoothedNgramPlugin] " << prefixCompletionCandidates[j]);
    }
#endif

    // compute smoothed probabilities for all candidates
    //
    db->beginTransaction();
    for (int j = 0; (j < prefixCompletionCandidates.size() && j < max_partial_prediction_size); j++) {
        // store w_i candidate at end of tokens
        tokens[cardinality - 1] = prefixCompletionCandidates[j];

	LOG("[SmoothedNgramPlugin] ------------------");
	LOG("[SmoothedNgramPlugin] w_i: " << tokens[cardinality - 1]);
    
	double probability = 0;
	for (int k = 0; k < cardinality; k++) {
	    double numerator = count(tokens, 0, k+1);
	    double denominator = count(tokens, -1, k);
	    double frequency = ((denominator > 0) ? (numerator / denominator) : 0);
	    probability += deltas[k] * frequency;

	    LOG("[SmoothedNgramPlugin] numerator:   " << numerator);
	    LOG("[SmoothedNgramPlugin] denominator: " << denominator);
	    LOG("[SmoothedNgramPlugin] frequency:   " << frequency);
	    LOG("[SmoothedNgramPlugin] delta:       " << deltas[k]);

            // for some sanity checks
	    assert(numerator <= denominator);
	    assert(frequency <= 1);
	}

        LOG("[SmoothedNgramPlugin] ____________ ");
        LOG("[SmoothedNgramPlugin] probability: " << probability);

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
