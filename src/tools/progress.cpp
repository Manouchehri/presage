
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>            *
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

#include <iostream>

#include "progress.h"


// print out progress bar percentage indicator
void printProgressHeading()
{
    std::cout << "0---10---20---30---40---50---60---70---80---90--100"
	// << "|----|----|----|----|----|----|----|----|----|----|"
	      << std::endl;
}


// progress bar display
void progressBar(double percentage)
{
    static int progress = 0;
    int quantum = 2;

    if ((percentage*100) >= progress) {
	progress += quantum;
	std::cout << '#' << std::flush;
    }
}
