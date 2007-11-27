
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


#ifndef SOOTH_ABBREVIATIONEXPANSIONPLUGIN
#define SOOTH_ABBREVIATIONEXPANSIONPLUGIN

#include "plugins/plugin.h"


/** Abbreviation expansion plugin.
 *
 * This plugin maps abbreviations to the corresponding fully expanded
 * token (i.e. word or phrase).
 *
 * The mapping between abbreviations and expansions is stored in the
 * file specified by the plugin configuration section.
 *
 * The format for the abbreviation-expansion database is a simple tab
 * separated text file format, with each abbreviation-expansion pair
 * per line.
 *
 */
class AbbreviationExpansionPlugin : public Plugin {
public:
    AbbreviationExpansionPlugin(Profile*, ContextTracker*);
    ~AbbreviationExpansionPlugin();

    virtual Prediction predict(const int);

    virtual void learn();
    virtual void extract();
    virtual void train();

private:
    void cacheAbbreviationsExpansions();
    
    std::string abbreviations;
    std::map< std::string, std::string> cache;
};

#endif // SOOTH_ABBREVIATIONEXPANSIONPLUGIN
