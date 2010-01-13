
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


#include "plugins/dummyPlugin.h"
#include "core/context_tracker/contextTracker.h"
#include "../common/stringstreamPresageCallback.h"

#include <iostream>
#include <vector>
#include <string>

int main()
{
	Configuration mock_config;
	std::stringstream stream;
	StringstreamPresageCallback callback(stream);
	ContextTracker contextTracker(&mock_config, 0, &callback);
	DummyPlugin dummyPlugin(&mock_config, &contextTracker);
	Plugin* pluginPtr = &dummyPlugin;
	

	std::cout << dummyPlugin.getName()
		  << " test driver program"
		  << std::endl;

	std::vector<std::string> change;
	dummyPlugin.learn(change);
	dummyPlugin.extract();
	dummyPlugin.train();

	std::cout << dummyPlugin.predict(100, 0);

	std::cout << "Now testing polymorphic behaviour..." << std::endl;

	pluginPtr->learn(change);
	pluginPtr->extract();
	pluginPtr->train();
	std::cout << pluginPtr->predict(100, 0);

	std::cout << "Testing name and description functions..." << std::endl
		  << "Name: " << pluginPtr->getName() << std::endl
		  << "Short description: " << pluginPtr->getShortDescription()
		  << std::endl
		  << "Long description: " << pluginPtr->getLongDescription()
		  << std::endl;

	return 0;
}

