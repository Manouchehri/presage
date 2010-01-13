
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


#ifndef PRESAGE_DICTIONARYPLUGIN
#define PRESAGE_DICTIONARYPLUGIN

#include "plugins/plugin.h"
#include <fstream>


/** Dictionary predictive plugin.
 *
 * Generates a prediction by extracting tokens that start with the
 * current prefix from a given dictionary.
 *
 */
class DictionaryPlugin : public Plugin {
public:
    DictionaryPlugin(Configuration*, ContextTracker*);
    ~DictionaryPlugin();

    virtual Prediction predict(const size_t size, const char** filter) const;

    virtual void learn(const std::vector<std::string>& change);
    virtual void extract();
    virtual void train();

private:
    static const Variable DICTIONARY;
    static const Variable PROBABILITY;

    std::string dictionary_path;
    double probability;

};

#endif // PRESAGE_DICTIONARYPLUGIN
