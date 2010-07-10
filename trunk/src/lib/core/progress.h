
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


#ifndef PROGRESS_H
#define PROGRESS_H

#include <iostream>

template <class _charT, class _Traits=std::char_traits<_charT> >
class ProgressBar {
    
private:
    int progress;
    int quantum;
    std::basic_ostream<_charT,_Traits>&  outstream;
    
public:
    ProgressBar(std::basic_ostream<_charT,_Traits>& ostr = std::cout) : outstream(ostr)
	{
	    progress = 0;
	    quantum = 2;
	    outstream << "0---10---20---30---40---50---60---70---80---90--100" << std::endl;
	}
    
    ~ProgressBar()
	{
	    for (int i = progress; i <= 100; i += quantum) {
		outstream << '#';
	    }
	    outstream << std::endl;
	}

    void update(const double percentage)
	{
	    if ((percentage*100) >= progress) {
		progress += quantum;
		outstream << '#' << std::flush;
	    }
	}

};

#endif

