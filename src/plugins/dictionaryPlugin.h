
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


#ifndef SOOTH_DICTIONARYPLUGIN
#define SOOTH_DICTIONARYPLUGIN

#include "plugins/plugin.h"


/** Dictionary plugin.
 *
 * Uses a list of italian words. Returns those words that contain the
 * current prefix as a prefix.
 *
 */
class DictionaryPlugin : public Plugin {
public:
    DictionaryPlugin(HistoryTracker&, Profile*);
    ~DictionaryPlugin();

    virtual Prediction predict() const;

    virtual void learn();
    virtual void extract();
    virtual void train();

private:
    std::string DICTIONARY_PATH;
	
};


// Class factory functions
extern "C" DictionaryPlugin* create (HistoryTracker&, Profile*);
extern "C" void              destroy(DictionaryPlugin*);


#endif // SOOTH_DICTIONARYPLUGIN
