
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


#include "smoothedNgramPredictor.h"

#include <sstream>
#include <algorithm>


SmoothedNgramPredictor::SmoothedNgramPredictor(Configuration* config, ContextTracker* ct, const char* name)
    : Predictor(config,
		ct,
		name,
		"SmoothedNgramPredictor, a linear interpolating n-gram predictor",
		"SmoothedNgramPredictor, long description." ),
      db (0),
      cardinality (0),
      learn_mode_set (false),
      dispatcher (this)
{
    LOGGER     = PREDICTORS + name + ".LOGGER";
    DBFILENAME = PREDICTORS + name + ".DBFILENAME";
    DELTAS     = PREDICTORS + name + ".DELTAS";
    LEARN      = PREDICTORS + name + ".LEARN";
    DATABASE_LOGGER = PREDICTORS + name + ".DatabaseConnector.LOGGER";

    // build notification dispatch map
    dispatcher.map (config->find (LOGGER), & SmoothedNgramPredictor::set_logger);
    dispatcher.map (config->find (DATABASE_LOGGER), & SmoothedNgramPredictor::set_database_logger_level);
    dispatcher.map (config->find (DBFILENAME), & SmoothedNgramPredictor::set_dbfilename);
    dispatcher.map (config->find (DELTAS), & SmoothedNgramPredictor::set_deltas);
    dispatcher.map (config->find (LEARN), & SmoothedNgramPredictor::set_learn);
}



SmoothedNgramPredictor::~SmoothedNgramPredictor()
{
    delete db;
}


void SmoothedNgramPredictor::set_dbfilename (const std::string& filename)
{
    dbfilename = filename;
    logger << INFO << "DBFILENAME: " << dbfilename << endl;

    init_database_connector_if_ready ();
}


void SmoothedNgramPredictor::set_database_logger_level (const std::string& value)
{
    dbloglevel = value;
}


void SmoothedNgramPredictor::set_deltas (const std::string& value)
{
    std::stringstream ss_deltas(value);
    cardinality = 0;
    std::string delta;
    while (ss_deltas >> delta) {
        logger << DEBUG << "Pushing delta: " << delta << endl;
	deltas.push_back (Utility::toDouble (delta));
	cardinality++;
    }
    logger << INFO << "DELTAS: " << value << endl;
    logger << INFO << "CARDINALITY: " << cardinality << endl;

    init_database_connector_if_ready ();
}


void SmoothedNgramPredictor::set_learn (const std::string& value)
{
    learn_mode = Utility::isYes (value);
    logger << INFO << "LEARN: " << value << endl;

    learn_mode_set = true;

    init_database_connector_if_ready ();
}


void SmoothedNgramPredictor::init_database_connector_if_ready ()
{
    // we can only init the sqlite database connector once we know the
    // following:
    //  - what database file we need to open
    //  - what cardinality we expect the database file to be
    //  - whether we need to open the database in read only or
    //    read/write mode (learning requires read/write access)
    //
    if (! dbfilename.empty()
        && cardinality > 0
        && learn_mode_set ) {

        delete db;

        if (dbloglevel.empty ()) {
	    // open database connector
	    db = new SqliteDatabaseConnector(dbfilename,
					     cardinality,
					     learn_mode);
        } else {
            // open database connector with logger lever
            db = new SqliteDatabaseConnector(dbfilename,
					     cardinality,
					     learn_mode,
					     dbloglevel);
        }
    }
}


// convenience function to convert ngram to string
//
static std::string ngram_to_string(const Ngram& ngram)
{
    const char separator[] = "|";
    std::string result = separator;

    for (Ngram::const_iterator it = ngram.begin();
	 it != ngram.end();
	 it++)
    {
	result += *it + separator;
    }

    return result;
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
unsigned int SmoothedNgramPredictor::count(const std::vector<std::string>& tokens, int offset, int ngram_size) const
{
    unsigned int result = 0;

    assert(offset <= 0);      // TODO: handle this better
    assert(ngram_size >= 0);

    if (ngram_size > 0) {
	Ngram ngram(ngram_size);
	copy(tokens.end() - ngram_size + offset , tokens.end() + offset, ngram.begin());
	result = db->getNgramCount(ngram);
	logger << DEBUG << "count ngram: " << ngram_to_string (ngram) << " : " << result << endl;
    } else {
	result = db->getUnigramCountsSum();
	logger << DEBUG << "unigram counts sum: " << result << endl;
    }

    return result;
}

Prediction SmoothedNgramPredictor::predict(const size_t max_partial_prediction_size, const char** filter) const
{
    logger << DEBUG << "predict()" << endl;

    // Result prediction
    Prediction prediction;

    // Cache all the needed tokens.
    // tokens[k] corresponds to w_{i-k} in the generalized smoothed
    // n-gram probability formula
    //
    std::vector<std::string> tokens(cardinality);
    for (int i = 0; i < cardinality; i++) {
	tokens[cardinality - 1 - i] = contextTracker->getToken(i);
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
    // getUnigramCountsSum is an expensive SQL query
    // caching it here saves much time later inside the loop
    int unigrams_counts_sum = db->getUnigramCountsSum(); 
    for (size_t j = 0; (j < prefixCompletionCandidates.size() && j < max_partial_prediction_size); j++) {
        // store w_i candidate at end of tokens
        tokens[cardinality - 1] = prefixCompletionCandidates[j];

	logger << DEBUG << "------------------" << endl;
	logger << DEBUG << "w_i: " << tokens[cardinality - 1] << endl;

	double probability = 0;
	for (int k = 0; k < cardinality; k++) {
	    double numerator = count(tokens, 0, k+1);
	    // reuse cached unigrams_counts_sum to speed things up
	    double denominator = (k == 0 ? unigrams_counts_sum : count(tokens, -1, k));
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

void SmoothedNgramPredictor::learn(const std::vector<std::string>& change)
{
    logger << INFO << "learn(\"" << ngram_to_string(change) << "\")" << endl;

    if (learn_mode) {
	// learning is turned on

	std::map<std::list<std::string>, int> ngramMap;

	// build up ngram map for all cardinalities
	// i.e. learn all ngrams and counts in memory
	for (size_t curr_cardinality = 1;
	     curr_cardinality < cardinality + 1;
	     curr_cardinality++)
	{
	    int change_idx = 0;
	    int change_size = change.size();

	    std::list<std::string> ngram_list;

	    // take care of first N-1 tokens
	    for (int i = 0;
		 (i < curr_cardinality - 1 && change_idx < change_size);
		 i++)
	    {
		ngram_list.push_back(change[change_idx]);
		change_idx++;
	    }

	    while (change_idx < change_size)
	    {
		ngram_list.push_back(change[change_idx++]);
		ngramMap[ngram_list] = ngramMap[ngram_list] + 1;
		ngram_list.pop_front();
	    }
	}

	// use (past stream - change) to learn token at the boundary
	// change, i.e.
	//

	// if change is "bar foobar", then "bar" will only occur in a
	// 1-gram, since there are no token before it. By dipping in
	// the past stream, we additional context to learn a 2-gram by
	// getting extra tokens (assuming past stream ends with token
	// "foo":
	//
	// <"foo", "bar"> will be learnt
	//
	// We do this till we build up to n equal to cardinality.
	//
	// First check that change is not empty (nothing to learn) and
	// that change and past stream match by sampling first and
	// last token in change and comparing them with corresponding
	// tokens from past stream
	//
	if (change.size() > 0 &&
	    change.back() == contextTracker->getToken(1) &&
	    change.front() == contextTracker->getToken(change.size()))
	{
	    // create ngram list with first (oldest) token from change
	    std::list<std::string> ngram_list(change.begin(), change.begin() + 1);

	    // prepend token to ngram list by grabbing extra tokens
	    // from past stream (if there are any) till we have built
	    // up to n==cardinality ngrams, and commit them to
	    // ngramMap
	    //
	    for (int tk_idx = 1;
		 ngram_list.size() < cardinality;
		 tk_idx++)
	    {
		// getExtraTokenToLearn returns tokens from
		// past stream that come before and are not in
		// change vector
		//
		std::string extra_token = contextTracker->getExtraTokenToLearn(tk_idx, change);
		logger << DEBUG << "Adding extra token: " << extra_token << endl;

		if (extra_token.empty())
		{
		    break;
		}
		ngram_list.push_front(extra_token);

		ngramMap[ngram_list] = ngramMap[ngram_list] + 1;
	    }
	}

	// then write out to language model database
	try
	{
	    db->beginTransaction();

	    std::map<std::list<std::string>, int>::const_iterator it;
	    for (it = ngramMap.begin(); it != ngramMap.end(); it++)
	    {
		// convert ngram from list to vector based Ngram
		Ngram ngram((it->first).begin(), (it->first).end());

		// update the counts
		int count = db->getNgramCount(ngram);
		if (count > 0)
		{
		    // ngram already in database, update count
		    db->updateNgram(ngram, count + it->second);
		    check_learn_consistency(ngram);
		}
		else
		{
		    // ngram not in database, insert it
		    db->insertNgram(ngram, it->second);
		}
	    }

	    db->endTransaction();
	    logger << INFO << "Committed learning update to database" << endl;
	}
	catch (SqliteDatabaseConnector::SqliteDatabaseConnectorException& ex)
	{
	    db->rollbackTransaction();
	    logger << ERROR << "Rolling back learning update : " << ex.what() << endl;
	    throw;
	}
    }

    logger << DEBUG << "end learn()" << endl;
}

void SmoothedNgramPredictor::check_learn_consistency(const Ngram& ngram) const
{
    // no need to begin a new transaction, as we'll be called from
    // within an existing transaction from learn()

    // BEWARE: if the previous sentence is not true, then performance
    // WILL suffer!

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

void SmoothedNgramPredictor::update (const Observable* var)
{
    logger << DEBUG << "About to invoke dispatcher: " << var->get_name () << " - " << var->get_value() << endl;
    dispatcher.dispatch (var);
}
