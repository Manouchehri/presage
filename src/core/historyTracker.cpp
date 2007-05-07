
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

HistoryTracker::HistoryTracker(const char wChars[], const char tChars[],
			       const char bChars[], const char cChars[])
    : wordChars      (wChars),
      separatorChars (tChars),
      blankspaceChars(bChars),
      controlChars   (cChars)
      //tokenizer      (pastStream, blankspaceChars, separatorChars)
{
    //pastStream.clear();
    //pastStream.seekg(0, std::ios::beg);
    //pastStream.seekp(0, std::ios::end);
    assert(pastStream.good());

    setMaxBufferSize(DEFAULT_MAX_BUFFER_SIZE);
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

        if( isWordChar(s[i])
            || isSeparatorChar(s[i])
            || isBlankspaceChar(s[i]) ) {
            // if s is a word string, append to pastBuffer
            // if s is a separator string, append to pastBuffer
            //std::cerr << "HistoryTracker::update(" << s[i] << ") ";
            assert(pastStream.good());
            pastStream.put(s[i]);
            assert(pastStream.good());
            //std::cerr << pastStream.str() << std::endl;
            //std::cerr << "HistoryTracker::update() tokenizer.streamToString() " << tokenizer.streamToString() << std::endl;
        } else if ( isControlChar(s[i]) ) {
            //if s is a control string, take the appropriate action

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
//            //BACKSPACE
//            // backspace key press causes the last character of
//            // pastBuffer to be erased
//            if ( s[i] == BACKSPACE && !pastBuffer.empty() ) {
//
//                pastBuffer.erase( pastBuffer.end()-1 );
//
//            }
//
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
	    std::cerr << "\aAn error occured in function " 
                 << "void HistoryTracker.update(" << s
                 << ")" << std::endl << std::endl
                 << "The error occured when I tried processing "
                 << "character/string \"" << s << "\""
                 << std::endl << std::endl;
            for( std::string::const_iterator i = s.begin();
                 i != s.end();
                 i++ ) {
                std::cerr << "Char: " << *i << "\tInt: "
                     << static_cast<int>( *i ) << std::endl;
            }
            sleep(1);
            //abort();
        }
    }

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
    ReverseTokenizer tokenizer(pastStream, blankspaceChars, separatorChars);

    std::string token;
    int i = 0;
    while (tokenizer.hasMoreTokens() && i <= index) {
        token = tokenizer.nextToken();
        i++;
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
    return futureStream.str();
}

std::string HistoryTracker::getPastStream() const
{
    return pastStream.str();
}

bool HistoryTracker::isWordChar(const char c) const
{
    char str[2];
    str[0] = c;
    str[1] = '\0';
    if( wordChars.find( str, 0 ) != std::string::npos )
        return true;
    else
        return false;
	
}

bool HistoryTracker::isSeparatorChar(const char c) const
{
    char str[2];
    str[0] = c;
    str[1] = '\0';
    if( separatorChars.find( str, 0 ) != std::string::npos )
        return true;
    else
        return false;

}

bool HistoryTracker::isBlankspaceChar(const char c) const
{
    for( unsigned int i=0; i<blankspaceChars.size(); i++ ) {
        if( blankspaceChars[i] == c ) {
            return true;
        }
    }

    return false;

}

bool HistoryTracker::isControlChar(const char c) const
{
    char str[2];
    str[0] = c;
    str[1] = '\0';
    if( controlChars.find( str, 0 ) != std::string::npos )
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
    return pastStream.str() + "<|>" + futureStream.str() + "\n";
	
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
