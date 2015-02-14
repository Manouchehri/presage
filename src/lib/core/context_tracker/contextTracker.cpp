
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


#include "contextTracker.h"
#include "../utility.h"
#include "../predictorRegistry.h"
#include "../tokenizer/forwardTokenizer.h"

#include <stdlib.h>  // for atoi()

const char* ContextTracker::LOGGER = "Presage.ContextTracker.LOGGER";
const char* ContextTracker::SLIDING_WINDOW_SIZE = "Presage.ContextTracker.SLIDING_WINDOW_SIZE";
const char* ContextTracker::LOWERCASE_MODE = "Presage.ContextTracker.LOWERCASE_MODE";
const char* ContextTracker::ONLINE_LEARNING = "Presage.ContextTracker.ONLINE_LEARNING";

ContextTracker::ContextTracker(Configuration* config,
			       PredictorRegistry* registry,
			       PresageCallback* callback,
			       const char wChars[],
			       const char tChars[],
			       const char bChars[],
			       const char cChars[])
    : wordChars      (wChars),
      separatorChars (tChars),
      blankspaceChars(bChars),
      controlChars   (cChars),
      predictorRegistry (registry),
      logger         ("ContextTracker", std::cerr),
      //tokenizer      (pastStream, blankspaceChars, separatorChars),
      lowercase_mode (true),
      dispatcher     (this)
{
    if (callback) {
	context_tracker_callback = callback;
    } else {
	throw new PresageException(PRESAGE_INVALID_CALLBACK_ERROR, "Invalid callback object");
    }

    contextChangeDetector = new ContextChangeDetector(wordChars,
						      separatorChars,
						      blankspaceChars,
						      controlChars,
						      lowercase_mode);

    // set pointer to this context tracker in predictor registry so that
    // predictors can be constructed when next iterator is requested
    //
    if (predictorRegistry) {
	predictorRegistry->setContextTracker(this);
    }

    // build dispatch map
    dispatcher.map (config->find (LOGGER), & ContextTracker::set_logger);
    dispatcher.map (config->find (SLIDING_WINDOW_SIZE), & ContextTracker::set_sliding_window_size);
    dispatcher.map (config->find (LOWERCASE_MODE), & ContextTracker::set_lowercase_mode);
    dispatcher.map (config->find (ONLINE_LEARNING), & ContextTracker::set_online_learning);
}

ContextTracker::~ContextTracker()
{
    delete contextChangeDetector;
}

void ContextTracker::set_logger (const std::string& value)
{
    logger << setlevel (value);
    logger << INFO << "LOGGER: " << value << endl;
}

void ContextTracker::set_sliding_window_size (const std::string& value)
{
    contextChangeDetector->set_sliding_window_size (value);
    logger << INFO << "SLIDING_WINDOWS_SIZE: " << value << endl;
}

void ContextTracker::set_lowercase_mode (const std::string& value)
{
    lowercase_mode = Utility::isYes(value);
    logger << INFO << "LOWERCASE_MODE: " << value << endl;
}

void ContextTracker::set_online_learning(const std::string& value)
{
    online_learning = Utility::isYes(value);
    logger << INFO << "ONLINE_LEARNING: " << value << endl;
}

const PresageCallback* ContextTracker::callback(const PresageCallback* new_callback)
{
    const PresageCallback* result = context_tracker_callback;
    if (new_callback) {
	context_tracker_callback = new_callback;
    }
    return result;
}

/** Returns true if a context change occured.
 *
 */
bool ContextTracker::contextChange()
{
    return contextChangeDetector->context_change(getPastStream());
}

void ContextTracker::update()
{
    // detect change that needs to be learned
    std::string change = contextChangeDetector->change(getPastStream());

    if (online_learning)
    {
	learn (change);
    }

    // update sliding window
    contextChangeDetector->update_sliding_window(getPastStream());
}

void ContextTracker::learn(const std::string& text) const
{
    logger << INFO << "learn(): text: " << text << endl;

    std::stringstream stream_to_learn(text);

    // split stream up into tokens
    std::vector<std::string> tokens;
    ForwardTokenizer tok(stream_to_learn,
			 blankspaceChars,
			 separatorChars);
    tok.lowercaseMode(lowercase_mode);
    logger << INFO << "learn(): tokenized change: ";
    while (tok.hasMoreTokens()) {
	std::string token = tok.nextToken();
	tokens.push_back(token);
	logger << INFO << token << '|';
    }
    logger << INFO << endl;

    if (! tokens.empty()) {
	// remove prefix (partially entered token or empty token)
	tokens.pop_back();
    }

    if ((logger << INFO).shouldLog())
    {
	logger << "learn(): sanitized change: ";
	for (std::vector<std::string>::const_iterator it = tokens.begin();
	     it != tokens.end();
	     it++) {
	    logger << *it << '|';
	}
	logger << endl;
    }

    // time to learn
    PredictorRegistry::Iterator it = predictorRegistry->iterator();
    Predictor* predictor = 0;

    while (it.hasNext()) {
	predictor = it.next();
	predictor->learn(tokens);
    }
}

std::string ContextTracker::getPrefix() const
{
    return getToken(0);
}

std::string ContextTracker::getToken(const int index) const
{
    std::stringstream pastStringStream(context_tracker_callback->get_past_stream());
    ReverseTokenizer tokenizer(pastStringStream, blankspaceChars, separatorChars);
    tokenizer.lowercaseMode(lowercase_mode);

    std::string token;
    int i = 0;
    while (tokenizer.hasMoreTokens() && i <= index) {
        token = tokenizer.nextToken();
        i++;
    }
    if (i <= index) {
	// in case the index points too far back
	token = "";
    }
    return token;

//// Using ForwardTokenizer
//    "a b c"
//     2 1 0
//     0 1 2
//     1 2 3
//
//    ForwardTokenizer tokenizer(pastStream, blankspaceChars, separatorChars);
//    tokenizer.lowercaseMode(lowercase_mode);
//    std::string result;
//    int tokens = tokenizer.countTokens();
//    // why oh why is this clear() required to get it to work???
//    pastStream.clear();
//    int j = 0;
//    while (tokenizer.hasMoreTokens() && j < tokens - index) {
//	result = tokenizer.nextToken();
//	j++;
//
//	std::cerr << "ContextTracker::getToken() current token: " << result << std::endl;
//    }
//    return result;
}

std::string ContextTracker::getExtraTokenToLearn(const int index, const std::vector<std::string>& change) const
{
    //logger << DEBUG
    //	   << "past_stream   : " << getPastStream() << endl
    //	   << "change        : " << contextChangeDetector->change(getPastStream()) << endl
    //	   << "sliding_window: " << contextChangeDetector->get_sliding_window() + "\n" << endl;


    // Extra tokens to learn are to be found in (past_stream - change)
    //
    // The change tokens are tokens that have not been seen or learnt
    // before.
    //
    // The extra tokens to learn are tokens that have been seen and
    // learn before, but that we need to reuse to fill out the n-gram
    // of required cardinality that we are about to learn.
    //
    // To find the extra tokens to learn, we use the size of tokenized
    // change vector to offset the index and extract the extra tokens
    // to learn from the past stream.
    //
    // For example:
    //   past_stream : "The quick brown fox jumped over the "
    //   change      : |over|the|
    //   extra_tokens: |The|quick|brown|fox|jumped|
    //
    return getToken(index + change.size());
}

std::string ContextTracker::getFutureStream() const
{
    return context_tracker_callback->get_future_stream();
}

std::string ContextTracker::getPastStream() const
{
    std::string result = context_tracker_callback->get_past_stream();
    return result;
}

bool ContextTracker::isCompletionValid(const std::string& completion) const
{
    bool result = false;

    std::string prefix = getPrefix();
    prefix = Utility::strtolower(prefix);  // no need to be case sensitive
    if (completion.find(prefix) == 0) {
        result = true;
    }

    return result;
}

bool ContextTracker::isWordChar(const char c) const
{
    if(wordChars.find(c, 0) != std::string::npos)
        return true;
    else
        return false;
}

bool ContextTracker::isSeparatorChar(const char c) const
{
    if(separatorChars.find(c, 0) != std::string::npos)
        return true;
    else
        return false;
}

bool ContextTracker::isBlankspaceChar(const char c) const
{
    if(blankspaceChars.find(c, 0) != std::string::npos)
        return true;
    else
        return false;
}

bool ContextTracker::isControlChar(const char c) const
{
    if(controlChars.find(c, 0) != std::string::npos)
        return true;
    else
        return false;
}

std::string ContextTracker::getWordChars() const
{
    return wordChars;
}

std::string ContextTracker::getSeparatorChars() const
{
    return separatorChars;
}

std::string ContextTracker::getBlankspaceChars() const
{
    return blankspaceChars;
}

std::string ContextTracker::getControlChars() const
{
    return controlChars;
}

std::string ContextTracker::toString() const
{
    return context_tracker_callback->get_past_stream() + "<|>" + context_tracker_callback->get_future_stream() + "\n";
}

void ContextTracker::update (const Observable* variable)
{
    logger << DEBUG << "Notification received: "
	   << variable->get_name () << " - " << variable->get_value () << endl;

    dispatcher.dispatch (variable);
}
