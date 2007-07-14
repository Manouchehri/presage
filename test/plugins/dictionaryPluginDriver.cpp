
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


#include <iostream>
#include "plugins/dictionaryPlugin.h"

int main()
{
    // This won't work. It is kept here so that when breaking changes
    // are made, this will fail to build.
    // TODO: transform this into a unit test.
	Profile mockProfile(0);
	HistoryTracker historyTracker(&mockProfile);
	DictionaryPlugin plugin(&mockProfile, &historyTracker);

	std::string prefix;

	std::cout << "Insert prefix: ";
	std::cin >> prefix;

	historyTracker.update( prefix );
	
	std::cout << plugin.predict();

	return 0;

}
