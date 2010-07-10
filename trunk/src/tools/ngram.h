
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


#ifndef PRESAGE_NGRAM
#define PRESAGE_NGRAM

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

class Ngram {
    friend std::ostream& operator<<( std::ostream&, const Ngram& );
public:
    Ngram(const int N);
    ~Ngram();

    int getN() const;

    std::string getNgram(const int n) const;
    void setNgram(const int n, const std::string str);

    bool operator<( const Ngram& ) const;
    Ngram& operator=( const Ngram& );

    std::string toString() const;

private:
    int N;
    std::string* ngrams;

};

#endif // PRESAGE_NGRAM
