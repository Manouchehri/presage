
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


#ifndef SOOTH_HISTORYTRACKER
#define SOOTH_HISTORYTRACKER

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>

#include "core/tokenizer/reverseTokenizer.h"
#include "core/charsets.h"

// Default option values
const int DEFAULT_MAX_BUFFER_SIZE = 1024;  // TODO: <==== not implemented yet


/** HistoryTracker tracks user's interaction and context
 *
 * HistoryTracker job is to track the history of user's input and
 * track context changes.
 *
 * History is the sequence of characters input by the user. Context is
 * defined by the token the system is attempting to predict. Context
 * changes occur when certain characters are input.
 *
 * Tracking user input history and context changes can be trickier
 * than one would think. To ease this operation, characters are
 * classified into these categories:
 *
 * word characters
 * blankspace characters
 * separator characters
 * control characters
 *
 * Word characters become part of the current prefix. Word characters
 * do not trigger a change in context.
 *
 * Blankspace and separator characters trigger a context change and
 * mark the end of the current prediction token.
 *
 * Control characters are non-printable characters such up, down,
 * left, right arrows and function keys.
 *
 *
 * Use-cases:
 *
 * 
 * Full sentence: Yesterday I went to the market to buy an apple.
 * 
 * History:Yesterday I went to the|
 * wi-2	"went"
 * wi-1	"to"
 * prefix	""
 * postfix ""
 * 
 * 
 * : 'SPC' (space)
 * History:Yesterday I went to the|
 * wi-2	"to"
 * wi-1	"the"
 * prefix	""
 * postfix ""
 * 
 * : 'm'
 * History:Yesterday I went to the m|
 * wi-2	"to"
 * wi-1	"the"
 * prefix	"m"
 * postfix ""
 * 
 * : 'a'
 * History:Yesterday I went to the ma|
 * wi-2	"to"
 * wi-1	"the"
 * prefix	"ma"
 * postfix ""
 * 
 * : 'rket'
 * History:Yesterday I went to the market|
 * wi-2	"to"
 * wi-1	"the"
 * prefix	"market"
 * postfix ""
 * 
 * : 'SPC'
 * History:Yesterday I went to the market |
 * wi-2	"the"
 * wi-1	"market"
 * prefix	""
 * postfix ""
 * 
 * : 't'
 * History:Yesterday I went to the market t|
 * wi-2	"the"
 * wi-1	"market"
 * prefix	"t"
 * postfix ""
 * 
 * : 'LEFT_ARROW'
 * History:Yesterday I went to the market |t
 * wi-2	"the"
 * wi-1	"market"
 * prefix	""
 * postfix ""
 * 
 * : 'LEFT_ARROW'
 * History:Yesterday I went to the market| t
 * wi-2	"to"
 * wi-1	"the"
 * prefix	"market"
 * postfix ""
 * 
 * : 's'
 * History:Yesterday I went to the markets| t
 * wi-2	"to"
 * wi-1	"the"
 * prefix	"markets"
 * postfix ""
 *
 * 
 */
class HistoryTracker {
public:
    HistoryTracker(const char[]=DEFAULT_WORD_CHARS,
                   const char[]=DEFAULT_SEPARATOR_CHARS,
                   const char[]=DEFAULT_BLANKSPACE_CHARS,
                   const char[]=DEFAULT_CONTROL_CHARS );
    ~HistoryTracker();

    void update(std::string);

    std::string getPrefix();
    std::string getToken (const int);
    //vector<string> getTokens(const int,const int) const;
	
    std::string getFutureStream() const;
    std::string getPastStream  () const;

    //eliminate after testing phase
    std::string getWordChars()       const;
    std::string getSeparatorChars()  const;
    std::string getBlankspaceChars() const;
    std::string getControlChars()    const;

    std::string toString() const;

    /** REVISIT: is this method really needed?
     */
    int  getMaxBufferSize() const;
    /** REVISIT: is this method really needed?
     */ 
    void setMaxBufferSize(const int);

private:

    std::stringstream pastStream;
    std::stringstream futureStream;

    std::string wordChars;
    std::string separatorChars;
    std::string blankspaceChars;
    std::string controlChars;

    // REVISIT: this was removed since a tokenizer is created with
    // each method invocation that needs it
    //ReverseTokenizer tokenizer;

    /** REVISIT: is this really needed?
     */
    int MAX_BUFFER_SIZE;

    // utility functions
    bool isWordChar      (const char) const;
    bool isSeparatorChar (const char) const;
    bool isControlChar   (const char) const;
    bool isBlankspaceChar(const char) const;

};

/*
 * Punctuation characters: should they be treated as separators or
 * blankspaces? Should they be considered as first-class tokens?
 * Certainly syntactic predictive plugins would benefit from having
 * punctuation as tokens.
 *
 */


#endif // SOOTH_HISTORYTRACKER
