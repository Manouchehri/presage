
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


#include "historyTracker.h"
#include "utility.h"

#define DEBUG

#ifdef DEBUG
# define LOG(x) std::cerr << x << std::endl
#else
# define LOG(x) /* x */
#endif


HistoryTracker::HistoryTracker(Profile* profile,
			       const char wChars[],
			       const char tChars[],
			       const char bChars[],
			       const char cChars[])
    : wordChars      (wChars),
      separatorChars (tChars),
      blankspaceChars(bChars),
      controlChars   (cChars)
      //tokenizer      (pastStream, blankspaceChars, separatorChars)
{
    //pastStream.clear();
    //pastStream.seekg(0, std::ios::beg);
    //pastStream.seekp(0, std::ios::end);
#ifdef USE_STRINGSTREAM
    assert(pastStream.good());
#endif

    // read config values
    Variable variable;
    variable.push_back("Soothsayer");
    variable.push_back("HistoryTracker");

    Value value;

    try {
	variable.push_back("MAX_BUFFER_SIZE");
	value = profile->getConfig(variable);
	LOG("[HistoryTracker] MAX_BUFFER_SIZE: " + value);
	setMaxBufferSize(toInt(value));
	variable.pop_back();
    } catch (Profile::ProfileException ex) {
	std::cerr << "[HistoryTracker] Caught ProfileException: " << ex.what() << std::endl;
    }

    contextChanged = true;
}

HistoryTracker::~HistoryTracker()
{
    // nothing to do here, move along
}

void HistoryTracker::update(std::string s)
{
    // process each char in string s individually
    for (unsigned int i=0; i<s.size(); i++) {

	//DEBUG
        //std::cout << "s[i] = " << static_cast<int>(s[i]) << std::endl;
        //std::cout << "s[i] " << (isControlChar( s[i] )?"":"non")
        //          << " e' un carattere di controllo." << std::endl;

        if(isWordChar(s[i])
           || isSeparatorChar(s[i])
           || isBlankspaceChar(s[i])) {
            // if s is a word string, append to pastBuffer
            // if s is a separator string, append to pastBuffer
            LOG("[HistoryTracker] updating wordChar/separatorChar/blankspaceChar: " + s[i]);
#ifdef USE_STRINGSTREAM
            assert(pastStream.good());
            pastStream.put(s[i]);
            assert(pastStream.good());
#else
	    pastStream.push_back(s[i]);
#endif
            //std::cerr << pastStream.str() << std::endl;
            //std::cerr << "HistoryTracker::update() tokenizer.streamToString() " << tokenizer.streamToString() << std::endl;
        } else if ( isControlChar(s[i]) ) {
            //if s is a control string, take the appropriate action
            LOG("[HistoryTracker] updating controlChar: " + s[i]);

// REVISIT ////

// Temporarily disabling control chars

//            //UP_ARROW
//            // up_arrow key press messes things up! There's
//            // currently no way of knowing where the cursors ends
//            // up, so we're better off clearing our history up and
//            // hoping that doesn't modify an existing word but
//            // starts a new one.
//            if ( s[i] == UP_ARROW ) {
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
//            if ( s[i] == LEFT_ARROW && !pastBuffer.empty() ) {
//				
//                char p = pastBuffer[pastBuffer.size()-1];
//                futureBuffer.insert( futureBuffer.begin(), p );
//                pastBuffer.erase( pastBuffer.end()-1 );
//				
//            }
//
//            //DOWN_ARROW
//            // see up_arrow
//            if ( s[i] == DOWN_ARROW ) {
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
//            if ( s[i] == RIGHT_ARROW && !futureBuffer.empty() ) {
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
//            if ( s[i] == END && !futureBuffer.empty() ) {
//				
//                pastBuffer.append( futureBuffer );
//                futureBuffer.clear();
//
//            }
//
//            //HOME
//            // see up_arrow
//            if ( s[i] == HOME ) {
//
//                pastBuffer.clear();
//                futureBuffer.clear();
//
//            }
//
            //BACKSPACE
            // backspace key press causes the last character of
            // pastBuffer to be erased
            if (s[i] == BACKSPACE &&
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
                LOG("[HistoryTracker] pastStream before: " + pastStream.str());
                std::string temp = pastStream.str();
                temp.erase(temp.end() - 1);
                pastStream.str(temp);
                pastStream.seekg(0, std::ios::end);
                LOG("[HistoryTracker] pastStream after : " + pastStream.str());
#else
                LOG("[HistoryTracker] pastStream before: " + pastStream);
                pastStream.erase(pastStream.end() - 1);
                LOG("[HistoryTracker] pastStream before: " + pastStream);
#endif
            }

//            //DELETE
//            // delete key press causes the first character of
//            // futureBuffer to be erased
//            if ( s[i] == DELETE && !futureBuffer.empty() ) {
//				
//                futureBuffer.erase( futureBuffer.begin(),
//                                    futureBuffer.begin()+1 );
//
//            }
//
//            //PAGE_UP
//            // see up_arrow
//            if ( s[i] == PAGE_UP ) {
//
//                pastBuffer.clear();
//                futureBuffer.clear();
//
//            }
//			
//            //PAGE_DOWN
//            // see up_arrow
//            if ( s[i] == PAGE_DOWN ) {
//
//                pastBuffer.clear();
//                futureBuffer.clear();
//
//            }
			
        } else {
	    std::cerr << "[HistoryTracker] Error parsing character: " << s[i] << std::endl
                      << "[HistoryTracker] The error occured while executing update(" << s << ")" << std::endl;
            for( std::string::const_iterator i = s.begin();
                 i != s.end();
                 i++) {
                std::cerr << "[HistoryTracker] Char: " << *i << "\tInt: "
                     << static_cast<int>( *i ) << std::endl;
            }
            sleep(1);
            //abort();
        }

#ifdef USE_STRINGSTREAM
        LOG("[HistoryTracker] pastStream: " + pastStream.str());
#else
        LOG("[HistoryTracker] pastStream: " + pastStream);
#endif
    }


    LOG("[HistoryTracker] contextChange: historyTracker-getPrefix():" + getPrefix());
    LOG("[HistoryTracker] contextChange: historyTracker-getToken(1):" + getToken(1));
    LOG("[HistoryTracker] contextChange: previous_prefix: " + previous_prefix);

    contextChanged = true;

    if (!getPrefix().empty()) {
	LOG("[HistoryTracker] Prefix not empty");
	// if current prefix is not null
	std::string::size_type loc = getPrefix().find(previous_prefix, 0);
	if (loc == 0) {
	    // if current prefix does not contain the previous prefix
	    // at the beginning of the string, the context has not
	    // changed
	    //
	    LOG("[HistoryTracker] Found prefix in getPrefix()");
	    contextChanged = false;
	}

// TODO: if this code block is uncommented, context changes do not
// occur when separator or blankspace chars are entered. However, this
// code intriduces a bug that can be reproduced by entering the
// following text blocks 'foo ' 'bar ' 'foo' 'bar '. The last entered
// block should cause a context change, but it does not.
//

//    } else {
//	// if prefix is empty
//	LOG("[HistoryTracker] Prefix not empty");
//	if (previous_prefix != getToken(1)) {
//	    LOG("[HistoryTracker] Reversing context change");
//	    contextChanged = false;
//	}
    }

    previous_prefix = getPrefix();

    LOG("[HistoryTracker] contextChange: previous_prefix: " + previous_prefix);
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
bool HistoryTracker::contextChange() const
{
    return contextChanged;
}

std::string HistoryTracker::getPrefix()
{
//    ReverseTokenizer tokenizer(pastStream, blankspaceChars, separatorChars);
//
//    //DEBUG
//    //std::cerr << "HistoryTracker::getPrefix()" << std::endl;
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

std::string HistoryTracker::getToken(const int index)
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
//	std::cerr << "HistoryTracker::getToken() current token: " << result << std::endl;
//    }
//    return result;
}

//vector<string> HistoryTracker::getTokens(const int i, const int j) const
//{}

std::string HistoryTracker::getFutureStream() const
{
#ifdef USE_STRINGSTREAM
    return futureStream.str();
#else
    return futureStream;
#endif
}

std::string HistoryTracker::getPastStream() const
{
#ifdef USE_STRINGSTREAM
    return pastStream.str();
#else
    return pastStream;
#endif
}

bool HistoryTracker::isWordChar(const char c) const
{
    if(wordChars.find(c, 0) != std::string::npos)
        return true;
    else
        return false;
}

bool HistoryTracker::isSeparatorChar(const char c) const
{
    if(separatorChars.find(c, 0) != std::string::npos)
        return true;
    else
        return false;
}

bool HistoryTracker::isBlankspaceChar(const char c) const
{
    if(blankspaceChars.find(c, 0) != std::string::npos)
        return true;
    else
        return false;
}

bool HistoryTracker::isControlChar(const char c) const
{
    if(controlChars.find(c, 0) != std::string::npos)
        return true;
    else
        return false;
}

std::string HistoryTracker::getWordChars() const
{
    return wordChars;
}

std::string HistoryTracker::getSeparatorChars() const
{
    return separatorChars;
}

std::string HistoryTracker::getBlankspaceChars() const
{
    return blankspaceChars;
}

std::string HistoryTracker::getControlChars() const
{
    return controlChars;
}

std::string HistoryTracker::toString() const
{
#ifdef USE_STRINGSTREAM
    return pastStream.str() + "<|>" + futureStream.str() + "\n";
#else
    return pastStream + "<|>" + futureStream + "\n";
#endif
}

int HistoryTracker::getMaxBufferSize() const
{
    return MAX_BUFFER_SIZE;
}

void HistoryTracker::setMaxBufferSize( const int size )
{
    if( size > 0 )
        MAX_BUFFER_SIZE = size;
}
