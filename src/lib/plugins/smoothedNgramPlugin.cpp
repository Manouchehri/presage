
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


#include "plugins/smoothedNgramPlugin.h"

#include <sstream>
#include <algorithm>

const Variable SmoothedNgramPlugin::LOGGER     = Variable("Presage.Plugins.SmoothedNgramPlugin.LOGGER");
const Variable SmoothedNgramPlugin::DBFILENAME = Variable("Presage.Plugins.SmoothedNgramPlugin.DBFILENAME");
const Variable SmoothedNgramPlugin::DELTAS     = Variable("Presage.Plugins.SmoothedNgramPlugin.DELTAS");
const Variable SmoothedNgramPlugin::LEARN      = Variable("Presage.Plugins.SmoothedNgramPlugin.LEARN");
const Variable SmoothedNgramPlugin::DATABASE_LOGGER = Variable("Presage.Plugins.SmoothedNgramPlugin.DatabaseConnector.LOGGER");

SmoothedNgramPlugin::SmoothedNgramPlugin(Configuration* config, ContextTracker* ct)
    : Plugin(config,
	     ct,
             "SmoothedNgramPlugin",
             "SmoothedNgramPlugin, a linear interpolating n-gram plugin",
             "SmoothedNgramPlugin, long description." )
{
    Value value;

    try {
	value = config->get(LOGGER);
	logger << setlevel(value);
	logger << INFO << "LOGGER: " << value << endl;
    } catch (Configuration::ConfigurationException ex) {
	logger << WARN << "Caught ConfigurationException: " << ex.what() << endl;
    }

    try {
	value = config->get(DBFILENAME);
	logger << INFO << "DBFILENAME: " << value << endl;
	dbfilename = value;

	value = config->get(DELTAS);
	logger << INFO << "DELTAS: " << value << endl;
	std::stringstream ss_deltas(value);
	std::string delta;
	while (ss_deltas >> delta) {
	    logger << DEBUG << "Pushing delta: " << delta << endl;
	    deltas.push_back(toDouble(delta));
	}

    } catch (Configuration::ConfigurationException ex) {
	logger << ERROR << "Caught fatal ConfigurationException: " << ex.what() << endl;
	throw PresageException("Unable to init " + name + " predictive plugin.");
    }

    try {
	value = config->get(DATABASE_LOGGER);
    } catch (Configuration::ConfigurationException& ex) {
        logger << WARN << "ConfigurationException while trying to fetch DatabaseConnector logger level." << endl;
	db = new SqliteDatabaseConnector(dbfilename);
    }

    // open database connector
    db = new SqliteDatabaseConnector(dbfilename, value);
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

	logger << DEBUG << "count ngram: ";
	for (size_t j = 0; j < ngram.size(); j++) {
	    logger << DEBUG << ngram[j] << ' ';
	}
	logger << DEBUG << endl;

	return db->getNgramCount(ngram);
    } else {
	return db->getUnigramCountsSum();
    }
}

Prediction SmoothedNgramPlugin::predict(const size_t max_partial_prediction_size, const char** filter) const
{
    logger << DEBUG << "predict()" << endl;

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
	logger << DEBUG << "Cached tokens[" << cardinality - 1 - i << "] = " << tokens[cardinality - 1 - i] << endl;
    }

    // Generate list of prefix completition candidates.
    //
    // The prefix completion candidates used to be obtained from the
    // _1_gram table because in a well-constructed ngram database the
    // _1_gram table (which contains all known tokens). However, this
    // introduced a skew, since the unigram counts will take
    // precedence over the higher-order counts.
    //
    // The current solution retrieves candidates from the highest
    // n-gram table, falling back on lower order n-gram tables if
    // initial completion set is smaller than required.
    //
    std::vector<std::string> prefixCompletionCandidates;
    for (size_t k = cardinality; (k > 0 && prefixCompletionCandidates.size() < max_partial_prediction_size); k--) {
        logger << DEBUG << "Building partial prefix completion table of cardinality: " << k << endl;
        // create n-gram used to retrieve initial prefix completion table
        Ngram prefix_ngram(k);
        copy(tokens.end() - k, tokens.end(), prefix_ngram.begin());

	if (logger.shouldLog()) {
	    logger << DEBUG << "prefix_ngram: ";
	    for (size_t r = 0; r < prefix_ngram.size(); r++) {
		logger << DEBUG << prefix_ngram[r] << ' ';
	    }
	    logger << DEBUG << endl;
	}

        // obtain initial prefix completion candidates
        db->beginTransaction();

        NgramTable partial;

        if (filter == 0) {
	    partial = db->getNgramLikeTable(prefix_ngram,max_partial_prediction_size - prefixCompletionCandidates.size());
	} else {
	    partial = db->getNgramLikeTableFiltered(prefix_ngram,filter, max_partial_prediction_size - prefixCompletionCandidates.size());
	}

        db->endTransaction();

	if (logger.shouldLog()) {
	    logger << DEBUG << "partial prefixCompletionCandidates" << endl
	           << DEBUG << "----------------------------------" << endl;
	    for (size_t j = 0; j < partial.size(); j++) {
		for (size_t k = 0; k < partial[j].size(); k++) {
		    logger << DEBUG << partial[j][k] << " ";
		}
		logger << endl;
	    }
	}

        logger << DEBUG << "Partial prefix completion table contains " << partial.size() << " potential completions." << endl;

        // append newly discovered potential completions to prefix
        // completion candidates array to fill it up to
        // max_partial_prediction_size
        //
        std::vector<Ngram>::const_iterator it = partial.begin();
        while (it != partial.end() && prefixCompletionCandidates.size() < max_partial_prediction_size) {
            // only add new candidates, iterator it points to Ngram,
            // it->end() - 2 points to the token candidate
            //
            std::string candidate = *(it->end() - 2);
            if (find(prefixCompletionCandidates.begin(),
                     prefixCompletionCandidates.end(),
                     candidate) == prefixCompletionCandidates.end()) {
                prefixCompletionCandidates.push_back(candidate);
            }
            it++;
        }
    }

    if (logger.shouldLog()) {
	logger << DEBUG << "prefixCompletionCandidates" << endl
	       << DEBUG << "--------------------------" << endl;
	for (size_t j = 0; j < prefixCompletionCandidates.size(); j++) {
	    logger << DEBUG << prefixCompletionCandidates[j] << endl;
	}
    }

    // compute smoothed probabilities for all candidates
    //
    db->beginTransaction();
    for (size_t j = 0; (j < prefixCompletionCandidates.size() && j < max_partial_prediction_size); j++) {
        // store w_i candidate at end of tokens
        tokens[cardinality - 1] = prefixCompletionCandidates[j];

	logger << DEBUG << "------------------" << endl;
	logger << DEBUG << "w_i: " << tokens[cardinality - 1] << endl;

	double probability = 0;
	for (int k = 0; k < cardinality; k++) {
	    double numerator = count(tokens, 0, k+1);
	    double denominator = count(tokens, -1, k);
	    double frequency = ((denominator > 0) ? (numerator / denominator) : 0);
	    probability += deltas[k] * frequency;

	    logger << DEBUG << "numerator:   " << numerator << endl;
	    logger << DEBUG << "denominator: " << denominator << endl;
	    logger << DEBUG << "frequency:   " << frequency << endl;
	    logger << DEBUG << "delta:       " << deltas[k] << endl;

            // for some sanity checks
	    assert(numerator <= denominator);
	    assert(frequency <= 1);
	}

        logger << DEBUG << "____________" << endl;
        logger << DEBUG << "probability: " << probability << endl;

	if (probability > 0) {
	    prediction.addSuggestion(Suggestion(tokens[cardinality - 1], probability));
	}
    }
    db->endTransaction();

    logger << DEBUG << "Prediction:" << endl;
    logger << DEBUG << "-----------" << endl;
    logger << DEBUG << prediction << endl;

    return prediction;
}


void SmoothedNgramPlugin::learn(const std::vector<std::string>& change)
{
    logger << INFO << "learn()" << endl;

    bool wanna_learn = false;
    try {
	wanna_learn = isTrue(configuration->get(LEARN));
    } catch (Configuration::ConfigurationException ex) {
	logger << WARN << "Caught ConfigurationException: " << ex.what() << endl;
    }

    if (wanna_learn) {
	// learning is turned on

	// n-gram cardinality (i.e. what is the n in n-gram?)
	int cardinality = deltas.size();

	std::string token;
	for (int curr_cardinality = 1;
	     curr_cardinality < cardinality + 1;
	     curr_cardinality++) {

	    // idx walks the change vector back to front
	    for (std::vector<std::string>::const_reverse_iterator idx = change.rbegin();
		 idx != change.rend();
		 idx++)
	    {
		Ngram ngram;

		// try to fill in the ngram to be learnt with change
		// tokens first
		for (std::vector<std::string>::const_reverse_iterator inner_idx = idx;
		     inner_idx != change.rend() && ngram.size() < curr_cardinality;
		     inner_idx++)
		{
		    ngram.insert(ngram.begin(), *inner_idx);
		}

		// then use past stream if ngram not filled in yet
		for (int tk_idx = 1;
		     ngram.size() < curr_cardinality;
		     tk_idx++)
		{		     
		    // ContextTracker already sees latest tokens that
		    // we need to learn, hence we need to look at the
		    // sliding window and obtain tokens from there.
		    //
		    // getSlidingWindowToken returns tokens from
		    // stream tied to sliding window from context
		    // change detector

		    ngram.insert(ngram.begin(), contextTracker->getSlidingWindowToken(tk_idx));
		}

		// now we have built the ngram we have to learn
		logger << INFO << "Considering to learn ngram: |";
		for (size_t j = 0; j < ngram.size(); j++) {
		    logger << INFO << ngram[j] << '|';
		}
		logger << INFO << endl;
		
		if (ngram.end() == find(ngram.begin(), ngram.end(), "")) {
		    // only learn ngram if it doesn't contain empty strings
		    db->beginTransaction();
		    
		    db->incrementNgramCount(ngram);
		    check_learn_consistency(ngram);
		    
		    db->endTransaction();
		    logger << INFO << "Committed ngram update to database" << endl;
		    
		} else {
		    logger << INFO << "Discarded ngram" << endl;
		}
	    }
	}
    }

    logger << DEBUG << "end learn()" << endl;
}

void SmoothedNgramPlugin::check_learn_consistency(const Ngram& ngram) const
{
    // no need to begin a new transaction, as we'll be called from
    // within an existing transaction from learn()

    // BEWARE: if the previous sentence is not true, then performance
    // will suffer!

    size_t size = ngram.size();
    for (size_t i = 0; i < size; i++) {
	if (count(ngram, -i, size - i) > count(ngram, -(i + 1), size - (i + 1))) {
	    logger << INFO << "consistency adjustment needed!" << endl;

	    int offset = -(i + 1);
	    int sub_ngram_size = size - (i + 1);

	    logger << DEBUG << "i: " << i << " | offset: " << offset << " | sub_ngram_size: " << sub_ngram_size << endl;

	    Ngram sub_ngram(sub_ngram_size); // need to init to right size for sub_ngram
	    copy(ngram.end() - sub_ngram_size + offset, ngram.end() + offset, sub_ngram.begin());

	    if (logger.shouldLog()) {
		logger << "ngram to be count adjusted is: ";
		for (size_t i = 0; i < sub_ngram.size(); i++) {
		    logger << sub_ngram[i] << ' ';
		}
		logger << endl;
	    }

	    db->incrementNgramCount(sub_ngram);
	    logger << DEBUG << "consistency adjusted" << endl;
	}
    }
}

void SmoothedNgramPlugin::extract()
{
    logger << DEBUG << "extract() method called" << endl;
    logger << DEBUG << "extract() method exited" << endl;
}



void SmoothedNgramPlugin::train()
{
    logger << DEBUG << "train() method called" << endl;
    logger << DEBUG << "train() method exited" << endl;
}
