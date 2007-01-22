
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
#include "plugins/dummyPlugin.h"
#include "core/historyTracker.h"
#include <vector>
#include <string>

int main()
{
	HistoryTracker historyTracker;
	DummyPlugin dummyPlugin( historyTracker );
	Plugin* pluginPtr = &dummyPlugin;
	

	std::cout << dummyPlugin.getName()
		  << " test driver program"
		  << std::endl;

	dummyPlugin.learn();
	dummyPlugin.extract();
	dummyPlugin.train();

	std::cout << dummyPlugin.predict();

	std::cout << "Now testing polymorphic behaviour..." << std::endl;

	pluginPtr->learn();
	pluginPtr->extract();
	pluginPtr->train();
	std::cout << pluginPtr->predict();

	std::cout << "Testing name and description functions..." << std::endl
		  << "Name: " << pluginPtr->getName() << std::endl
		  << "Short description: " << pluginPtr->getShortDescription()
		  << std::endl
		  << "Long description: " << pluginPtr->getLongDescription()
		  << std::endl;

	std::cout << "Testing Option handling..." << std::endl
		  << "Retrieving optionTemplate..." << std::endl;

	std::vector<std::string> optTemplate;
	optTemplate = pluginPtr->getOptionTemplate();
	
	std::cout << "Option template retrieved." << std::endl
		  << "Now accessing individual option data..." << std::endl;

	std::vector<std::string>::const_iterator i;
	for( i=optTemplate.begin(); i!=optTemplate.end(); i++ ) {
		std::cout << "Option name       : " << *i << std::endl
			  << "Option value      : " 
			  << pluginPtr->getOptionValue( *i ) << std::endl
			  << "Option default    : "
			  << pluginPtr->getOptionDefault(*i) << std::endl
			  << "Option description: "
			  << pluginPtr->getOptionDescription(*i) << std::endl;
	}

	return 0;

}

