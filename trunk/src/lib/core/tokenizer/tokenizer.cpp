
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


#include "tokenizer.h"

Tokenizer::Tokenizer(
    std::istream& is,
    const std::string   blankspaces,
    const std::string   separators
)
    : stream(is),
      lowercase(false)
{
    // this should be changed to deal with a !good() stream
    // appropriately
    //assert(stream.good());

    offset = stream.tellg();
    sstate = stream.rdstate();

    StreamGuard(stream, offset);

    stream.seekg(0, std::ios::end);
    offend = stream.tellg();
    stream.seekg(0, std::ios::beg);
    offbeg = stream.tellg();

    blankspaceChars(blankspaces);
    separatorChars (separators );
}

Tokenizer::~Tokenizer()
{
    // reset stream state to enable repeatability
    // (see reverseTokenizerTest::testRepeatability())
    stream.setstate(sstate);
    stream.clear();
}

void Tokenizer::blankspaceChars(const std::string chars)
{
    blankspaces = chars;
}

std::string Tokenizer::blankspaceChars() const
{
    return blankspaces;
}

void Tokenizer::separatorChars(const std::string chars)
{
    separators = chars;
}

std::string Tokenizer::separatorChars() const
{
    return separators;
}

void Tokenizer::lowercaseMode(const bool value)
{
    lowercase = value;
}

bool Tokenizer::lowercaseMode() const
{
    return lowercase;
}

bool Tokenizer::isBlankspace(const int character) const
{
    std::string::size_type ret = blankspaces.find(character);
    if (ret == std::string::npos) {
	return false;
    } else {
	return true;
    }
}

bool Tokenizer::isSeparator(const int character) const
{
    std::string::size_type ret = separators.find(character);
    if (ret == std::string::npos) {
	return false;
    } else {
	return true;
    }
}
