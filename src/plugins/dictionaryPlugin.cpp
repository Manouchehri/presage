
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>            *
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


#include "plugins/dictionaryPlugin.h"

#include <fstream>
#include <assert.h>

DictionaryPlugin::DictionaryPlugin( HistoryTracker &ht )
    : Plugin( ht,
	      "DictionaryPlugin",
	      "DictionaryPlugin, dictionary lookup",
	      "DictionaryPlugin is a dictionary based plugin.\nIt searches a list of words and returns those that contain the current prefix as a prefix." )
{
    options.push_back(Option("/usr/share/dict/italian",
			     STRING,
			     "DICTIONARY_PATH",
			     "Dictionary file path" )
	);
}

DictionaryPlugin::~DictionaryPlugin()
{}

Prediction DictionaryPlugin::predict() const
{
//	std::cout << "DictionaryPlugin::predict() method called" << std::endl;
	
    Prediction p;

    // open dictionary file for input
    std::ifstream dictFile;

    dictFile.open(getOptionValue("DICTIONARY_PATH").data());

    if(!dictFile)
	std::cerr << "Dictionary file could not be opened\a"
		  << std::endl;
    assert(dictFile);

    std::string candidate;
    std::string prefix = historyTracker.getPrefix();

    // scan file entries until we get enough suggestions
    while(dictFile >> candidate) {
	if(candidate.find(prefix) == 0) {
	    p.addSuggestion(Suggestion(candidate,0.1));
	}
    }

//	std::cout << "DictionaryPlugin::predict() method exited" << std::endl;
    return p;
}

void DictionaryPlugin::learn()
{
    std::cout << "DictionaryPlugin::learn() method called" << std::endl;
    std::cout << "DictionaryPlugin::learn() method exited" << std::endl;
}

void DictionaryPlugin::extract()
{
    std::cout << "DictionaryPlugin::extract() method called" << std::endl;
    std::cout << "DictionaryPlugin::extract() method exited" << std::endl;
}

void DictionaryPlugin::train()
{
    std::cout << "DictionaryPlugin::train() method called" << std::endl;
    std::cout << "DictionaryPlugin::train() method exited" << std::endl;
}


extern "C" Plugin* create( HistoryTracker& ht )
{
    return new DictionaryPlugin( ht );
}

extern "C" void destroy( Plugin *p )
{
    delete p;
}
