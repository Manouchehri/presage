
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


#ifndef PRESAGE_CONTEXTTRACKER
#define PRESAGE_CONTEXTTRACKER

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>

#include "contextChangeDetector.h"

#include "../tokenizer/reverseTokenizer.h"
#include "../charsets.h"
#include "../configuration.h"
#include "../logger.h"
#include "../dispatcher.h"

#include "../../presageCallback.h"

class PredictorRegistry;

/** \brief Tracks user interaction and context.
 *
 * ContextTracker job is to track the history of user input and
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
class ContextTracker : public Observer {
public:
    ContextTracker(Configuration* config,
		   PredictorRegistry* predictorRegistry,
		   PresageCallback* callback,
		   const char[]=DEFAULT_WORD_CHARS,
                   const char[]=DEFAULT_SEPARATOR_CHARS,
                   const char[]=DEFAULT_BLANKSPACE_CHARS,
                   const char[]=DEFAULT_CONTROL_CHARS );
    ~ContextTracker();

    const PresageCallback* callback(const PresageCallback* callback);

    bool contextChange();

    std::string getPrefix() const;
    std::string getToken (const int) const;

    std::string getExtraTokenToLearn(const int index,
				     const std::vector<std::string>& change) const;

    std::string getFutureStream() const;
    std::string getPastStream  () const;

    bool isCompletionValid(const std::string&) const;

    //eliminate after testing phase
    std::string getWordChars()       const;
    std::string getSeparatorChars()  const;
    std::string getBlankspaceChars() const;
    std::string getControlChars()    const;

    std::string toString() const;

    void update();

    /** \brief Learn from text
     *
     * Train the predictors on \param text. Each active predictor will
     * apply their machine learning algorithm to learn from the \param
     * text.
     *
     */
    void learn(const std::string& text) const;

    virtual void update (const Observable* variable);

    void set_logger (const std::string& value);
    void set_sliding_window_size (const std::string& value);
    void set_lowercase_mode (const std::string& value);
    void set_online_learning (const std::string& value);

    static const char* LOGGER;
    static const char* SLIDING_WINDOW_SIZE;
    static const char* LOWERCASE_MODE;
    static const char* ONLINE_LEARNING;

private:
    std::string wordChars;
    std::string separatorChars;
    std::string blankspaceChars;
    std::string controlChars;

    bool lowercase_mode;
    bool online_learning;

    // REVISIT: this was removed since a tokenizer is created with
    // each method invocation that needs it
    //ReverseTokenizer tokenizer;

    // utility functions
    bool isWordChar      (const char) const;
    bool isSeparatorChar (const char) const;
    bool isControlChar   (const char) const;
    bool isBlankspaceChar(const char) const;

    const PresageCallback* context_tracker_callback;
    PredictorRegistry* predictorRegistry;
    ContextChangeDetector* contextChangeDetector;
    Logger<char> logger;
    
    Dispatcher<ContextTracker> dispatcher;
};

#endif // PRESAGE_CONTEXTTRACKER
