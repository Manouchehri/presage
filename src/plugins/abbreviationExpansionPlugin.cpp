
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


#include "plugins/abbreviationExpansionPlugin.h"
#include <fstream>

#define DEBUG

#ifdef DEBUG
# define LOG(x) std::cout << x << std::endl
#else
# define LOG(x) /* x */
#endif

AbbreviationExpansionPlugin::AbbreviationExpansionPlugin(Profile* profile, HistoryTracker* ht)
    : Plugin(profile,
             ht,
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
	variable.push_back("ABBREVIATIONS");
	value = profile->getConfig(variable);
        LOG("[AbbreviationExpansionPlugin] ABBREVIATIONS:" + value);
        abbreviations = value;
	variable.pop_back();
    } catch (Profile::ProfileException ex) {
        std::cerr << "[AbbreviationExpansionPlugin] Caught ProfileException: " << ex.what() << std::endl;
    }

    cacheAbbreviationsExpansions();
}

AbbreviationExpansionPlugin::~AbbreviationExpansionPlugin()
{}


Prediction AbbreviationExpansionPlugin::predict() const
{
    Prediction result;

    std::map< std::string, std::string >::const_iterator it = 
        cache.find(historyTracker->getPrefix());

    if (it != cache.end()){
        //result.addSuggestion(Suggestion(it->second, 1.0));

        // prepend expansion with enough backspaces to erase
        // abbreviation
        std::string expansion(historyTracker->getPrefix().size(), '\b');

        // concatenate actual expansion
        expansion += it->second;

        result.addSuggestion(Suggestion(expansion, 1.0));

    } else {
        LOG("[AbbreviationExpansionPlugin] Could not find expansion for abbreviation: " + historyTracker->getPrefix());
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
        LOG("[AbbreviationExpansionPlugin] Could not open abbreviations file: " + abbreviations);
        // TODO: throw exception here
        //

    } else {
        LOG("[AbbreviationExpansionPlugin] Caching abbreviations/expansions from file: " + abbreviations);
    
        std::string buffer;
        std::string abbreviation;
        std::string expansion;
        std::string::size_type tab_pos;
        while (getline(abbr_file, buffer)) {
            tab_pos = buffer.find_first_of('\t');
            if (tab_pos == std::string::npos) {
                LOG("[AbbreviationExpansionPlugin] Error reading abbreviations/expansions from file: " + abbreviations);
            } else {
                abbreviation = buffer.substr(0, tab_pos);
                expansion    = buffer.substr(tab_pos + 1, std::string::npos);

                LOG("[AbbreviationExpansionPlugin] Caching abbreviation: " + abbreviation + " - expansion: " + expansion);
                cache[abbreviation] = expansion;
            }
            //LOG("[AbbreviationExpansionPlugin] Error reading expansion for abbreviation: " + abbreviation + " from file: " + abbreviations);
        }
        
        abbr_file.close();
    }
}
