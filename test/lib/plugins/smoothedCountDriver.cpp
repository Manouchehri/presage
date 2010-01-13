
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


#include <iostream>
#include "plugins/smoothedCountPlugin.h"
#include "../common/stringstreamPresageCallback.h"

int main()
{
    // This won't work. It is kept here so that when breaking changes
    // are made, this will fail to build.
    // TODO: transform this into a unit test.
	Configuration mock_config;
	std::stringstream stream;
	StringstreamPresageCallback callback(stream);
	ContextTracker contextTracker(&mock_config, 0, &callback);
	SmoothedCountPlugin plugin(&mock_config, &contextTracker);

	const int SIZE = 80;
	char historyBuffer[SIZE];

	std::cout << "Insert string: ";
	std::cin.getline( historyBuffer, SIZE );

	stream << historyBuffer;
	
	std::cout << plugin.predict(100, 0);

	return 0;

}
