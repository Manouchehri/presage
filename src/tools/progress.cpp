
/*********************************************************
 *  Soothsayer, an extensible predictive text entry system
 *  ------------------------------------------------------
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

#include "progress.h"

#include <iostream>

ProgressBar::ProgressBar()
{
    progress = 0;
    quantum = 2;
    printProgressHeading();
}

ProgressBar::~ProgressBar()
{
    // complete progress bar if not 100% yet
    for (int i = progress; i <= 100; i += quantum) {
	std::cout << '#';
    }
    std::cout << std::endl;
}

// print out progress bar percentage indicator
void ProgressBar::printProgressHeading() const
{
    std::cout << "0---10---20---30---40---50---60---70---80---90--100"
	// << "|----|----|----|----|----|----|----|----|----|----|"
	      << std::endl;
}


// progress bar display
void ProgressBar::update(const double percentage)
{
    if ((percentage*100) >= progress) {
	progress += quantum;
	std::cout << '#' << std::flush;
    }
}
