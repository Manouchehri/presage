
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


#include "contextChangeDetector.h"
#include "core/tokenizer/reverseTokenizer.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>  // for atoi()
#include <assert.h>

const std::string::size_type ContextChangeDetector::DEFAULT_SLIDING_WINDOW_SIZE = 80;

ContextChangeDetector::ContextChangeDetector(const std::string wChars,
					     const std::string tChars,
					     const std::string bChars,
					     const std::string cChars)
    : wordChars      (wChars),
      separatorChars (tChars),
      blankspaceChars(bChars),
      controlChars   (cChars)
{
    //std::cerr << "ContextChangeDetector::ContextChangeDetector()" << std::endl
    //          << "wordChars: " << wordChars << std::endl;
}

ContextChangeDetector::~ContextChangeDetector()
{

}

void ContextChangeDetector::set_sliding_window_size(const std::string& str)
{
    if(!str.empty()) {
        SLIDING_WINDOW_SIZE = atoi(str.c_str());
    } else {
	SLIDING_WINDOW_SIZE = DEFAULT_SLIDING_WINDOW_SIZE;
    }
}

void ContextChangeDetector::update_sliding_window(const std::string& str)
{
    if (str.size() <= SLIDING_WINDOW_SIZE) {
	// past stream fits in sliding window
	sliding_window = str;
    } else {
	// trim past stream down to sliding window
	sliding_window = str.substr(str.size() - SLIDING_WINDOW_SIZE);
	assert(sliding_window.size() == SLIDING_WINDOW_SIZE);
    }
}

bool ContextChangeDetector::context_change(const std::string& past_stream) const
{
    // Here's how this is going to be implemented...  We'll keep a
    // sliding window on the last few chars seen by presage; the
    // buffer holding them is the sliding window. We'll search for the
    // last occurence of sliding_window in past_stream, if any.

    // If sliding_window is not found in past_stream, then it is not
    // possible to relate the current context to the previously seen
    // context, hence we assume a context change has occured.

    // If sliding_window is found, then we need to examine the chars
    // following the sliding window in the past stream. We call this
    // the remainder. If there are any non-word chars in the
    // remainder, then a context change has occurred. Else, no context
    // change occured.

    // The sliding window is never implicitly updated as part of
    // invoking this method.

    return context_change_helper(sliding_window, past_stream);
}


bool ContextChangeDetector::context_change_helper(const std::string& prev_context, const std::string& curr_context) const
{
    bool result = false;
    
    if (prev_context.empty()) {
	if (curr_context.empty()) {
	    // both contexts are empty, nothing has happened, no
	    // change happened
	    result = false;
	} else {
	    // current context changed, previous context is empty,
	    // first change happened
	    result = true;  // REVISIT: this should really be true,
			    // but setting it to true screws up
			    // learning
	}
    } else {
	// find position of previous context in current context
	// i.e. find index pointing to last char of last occurence of
	// prev_context in curr_context
	std::string::size_type ctx_idx = curr_context.rfind(prev_context);
	
	if (ctx_idx == std::string::npos) {
	    // prev_context could not be found in curr_context, a lot
	    // changed
	    result = true;
	} else {
	    // found prev_context, examine remainder string.
	    // remainder string is substr(ctx_idx +
	    // prev_context.size()); i.e. substring given by index
	    // returned by rfind (which points at beginning of
	    // prev_context string found in curr_context) plus size of
	    // prev_context: this index points at end of prev_context
	    // substring found in curr_context

	    std::string remainder = curr_context.substr(ctx_idx + prev_context.size());

	    std::string::size_type idx = remainder.find_last_of(wordChars);
	    if (idx == std::string::npos) {
		if (remainder.empty()) {
		    result = false;
		} else {
		    char last_char = curr_context[ctx_idx + prev_context.size() - 1];
		    idx = wordChars.find(last_char);
		    if (idx == std::string::npos) {
			result = false;
		    } else {
			result = true;
		    }
		}
	    } else {
		if (idx == remainder.size() - 1) {
		    result = false;
		} else {
		    result = true;
		}
	    }


/*
 * alternate implementation of the logic in the enclosing else
 * block. This uses tokenizers, which is not desirable as it makes
 * tokenizer a dependency of context change detector.

	    std::string remainder = curr_context.substr(loc + prev_context.size());

	    std::stringstream curr_strstream(curr_context);
	    std::stringstream prev_strstream(prev_context);

	    ReverseTokenizer curr_tokenizer(curr_strstream, blankspaceChars, separatorChars);
	    ReverseTokenizer prev_tokenizer(prev_strstream, blankspaceChars, separatorChars);

	    std::string prev_token = prev_tokenizer.nextToken();
	    std::string curr_token = curr_tokenizer.nextToken();

	    if (curr_token.empty()) {
		if (prev_token.empty()) {
		    result = false;
		    
		    loc = curr_context.find_first_of(wordChars, loc);
		    if (loc == std::string::npos) {
			result = false;
		    } else {
			result = true;
		    }

		} else {
		    result = true;
		}

	    } else {
		loc = curr_token.find(prev_token);
		if (loc == std::string::npos) {
		    result = true;
		} else {
		    result = false;
		}
	    }
*/

	}
    }

    return result;
}

std::string ContextChangeDetector::change(const std::string& past_stream) const
{
    const std::string& prev_context = sliding_window;  // let's rename these
    const std::string& curr_context = past_stream;     // for clarity's sake

    std::string result;

    if (sliding_window.empty()) {
	result = past_stream;
    } else {
	// find position of previous context in current context
	// i.e. find index pointing to last char of last occurence of
	// prev_context in curr_context
	std::string::size_type ctx_idx = curr_context.rfind(prev_context);
	
	if (ctx_idx == std::string::npos) {
	    // prev_context could not be found in curr_context, a lot
	    // changed
	    result = past_stream;
	} else {
	    // found prev_context, examine remainder string.
	    // remainder string is substr(ctx_idx +
	    // prev_context.size()); i.e. substring given by index
	    // returned by rfind (which points at beginning of
	    // prev_context string found in curr_context) plus size of
	    // prev_context: this index points at end of prev_context
	    // substring found in curr_context

	    result = curr_context.substr(ctx_idx + prev_context.size());
	}
    }

    return result;
}

std::string ContextChangeDetector::get_sliding_window() const
{
    return sliding_window;
}
