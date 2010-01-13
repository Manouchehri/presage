
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
#include "core/utility.h"
#include "core/pluginRegistry.h"
#include "core/tokenizer/forwardTokenizer.h"

#include <stdlib.h>  // for atoi()

ContextTracker::ContextTracker(Configuration* config,
			       PluginRegistry* registry,
			       PresageCallback* callback,
			       const char wChars[],
			       const char tChars[],
			       const char bChars[],
			       const char cChars[])
    : wordChars      (wChars),
      separatorChars (tChars),
      blankspaceChars(bChars),
      controlChars   (cChars),
      pluginRegistry (registry),
      logger         ("ContextTracker", std::cerr)
      //tokenizer      (pastStream, blankspaceChars, separatorChars)
{
    if (callback) {
	context_tracker_callback = callback;
    } else {
	throw new PresageException("Invalid callback object");
    }

    contextChangeDetector = new ContextChangeDetector(wordChars,
						      separatorChars,
						      blankspaceChars,
						      controlChars);

    // set pointer to this context tracker in plugin registry so that
    // plugins can be constructed when next iterator is requested
    //
    if (pluginRegistry) {
	pluginRegistry->setContextTracker(this);
    }

    // read config values
    Variable* variable;
    Value value;

    try {
	variable = new Variable("Presage.ContextTracker.LOGGER");
	value = config->get(*variable);
	logger << setlevel(value);
	logger << INFO << "LOGGER: " << value << endl;
	delete variable;

	variable = new Variable("Presage.ContextTracker.SLIDING_WINDOW_SIZE");
	value = config->get(*variable);
	logger << INFO << "SLIDING_WINDOWS_SIZE: " << value << endl;
	contextChangeDetector->set_sliding_window_size(value);
	delete variable;

    } catch (Configuration::ConfigurationException ex) {
	logger << ERROR << "Caught ConfigurationException: " << ex.what() << endl;
    }

}

ContextTracker::~ContextTracker()
{
    delete contextChangeDetector;
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
    std::stringstream change;

    // prepend partially entered token to change if exists, need to
    // look into sliding_window to get previously partially entered
    // token if it exists
    std::stringstream sliding_window_stream;
    sliding_window_stream << contextChangeDetector->get_sliding_window();
    ReverseTokenizer rTok(sliding_window_stream,
			  blankspaceChars,
			  separatorChars);
    std::string first_token = rTok.nextToken();
    if (!first_token.empty()) {
	change << first_token;
    }

    logger << DEBUG << "update(): getPastStream(): " << getPastStream() << endl;

    // append change detected by context change detector
    change << contextChangeDetector->change(getPastStream());

    logger << INFO << "update(): change: " << change.str() << endl;

    // split change up into tokens
    std::vector<std::string> change_tokens;
    ForwardTokenizer tok(change,
			 blankspaceChars,
			 separatorChars);
    logger << INFO << "update(): tokenized change: ";
    while (tok.hasMoreTokens()) {
	std::string token = tok.nextToken();
	change_tokens.push_back(token);
	logger << INFO << token << ':';
    }
    logger << INFO << endl;

    if (! change_tokens.empty()) {
	// remove prefix (partially entered token or empty token)
	change_tokens.pop_back();
    }

    logger << INFO << "update(): change tokens: ";
    for (std::vector<std::string>::const_iterator it = change_tokens.begin();
	 it != change_tokens.end();
	 it++) {
	logger << INFO << *it << ':';
    }
    logger << INFO << endl;

    // time to learn
    PluginRegistry::Iterator it = pluginRegistry->iterator();
    Plugin* plugin = 0;

    while (it.hasNext()) {
	plugin = it.next();
	plugin->learn(change_tokens);
    }

    // update sliding window
    contextChangeDetector->update_sliding_window(getPastStream());
}

std::string ContextTracker::getPrefix() const
{
    return getToken(0);
}

std::string ContextTracker::getToken(const int index) const
{
    std::stringstream pastStringStream(context_tracker_callback->get_past_stream());
    ReverseTokenizer tokenizer(pastStringStream, blankspaceChars, separatorChars);

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

std::string ContextTracker::getSlidingWindowToken(const int index) const
{
    std::stringstream slidingWindowStream(contextChangeDetector->get_sliding_window());
    ReverseTokenizer tokenizer(slidingWindowStream, blankspaceChars, separatorChars);

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
    prefix = strtolower(prefix);  // no need to be case sensitive
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
