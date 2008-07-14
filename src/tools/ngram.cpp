
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


#include "ngram.h"

Ngram::Ngram(const int size)
{
    assert(size > 0);
    N = size;
    ngrams = new std::string[N];
}

Ngram::~Ngram()
{
    delete[] ngrams;
}

Ngram& Ngram::operator=( const Ngram& other )
{
    if (&other != this) {
	for (int i = 0; i < N; i++) {
	    ngrams[i] = other.ngrams[i];
	}
    }

    return *this;
}


bool Ngram::operator<( const Ngram& other ) const
{
    if (&other != this) {
	for (int i = N - 1; i >= 0; i--) {
	    if (ngrams[i] < other.ngrams[i]) {
		return true;
	    }
	}
    }

    return false;
}

std::string Ngram::toString() const
{
    std::string str;
    for (int i = 0; i < N; i++) {
	str += "<" + ngrams[i] + "> ";
    }
    return str;
}

std::string Ngram::getNgram(const int n) const
{
    assert(n >= 0 && n < N);
    return ngrams[n];
}

void Ngram::setNgram(const int n, const std::string str)
{
    assert(n >= 0 && n < N);
    ngrams[n] = str;
}

int Ngram::getN() const
{
    return N;
}

std::ostream& operator<<( std::ostream& output, const Ngram& b )
{
    output << b.toString();

    return output;
}

