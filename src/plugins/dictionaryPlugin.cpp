
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

#include "plugins/dictionaryPlugin.h"

#include <fstream>
#include <assert.h>

DictionaryPlugin::DictionaryPlugin(Configuration* config, ContextTracker* ht)
    : Plugin(config,
	     ht,
	     "DictionaryPlugin",
	     "DictionaryPlugin, dictionary lookup",
	     "DictionaryPlugin is a dictionary based plugin.\n"
	     "It searches a list of words and returns those that contain the current prefix as a prefix." )
{
    Variable variable;
    variable.push_back("Soothsayer");
    variable.push_back("Plugins");
    variable.push_back("DictionaryPlugin");
    variable.push_back("DICTIONARY_PATH");
    Value value = config->get(variable);
    
    DICTIONARY_PATH = value;
}

DictionaryPlugin::~DictionaryPlugin()
{}

Prediction DictionaryPlugin::predict(const size_t max_partial_predictions_size) const
{
//	std::cout << "DictionaryPlugin::predict() method called" << std::endl;
	
    Prediction p;

    // open dictionary file for input
    std::ifstream dictFile;

    dictFile.open(DICTIONARY_PATH.c_str());

    if(!dictFile)
	std::cerr << "Dictionary file could not be opened\a"
		  << std::endl;
    assert(dictFile);

    std::string candidate;
    std::string prefix = contextTracker->getPrefix();

    // scan file entries until we get enough suggestions
    unsigned int count = 0;
    while(dictFile >> candidate && count < max_partial_predictions_size) {
	if(candidate.find(prefix) == 0) {
	    p.addSuggestion(Suggestion(candidate,0.1));
	    count++;
	    std::cout << "o";
	} else {
	    std::cout << ".";
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
