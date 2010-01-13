
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


#ifndef PRESAGE_TOKENIZER
#define PRESAGE_TOKENIZER

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <istream>
#include <string>
#include <assert.h>

/** The Tokenizer class takes an input stream and parses it into "tokens", allowing the tokens to be read one at a time.
 * 
 * The parsing process is controlled by the character classification
 * sets: 
 *
 * - blankspace characters: characters that mark a token boundary and
 *                          are not part of the token.
 *
 * - separator characters: characters that mark a token boundary and
 *                         might be considered tokens, depending on the
 *                         value of a flag (to be implemented).
 *
 * - valid characters: any non blankspace and non separator character.
 *
 * Each byte read from the input stream is regarded as a character in
 * the range '\\u0000' through '\\u00FF'.
 *
 * In addition, an instance has flags that control:
 *
 * - whether the characters of tokens are converted to lowercase. 
 * - whether separator characters constitute tokens. (TBD)
 * 
 * A typical application first constructs an instance of this class,
 * supplying the input stream to be tokenized, the set of blankspaces,
 * and the set of separators, and then repeatedly loops, while method
 * hasMoreTokens returns true, calling the nextToken method.
 */
class Tokenizer {
public:
    Tokenizer(std::istream& stream,
	      const std::string   blankspaces,
	      const std::string   separators  );
    virtual ~Tokenizer();

    /** Returns the number of tokens left.
     */
    virtual int countTokens() = 0;

    /** Tests if there are more tokens.
     */
    virtual bool hasMoreTokens() const = 0;
    
    /** Returns the next token.
     */
    virtual std::string nextToken() = 0;

    /** Returns progress percentage.
     */
    virtual double progress() const = 0;


    /** Sets blankspace characters.
     */
    void blankspaceChars(const std::string);
    /** Gets blankspace characters.
     */
    std::string blankspaceChars() const;

    /** Sets separator characters.
     */
    void separatorChars(const std::string);
    /** Gets separator characters.
     */
    std::string separatorChars() const;

    /** Sets lowercase mode.
     */
    void lowercaseMode(const bool);
    /** Gets lowercase mode.
     */
    bool lowercaseMode() const;

    std::string streamToString() const {
	std::streamoff offbackup = stream.tellg();
	std::string str;
	std::streamoff curroff = offbeg;
	stream.seekg(curroff);
	while (curroff < offend) {
            stream.clear();
	    str.push_back(stream.peek());
	    curroff++;
	    stream.seekg(curroff);
	}
	stream.seekg(offbackup);
	return str;
    }
    
protected:
    class StreamGuard {
    public:
	StreamGuard(std::istream& so, std::streamoff& of) 
	    : guardedStream(so) {
            currstate = guardedStream.rdstate();
	    curroff   = guardedStream.tellg();
	    guardedStream.seekg   (of       );
	}
	~StreamGuard() {
	    guardedStream.seekg   (curroff  );
            guardedStream.setstate(currstate);
	}

    private:
	std::istream&     guardedStream;
        std::ios::iostate currstate;
	std::streamoff    curroff;
    };

    std::istream&     stream;
    std::ios::iostate sstate;
    std::streamoff    offbeg;
    std::streamoff    offend;
    std::streamoff    offset;

    bool isBlankspace(const int character) const;
    bool isSeparator (const int character) const;

private:
    std::string blankspaces;
    std::string separators;

    bool lowercase;
};

#endif // PRESAGE_TOKENIZER
