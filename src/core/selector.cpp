
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


#include "selector.h"

Selector::Selector( HistoryTracker& ht )
    : historyTracker( ht )
{
    // set options to default values
    SUGGESTIONS = DEFAULT_SUGGESTIONS;
    REPEAT_SUGGESTION = DEFAULT_REPEAT_SUGGESTION;
    GREEDY_SUGGESTION_THRESHOLD = DEFAULT_GREEDY_SUGGESTION_THRESHOLD;

    // set prefix
    prefix = historyTracker.getPrefix();
}

Selector::~Selector()
{
    // nothing to do here, move along
}

std::vector<std::string> Selector::select( Prediction p )
{
    // copy words from Prediction.Suggestion.word in result vector
    std::vector<std::string> result;
    for( int i=0 ; i<p.getSize() ; i++ ) {
	result.push_back( p.getSuggestion( i ).getWord() );
    }
	
    // check whether user has not moved on to a new word
    if( contextChange( prefix, historyTracker.getPrefix() ) ) {
	clearSuggestedWords();
    }

    // store new prefix into prefix
    prefix = historyTracker.getPrefix();

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
		
	int length = prefix.size();
	std::vector<std::string>::iterator i = v.begin();
	while (i != v.end()) {
	    if( (i->size()-length) < GREEDY_SUGGESTION_THRESHOLD) {
		//std::cerr << "Removing token: " << *i << std::endl;
		v.erase( i );
	    } else {
		i++;
	    }
	}
    }
}


/** Returns true if a context change occured.
 *
 * A context change occurs when the word the system is trying to
 * predict changes to a new word. This can occur when:
 *
 * - the word was correctly predicted and a new word becomes the
 *   current word
 * - the word is changed by the user, by deleting its characters
 *
 * At the moment, a context change is detected by comparing the first
 * character of the old prefix with the first character of the new
 * prefix. If they match, a context change did not occur, therefore
 * function returns false.  If they do not match, a context change
 * occured and therefore true is returned.
 */
bool Selector::contextChange(const std::string& old,const std::string& nu) const
{
    if( old[0] != nu[0] ) {
	if( old == "" ) {
	    return false;
	} else {
	    return true;
	}
    } else {
	return false;
    }
}

/** Getter function, here for testing purposes.
 * 
 */
StringSet Selector::getSuggestedWords() const
{
    return suggestedWords;
}

/** Getter function, here for testing purposes.
 * 
 */
std::string Selector::getPrefix() const
{
    return prefix;
}


/** Set SUGGESTIONS option.
 *
 */
void Selector::setSuggestions( const int value )
{
    if( value > 0 )
	SUGGESTIONS = value;
    else
	std::cerr << "SUGGESTIONS option value " << value << " is out of range!/a" << std::endl;
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
void Selector::setRepeatSuggestion( const bool value )
{
    REPEAT_SUGGESTION = value;
}


/** Get REPEAT_SUGGESTION option.
 *
 */
bool Selector::getRepeatSuggestion() const
{
    return REPEAT_SUGGESTION;
}


/** Set SUGGESTION_THRESHOLD option.
 *
 */
void Selector::setGreedySuggestionThreshold( const unsigned int value )
{
    if( value >= 0 ) {
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
