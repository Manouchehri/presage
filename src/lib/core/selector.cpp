
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


#include "selector.h"
#include "utility.h"

const char* Selector::SUGGESTIONS = "Presage.Selector.SUGGESTIONS";
const char* Selector::REPEAT_SUGGESTIONS = "Presage.Selector.REPEAT_SUGGESTIONS";
const char* Selector::GREEDY_SUGGESTION_THRESHOLD = "Presage.Selector.GREEDY_SUGGESTION_THRESHOLD";

const char* Selector::LOGGER = "Presage.Selector.LOGGER";

Selector::Selector(Configuration* configuration, ContextTracker* ct)
    : contextTracker(ct),
      config(configuration),
      logger("Selector", std::cerr),
      dispatcher(this)
{
    // build notification dispatch map
    dispatcher.map (config->find (LOGGER), & Selector::set_logger);
    dispatcher.map (config->find (SUGGESTIONS), & Selector::set_suggestions);
    dispatcher.map (config->find (REPEAT_SUGGESTIONS), & Selector::set_repeat_suggestions);
    dispatcher.map (config->find (GREEDY_SUGGESTION_THRESHOLD), & Selector::set_greedy_suggestion_threshold);

    // set prefix
    previous_prefix = contextTracker->getPrefix();
}

Selector::~Selector()
{
    // nothing to do here, move along
}

std::vector<std::string> Selector::select( Prediction p )
{
    // copy words from Prediction.Suggestion.word in result vector
    std::vector<std::string> result;
    std::string token;
    for (size_t i=0 ; i<p.size() ; i++) {
	token =  p.getSuggestion(i).getWord();
	result.push_back(token);
	logger << DEBUG << "Added token to selector consideration set: " << token << endl;
    }
	
    // check whether user has not moved on to a new word
    if (contextTracker->contextChange()) {
	logger << DEBUG << "Context change detected." << endl;
	clearSuggestedWords();
    } else {
	logger << DEBUG << "No context change detected." << endl;
    }

    // filter out suggestions that do not satisfy repetition constraint
    if( !repeat_suggestions )
	repetitionFilter( result );

    // filter out suggestions that do not satisfy threshold constraint
    if( greedy_suggestion_threshold > 0 )
	thresholdFilter( result );

    // if more suggestions than required, trim suggestions down to requested number
    if( result.size() > suggestions ) {
	result.erase (result.begin() + suggestions, result.end());
    }

    // update suggested words set
    updateSuggestedWords( result );

    return result;
}


/** Trigger update of the suggested tokens cache
 *
 */
void Selector::update()
{
    // check whether user has not moved on to a new word
    if (contextTracker->contextChange()) {
	clearSuggestedWords();
    }
}


/** Adds suggestions to the set of previously selected suggestions.
 *
 */
void Selector::updateSuggestedWords( const std::vector<std::string>& v )
{
    std::vector<std::string>::const_iterator i = v.begin();
    while( i != v.end() ) {
	logger << DEBUG << "Adding token to suggested token set: " << *i << endl; 
	suggestedWords.insert( *i );
	i++;
    }

    logger << DEBUG << "Suggested words: ";
    for (StringSet::const_iterator it = suggestedWords.begin();
	 it != suggestedWords.end();
	 it++) {
	logger << *it << ' ';
    }
    logger << endl;
}


/** Clear the set of previously selected suggestions.
 *
 */
void Selector::clearSuggestedWords()
{
    logger << DEBUG << "Clearing previously suggested tokens set." << endl;
    suggestedWords.clear();
}

/** Filters out suggestions that have previously been selected in the current context.
 *
 * This filter removes the suggestions that have previously been
 * selected. The set of suggestions that were previously selected is
 * stored in suggestedWords.  This filters removes the words that are
 * contained in both @param v and suggestedWords.
 *
 */
void Selector::repetitionFilter( std::vector<std::string>& v )
{
    std::vector< std::string > temp;

    for( std::vector<std::string>::iterator i = v.begin();
	 i != v.end();
	 i++ ) {
	if( suggestedWords.find( *i ) == suggestedWords.end() ) {
	    temp.push_back( *i );
	    logger << DEBUG << "Token passed repetition filter: " << *i << endl;
	} else {
	    logger << DEBUG << "Token failed repetition filter: " << *i << endl;
	}
    }

    v = temp;
}

/** Filters out suggestions that could save fewer than THRESHOLD keystrokes.
 *
 * Assuming prefix.size() == n, suggestion.size() == m, and THRESHOLD
 * == t, then this filter removes those suggestions for which the
 * following condition is true: (m - n) < t
 *
 */
void Selector::thresholdFilter( std::vector<std::string>& v )
{
    assert( greedy_suggestion_threshold >= 0 );

    // zero threshold indicates feature is disabled
    if( greedy_suggestion_threshold != 0 ) {
		
	int length = contextTracker->getPrefix().size();
	std::vector<std::string>::iterator i = v.begin();
	while (i != v.end()) {
	    if( (i->size()-length) < greedy_suggestion_threshold) {
		logger << INFO << "Removing token: " << *i << endl;
		i = v.erase( i );
	    } else {
		i++;
	    }
	}
    }
}


/** Set LOGGER option.
 *
 */
void Selector::set_logger (const std::string& value)
{
    logger << setlevel (value);
    logger << INFO << "LOGGER: " << value << endl;
}


/** Set SUGGESTIONS option.
 *
 */
void Selector::set_suggestions(const std::string& value)
{
    logger << INFO << "SUGGESTIONS: " << value << endl;
    int result = Utility::toInt(value);
    if (result < 0) {
	logger << ERROR << "Presage.Selector.SUGGESTIONS value out of range!/a" << endl;
	// REVISIT: throw exception
	abort();
    }

    suggestions = result;
}


/** Set REPEAT_SUGGESTION option.
 *
 */
void Selector::set_repeat_suggestions(const std::string& value)
{
    logger << INFO << "REPEAT_SUGGESTIONS: " << value << endl;
    bool result = Utility::isYes(value);

    repeat_suggestions = result;
}


/** Set SUGGESTION_THRESHOLD option.
 *
 */
void Selector::set_greedy_suggestion_threshold(const std::string& value)
{
    logger << INFO << "GREEDY_SUGGESTION_THRESHOLD: " << value << endl;
    int result = Utility::toInt(value);
    if( result < 0 ) {
	logger << ERROR << "GREEDY_SUGGESTION_THRESHOLD value out of range." << value << endl;
	// REVISIT: throw exception
	abort();
    }

    greedy_suggestion_threshold = result;
}

size_t Selector::get_suggestions () const
{
    return suggestions;
}

bool Selector::get_repeat_suggestions () const
{
    return repeat_suggestions;
}

size_t Selector::get_greedy_suggestion_threshold () const
{
    return greedy_suggestion_threshold;
}

void Selector::update (const Observable* variable)
{
    logger << DEBUG << "update(" << variable->get_name () << ") called" << endl;

    dispatcher.dispatch (variable);
}
