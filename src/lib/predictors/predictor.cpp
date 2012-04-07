
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


#include "predictor.h"

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

/** Creates a predictor object.
 *
 * @param config pointer to configuration
 * @param ct is a reference to HistoryTracker
 * @param predictorName is the predictor name
 * @param shortDesc is a short description of the predictor's functionality
 * @param longDesc is a long description of the predictor's functionality
 */
Predictor::Predictor(Configuration* config,
		     ContextTracker* ct,
		     const char* predictorName,
		     const char* shortDesc,
		     const char* longDesc)
    : name            (predictorName),
      shortDescription(shortDesc ),
      longDescription (longDesc  ),
      contextTracker  (ct        ),
      configuration   (config    ),
      PREDICTORS      ("Presage.Predictors."),
      logger          (predictorName, std::cerr)
{
    // NOTE: predictor implementations deriving from this class should
    // use profile to query the value of needed configuration
    // variables.
}

/** Virtual predictor destructor.
 *
 */
Predictor::~Predictor()
{}

/** Get predictor name.
 *
 */
const std::string Predictor::getName() const
{
    return name;
}

/** Get predictor short description.
 *
 */
const std::string Predictor::getShortDescription() const
{
    return shortDescription;
}

/** Get predictor long description.
 *
 */
const std::string Predictor::getLongDescription() const
{
    return longDescription;
}


void Predictor::set_logger (const std::string& level)
{
    logger << setlevel (level);
    logger << INFO << "LOGGER: " << level << endl;
}


bool Predictor::token_satisfies_filter (const std::string& token,
					const std::string& prefix,
					const char** filter) const
{
    bool result = false;

    if (filter) {
	// filter is not empty, examine each filter token
	int i = 0;
	while (filter[i] && !result) {
	    std::string candidate = prefix + filter[i];

	    // if token starts with candidate
	    if (token.find (candidate) == 0) {
		result = true;
	    }

	    i++;
	}
    } else {
	// filter is empty, token matches
	result = true;
    }

    return result;
}
