
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

#include "plugins/abbreviationExpansionPlugin.h"
#include <fstream>


AbbreviationExpansionPlugin::AbbreviationExpansionPlugin(Profile* profile, ContextTracker* ct)
    : Plugin(profile,
             ct,
             "AbbreviationExpansionPlugin",
             "AbbreviationExpansionPlugin, maps abbreviations to the corresponding fully expanded token.",
             "AbbreviationExpansionPlugin maps abbreviations to the corresponding fully expanded token (i.e. word or phrase).\n\nThe mapping between abbreviations and expansions is stored in the file specified by the plugin configuration section.\n\nThe format for the abbreviation-expansion database is a simple tab separated text file format, with each abbreviation-expansion pair per line."
    )
{
    Variable variable;
    variable.push_back("Soothsayer");
    variable.push_back("Plugins");
    variable.push_back("AbbreviationExpansionPlugin");

    Value value;

    try {
	variable.push_back("LOGGER");
	value = profile->getConfig(variable);
	logger << setlevel(value);
        logger << INFO << "LOGGER:" << value << endl;
	variable.pop_back();

	variable.push_back("ABBREVIATIONS");
	value = profile->getConfig(variable);
        logger << INFO << "ABBREVIATIONS:" << value << endl;
        abbreviations = value;
	variable.pop_back();
    } catch (Profile::ProfileException ex) {
        logger << ERROR << "Caught ProfileException: " << ex.what() << endl;
    }

    cacheAbbreviationsExpansions();
}

AbbreviationExpansionPlugin::~AbbreviationExpansionPlugin()
{}


Prediction AbbreviationExpansionPlugin::predict(const size_t max_partial_predictions_size) const
{
    Prediction result;

    std::map< std::string, std::string >::const_iterator it = 
        cache.find(contextTracker->getPrefix());

    if (it != cache.end()){
        //result.addSuggestion(Suggestion(it->second, 1.0));

        // prepend expansion with enough backspaces to erase
        // abbreviation
        std::string expansion(contextTracker->getPrefix().size(), '\b');

        // concatenate actual expansion
        expansion += it->second;

        result.addSuggestion(Suggestion(expansion, 1.0));

    } else {
        logger << NOTICE << "Could not find expansion for abbreviation: " << contextTracker->getPrefix() << endl;
    }

    return result;
}

void AbbreviationExpansionPlugin::learn()
{}

void AbbreviationExpansionPlugin::extract()
{}

void AbbreviationExpansionPlugin::train()
{}

void AbbreviationExpansionPlugin::cacheAbbreviationsExpansions()
{
    cache.clear();

    std::ifstream abbr_file(abbreviations.c_str());
    if (!abbr_file) {
        logger << ERROR << "Could not open abbreviations file: " << abbreviations << endl;
        // TODO: throw exception here
        //

    } else {
        logger << INFO << "Caching abbreviations/expansions from file: " << abbreviations << endl;
    
        std::string buffer;
        std::string abbreviation;
        std::string expansion;
        std::string::size_type tab_pos;
        while (getline(abbr_file, buffer)) {
            tab_pos = buffer.find_first_of('\t');
            if (tab_pos == std::string::npos) {
                logger << ERROR << "Error reading abbreviations/expansions from file: " << abbreviations << endl;
            } else {
                abbreviation = buffer.substr(0, tab_pos);
                expansion    = buffer.substr(tab_pos + 1, std::string::npos);

                logger << INFO << "Caching abbreviation: " << abbreviation << " - expansion: " << expansion << endl;
                cache[abbreviation] = expansion;
            }
        }
        
        abbr_file.close();
    }
}
