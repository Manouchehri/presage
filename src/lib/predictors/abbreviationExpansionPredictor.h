
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


#ifndef PRESAGE_ABBREVIATIONEXPANSIONPREDICTOR
#define PRESAGE_ABBREVIATIONEXPANSIONPREDICTOR

#include "predictor.h"
#include "../core/dispatcher.h"


/** Abbreviation expansion predictor.
 *
 * This predictor maps abbreviations to the corresponding fully expanded
 * token (i.e. word or phrase).
 *
 * The mapping between abbreviations and expansions is stored in the
 * file specified by the predictor configuration section.
 *
 * The format for the abbreviation-expansion database is a simple tab
 * separated text file format, with each abbreviation-expansion pair
 * per line.
 *
 */
class AbbreviationExpansionPredictor : public Predictor, public Observer {
public:
    AbbreviationExpansionPredictor(Configuration*, ContextTracker*, const char*);
    ~AbbreviationExpansionPredictor();

    virtual Prediction predict(const size_t size, const char** filter) const;

    virtual void learn(const std::vector<std::string>& change);

    virtual void update (const Observable* variable);

private:
    std::string LOGGER;
    std::string ABBREVIATIONS;

    void set_abbreviations (const std::string& filename);
    void cacheAbbreviationsExpansions();

    std::string abbreviations;
    std::map< std::string, std::string> cache;

    Dispatcher<AbbreviationExpansionPredictor> dispatcher;
};

#endif // PRESAGE_ABBREVIATIONEXPANSIONPREDICTOR
