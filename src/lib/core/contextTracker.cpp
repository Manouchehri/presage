
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
#include "utility.h"
#include "pluginRegistry.h"

ContextTracker::ContextTracker(Configuration* config,
			       PluginRegistry* registry,
			       const char wChars[],
			       const char tChars[],
			       const char bChars[],
			       const char cChars[])
    : wordChars      (wChars),
      separatorChars (tChars),
      blankspaceChars(bChars),
      controlChars   (cChars),
      pluginRegistry (registry),
      logger("ContextTracker", std::cerr)
    //tokenizer      (pastStream, blankspaceChars, separatorChars)
{
    //pastStream.clear();
    //pastStream.seekg(0, std::ios::beg);
    //pastStream.seekp(0, std::ios::end);
#ifdef USE_STRINGSTREAM
    assert(pastStream.good());
#endif

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

	variable = new Variable("Presage.ContextTracker.MAX_BUFFER_SIZE");
	value = config->get(*variable);
	logger << INFO << "MAX_BUFFER_SIZE: " << value << endl;
	setMaxBufferSize(toInt(value));
	delete variable;

    } catch (Configuration::ConfigurationException ex) {
	logger << ERROR << "Caught ConfigurationException: " << ex.what() << endl;
    }

    contextChanged = true;
}

ContextTracker::~ContextTracker()
{}

void ContextTracker::update(std::string s)
{
    // process each char in string s individually
    for (unsigned int i=0; i<s.size(); i++) {
	update(s[i]);

	logger << DEBUG << "update: contextTracker-getPrefix():" << getPrefix() << endl;
	logger << DEBUG << "update: contextTracker-getToken(1):" << getToken(1) << endl;
	logger << DEBUG << "update: previous_prefix: " << previous_prefix << endl;

	if (update_context_change()) {
	    PluginRegistry::Iterator it = pluginRegistry->iterator();
	    Plugin* plugin = 0;

	    while (it.hasNext()) {
		plugin = it.next();
		plugin->learn();
	    }
	}
    }
}

bool ContextTracker::update_context_change()
{
    contextChanged = true;

    if (!getPrefix().empty()) {
	logger << DEBUG << "Prefix not empty" << endl;
	// if current prefix is not null
	std::string::size_type loc = getPrefix().find(previous_prefix, 0);
	if (loc == 0) {
	    // if current prefix does not contain the previous prefix
	    // at the beginning of the string, the context has not
	    // changed
	    //
	    logger << DEBUG << "Found prefix in getPrefix()" << endl;
	    contextChanged = false;
	}

// TODO: if this code block is uncommented, context changes do not
// occur when separator or blankspace chars are entered. However, this
// code intriduces a bug that can be reproduced by entering the
// following text blocks 'foo ' 'bar ' 'foo' 'bar '. The last entered
// block should cause a context change, but it does not.
//

    } else {
	// if prefix is empty
	logger << DEBUG << "Prefix not empty" << endl;
	if (previous_prefix != getToken(1)) {
	    logger << DEBUG << "Reversing context change" << endl;
	    contextChanged = false;
	}
    }

    previous_prefix = getPrefix();

    logger << DEBUG << "update_context_change(): previous_prefix: " << previous_prefix << endl;
    logger << DEBUG << "update_context_change(): context changed: " << contextChanged  << endl;

    return contextChanged;
}

void ContextTracker::update(unsigned int character)
{
    //DEBUG
    //std::cout << "s[i] = " << static_cast<int>(s[i]) << std::endl;
    //std::cout << "s[i] " << (isControlChar( s[i] )?"":"non")
    //          << " e' un carattere di controllo." << std::endl;

    if(isWordChar(character)
       || isSeparatorChar(character)
       || isBlankspaceChar(character)) {
	// if s is a word string, append to pastBuffer
	// if s is a separator string, append to pastBuffer
	logger << DEBUG << "updating wordChar/separatorChar/blankspaceChar: " << character << endl;
#ifdef USE_STRINGSTREAM
	assert(pastStream.good());
	pastStream.put(character);
	assert(pastStream.good());
#else
	pastStream.push_back(character);
#endif
	//std::cerr << pastStream.str() << std::endl;
	//std::cerr << "ContextTracker::update() tokenizer.streamToString() " << tokenizer.streamToString() << std::endl;
    } else if ( isControlChar(character) ) {
	//if s is a control string, take the appropriate action
	logger << DEBUG << "updating controlChar: " << character << endl;

// REVISIT ////

// Temporarily disabling control chars

//            //UP_ARROW
//            // up_arrow key press messes things up! There's
//            // currently no way of knowing where the cursors ends
//            // up, so we're better off clearing our history up and
//            // hoping that doesn't modify an existing word but
//            // starts a new one.
//            if ( character == UP_ARROW ) {
//
//                pastBuffer.clear();
//                futureBuffer.clear();
//
//            }
//
//            //LEFT_ARROW
//            // left_arrow key press causes last char of pastBuffer
//            // to be erased from pastBuffer and inserted at the
//            // beginning of futureBuffer
//            if ( character == LEFT_ARROW && !pastBuffer.empty() ) {
//				
//                char p = pastBuffer[pastBuffer.size()-1];
//                futureBuffer.insert( futureBuffer.begin(), p );
//                pastBuffer.erase( pastBuffer.end()-1 );
//				
//            }
//
//            //DOWN_ARROW
//            // see up_arrow
//            if ( character == DOWN_ARROW ) {
//
//                pastBuffer.clear();
//                futureBuffer.clear();
//
//            }			
//
//            //RIGHT_ARROW
//            // right_arrow key press causes first char of
//            // futureBuffer to be erased from futureBuffer and 
//            // inserted at the end of pastBuffer
//            if ( character == RIGHT_ARROW && !futureBuffer.empty() ) {
//
//                char p = futureBuffer[0];
//                pastBuffer.insert( pastBuffer.end(), p );
//                futureBuffer.erase( futureBuffer.begin() );
//
//            }
//			
//            //END
//            // end key press causes string futureBuffer to be 
//            // appended to pastBuffer and to be cleared
//            if ( character == END && !futureBuffer.empty() ) {
//				
//                pastBuffer.append( futureBuffer );
//                futureBuffer.clear();
//
//            }
//
//            //HOME
//            // see up_arrow
//            if ( character == HOME ) {
//
//                pastBuffer.clear();
//                futureBuffer.clear();
//
//            }
//
	//BACKSPACE
	// backspace key press causes the last character of
	// pastBuffer to be erased
	if (character == BACKSPACE &&
#ifdef USE_STRINGSTREAM
	    !pastStream.str().empty()
#else
	    !pastStream.empty()
#endif
	    ) {
	    // WARNING: this is bound to be very inefficient!
	    // TODO: provide a better implementation.
	    //
#ifdef USE_STRINGSTREAM
	    logger << DEBUG << "pastStream before: " << pastStream.str() << endl;
	    std::string temp = pastStream.str();
	    temp.erase(temp.end() - 1);
	    pastStream.str(temp);
	    pastStream.seekg(0, std::ios::end);
	    logger << DEBUG << "pastStream after : " << pastStream.str() << endl;
#else
	    logger << DEBUG << "pastStream before: " << pastStream << endl;
	    pastStream.erase(pastStream.end() - 1);
	    logger << DEBUG << "pastStream before: " << pastStream << endl;
#endif
	}

//            //DELETE
//            // delete key press causes the first character of
//            // futureBuffer to be erased
//            if ( character == DELETE && !futureBuffer.empty() ) {
//				
//                futureBuffer.erase( futureBuffer.begin(),
//                                    futureBuffer.begin()+1 );
//
//            }
//
//            //PAGE_UP
//            // see up_arrow
//            if ( character == PAGE_UP ) {
//
//                pastBuffer.clear();
//                futureBuffer.clear();
//
//            }
//			
//            //PAGE_DOWN
//            // see up_arrow
//            if ( character == PAGE_DOWN ) {
//
//                pastBuffer.clear();
//                futureBuffer.clear();
//
//            }
			
    } else {
	logger << ERROR << "Error parsing character: " << character << endl
	       << "The error occured while executing update(" << character << ")" << endl;
	//abort();
    }

#ifdef USE_STRINGSTREAM
    logger << DEBUG << "pastStream: " << pastStream.str() << endl;
#else
    logger << DEBUG << "pastStream: " << pastStream << endl;
#endif
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
 */
bool ContextTracker::contextChange() const
{
    return contextChanged;
}

std::string ContextTracker::getPrefix()
{
//    ReverseTokenizer tokenizer(pastStream, blankspaceChars, separatorChars);
//
//    //DEBUG
//    //std::cerr << "ContextTracker::getPrefix()" << std::endl;
//    //std::cerr << "tokenizer.streamToString() " << tokenizer.streamToString() << std::endl;
//    //std::cerr << "stream: " << pastStream.str() << std::endl;
//
//    if (tokenizer.hasMoreTokens()) {
//        return tokenizer.nextToken();
//    } else {
//        return "";
//    }

//    ForwardTokenizer tokenizer(pastStream, blankspaceChars, separatorChars);
//    std::string result;
//    while (tokenizer.hasMoreTokens()) {
//	result = tokenizer.nextToken();
//    }
//    return result;

    return getToken(0);
}

std::string ContextTracker::getToken(const int index)
{
#ifdef USE_STRINGSTREAM
    ReverseTokenizer tokenizer(pastStream, blankspaceChars, separatorChars);
#else
    std::stringstream pastStringStream(pastStream);
    ReverseTokenizer tokenizer(pastStringStream, blankspaceChars, separatorChars);
#endif

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

//vector<string> ContextTracker::getTokens(const int i, const int j) const
//{}

std::string ContextTracker::getFutureStream() const
{
#ifdef USE_STRINGSTREAM
    return futureStream.str();
#else
    return futureStream;
#endif
}

std::string ContextTracker::getPastStream() const
{
#ifdef USE_STRINGSTREAM
    return pastStream.str();
#else
    return pastStream;
#endif
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
#ifdef USE_STRINGSTREAM
    return pastStream.str() + "<|>" + futureStream.str() + "\n";
#else
    return pastStream + "<|>" + futureStream + "\n";
#endif
}

int ContextTracker::getMaxBufferSize() const
{
    return MAX_BUFFER_SIZE;
}

void ContextTracker::setMaxBufferSize( const int size )
{
    if( size > 0 )
        MAX_BUFFER_SIZE = size;
}
