
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


#include "plugins/dummyPlugin.h"


DummyPlugin::DummyPlugin( HistoryTracker &ht )
    : Plugin(ht,
	     "DummyPlugin",
	     "DummyPlugin, a fake plugin",
	     "DummyPlugin is a fake plugin.\n"
	     "It does not perform any actual computation nor implement any prediction mechanism.\n"
	     "It always returns the same sample prediction.\n")
{
    options.push_back( Option( "10",
			       INT,
			       "OPTION1",
			       "OPTION1 is of type integer" )
	);
			   
    options.push_back( Option( "yes",
			       BOOL,
			       "OPTION2",
			       "OPTION2 is of type boolean" )
	);
}

DummyPlugin::~DummyPlugin()
{}

Prediction DummyPlugin::predict() const
{
//	std::cout << "DummyPlugin::predict() method called" << std::endl;
	
    // A real plugin would query its resources to retrieve the most 
    // probable completion of the prefix based on the current history,
    // but this is just a dummy plugin that returns random suggestions.

    Prediction p;
    Suggestion *sPtr;
	
    sPtr = new Suggestion( "foo", 0.5 );
    p.addSuggestion( *sPtr );
    delete sPtr;

    sPtr = new Suggestion( "bar", 0.4 );
    p.addSuggestion( *sPtr );
    delete sPtr;

    sPtr = new Suggestion( "foobar", 0.3 );
    p.addSuggestion( *sPtr );
    delete sPtr;

    sPtr = new Suggestion( "FoO", 0.2 );
    p.addSuggestion( *sPtr );
    delete sPtr;

    sPtr = new Suggestion( "bAr", 0.1 );
    p.addSuggestion( *sPtr );
    delete sPtr;

    sPtr = new Suggestion( "FoObAr", 0.05 );
    p.addSuggestion( *sPtr );
    delete sPtr;
	
//	std::cout << "DummyPlugin::predict() method exited" << std::endl;

    return p;
}

void DummyPlugin::learn()
{
    std::cout << "DummyPlugin::learn() method called" << std::endl;
    std::cout << "DummyPlugin::learn() method exited" << std::endl;
}

void DummyPlugin::extract()
{
    std::cout << "DummyPlugin::extract() method called" << std::endl;
    std::cout << "DummyPlugin::extract() method exited" << std::endl;
}

void DummyPlugin::train()
{
    std::cout << "DummyPlugin::train() method called" << std::endl;
    std::cout << "DummyPlugin::train() method exited" << std::endl;
}


extern "C" Plugin* create( HistoryTracker& ht )
{
    return new DummyPlugin( ht );
}

extern "C" void destroy( Plugin *p )
{
    delete p;
}
