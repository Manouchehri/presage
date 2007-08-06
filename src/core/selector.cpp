
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


#include "selector.h"
#include "utility.h"

#ifdef DEBUG
# define LOG(x) std::cout << x << std::endl
#else
# define LOG(x) /* x */
#endif


Selector::Selector(Profile* profile, ContextTracker* ct)
    : contextTracker(ct)
{
    // read config values
    Variable variable;
    variable.push_back("Soothsayer");
    variable.push_back("Selector");

    Value value;

    try {
	variable.push_back("SUGGESTIONS");
	value = profile->getConfig(variable);
	LOG("[Selector] SUGGESTIONS: " + value);
	setSuggestions(toInt(value));
	variable.pop_back();

	variable.push_back("REPEAT_SUGGESTIONS");
	value = profile->getConfig(variable);
	LOG("[Selector] REPEAT_SUGGESTIONS: " + value);
	setRepeatSuggestions(isYes(value));
	variable.pop_back();

	variable.push_back("GREEDY_SUGGESTION_THRESHOLD");
	value = profile->getConfig(variable);
	LOG("[Selector] GREEDY_SUGGESTION_THRESHOLD: " + value);
	setGreedySuggestionThreshold(toInt(value));
	variable.pop_back();
    } catch (Profile::ProfileException ex) {
	std::cerr << "[Selector] Caught ProfileException: " << ex.what() << std::endl;
    }

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
    for( int i=0 ; i<p.size() ; i++ ) {
	result.push_back( p.getSuggestion( i ).getWord() );
    }
	
    // check whether user has not moved on to a new word
    if (contextTracker->contextChange()) {
	clearSuggestedWords();
    }

    // filter out suggestions that do not satisfy repetition constraint
    if( !REPEAT_SUGGESTION )
	repetitionFilter( result );

    // filter out suggestions that do not satisfy threshold constraint
    if( GREEDY_SUGGESTION_THRESHOLD > 0 )
	thresholdFilter( result );

    // build result
	
    // check that we have enough selected words
    if( result.size() < static_cast<unsigned int>(SUGGESTIONS) ) {
	// Job's not done, got to get a bigger Prediction
	// we should invoke predict() to get more Suggestions

	// could throw an exception that would be caught by predictor
	// which would reissue the predict call to get more
	// suggestions
			
	// TODO <============================================
			
	// just abort for now
	//std::cerr << "Not enough Suggestions" << std::endl;
	//abort();
		
    } else {
	// erase the requested number of words
	result.erase( result.begin() + SUGGESTIONS, result.end() );

	// update suggested words set
	updateSuggestedWords( result );
    }

    return result;
}


/** Adds suggestions to the set of previously selected suggestions.
 *
 */
void Selector::updateSuggestedWords( const std::vector<std::string>& v )
{
    std::vector<std::string>::const_iterator i = v.begin();
    while( i != v.end() ) {
	suggestedWords.insert( *i );
	i++;
    }
}


/** Clear the set of previously selected suggestions.
 *
 */
void Selector::clearSuggestedWords()
{
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
    assert( GREEDY_SUGGESTION_THRESHOLD >= 0 );

    // zero threshold indicates feature is disabled
    if( GREEDY_SUGGESTION_THRESHOLD != 0 ) {
		
	int length = contextTracker->getPrefix().size();
	std::vector<std::string>::iterator i = v.begin();
	while (i != v.end()) {
	    if( (i->size()-length) < GREEDY_SUGGESTION_THRESHOLD) {
		LOG("[Selector] Removing token: " + *i);
		v.erase( i );
	    } else {
		i++;
	    }
	}
    }
}

/** Set SUGGESTIONS option.
 *
 */
void Selector::setSuggestions( const int value )
{
    if( value > 0 ) {
	LOG("[Selector] Setting SUGGESTIONS to " << value);
	SUGGESTIONS = value;
    } else {
	std::cerr << "[Selector] SUGGESTIONS option not set. Value " << value << " out of range!/a" << std::endl;
    }
}


/** Get SUGGESTIONS option.
 *
 */
int Selector::getSuggestions() const
{
    return SUGGESTIONS;
}


/** Set REPEAT_SUGGESTION option.
 *
 */
void Selector::setRepeatSuggestions( const bool value )
{
    LOG("[Selector] Setting REPEAT_SUGGESTION: " << value);
    REPEAT_SUGGESTION = value;
}


/** Get REPEAT_SUGGESTION option.
 *
 */
bool Selector::getRepeatSuggestions() const
{
    return REPEAT_SUGGESTION;
}


/** Set SUGGESTION_THRESHOLD option.
 *
 */
void Selector::setGreedySuggestionThreshold( const unsigned int value )
{
    if( value >= 0 ) {
	LOG("[Selector] Setting GREEDY_SUGGESTION_THRESHOLD: " << value);
	GREEDY_SUGGESTION_THRESHOLD = value;
    }
}


/** Get SUGGESTION_THRESHOLD option.
 *
 */
unsigned int Selector::getGreedySuggestionThreshold() const
{
    return GREEDY_SUGGESTION_THRESHOLD;
}
