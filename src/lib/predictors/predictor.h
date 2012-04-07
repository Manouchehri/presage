
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


#ifndef PRESAGE_PREDICTOR
#define PRESAGE_PREDICTOR

#include "../core/prediction.h"
#include "../core/context_tracker/contextTracker.h"
#include "../core/configuration.h"
#include "../core/utility.h"

#include <string>
#include <vector>

/** Predictor is an abstract class that defines the interface implemented by concrete predictors.
 * 
 * The prediction algorithm implemented by predictors is at the heart
 * of presage.  The predictor-based architecture allows new prediction
 * methods to be easily and transparently added to the system.
 * 
 * Predictors have access to and rely on the services provided by core
 * components of the presage system.
 *
 */
class Predictor {
public:
    Predictor(Configuration*  configuration,
	      ContextTracker* contextTracker,
	      const char*     predictorName    = "Predictor",
	      const char*     shortDescription = "",
	      const char*     longDescription  = "");
    virtual ~Predictor();

    /** \brief Generate prediction.
     *
     * \param size    desired prediction size
     * \param filter  filter
     *
     */
    virtual Prediction predict(const size_t size, const char** filter) const = 0;

    virtual void learn(const std::vector<std::string>& change) = 0;

    const std::string getName() const;
    const std::string getShortDescription() const;
    const std::string getLongDescription() const;


protected:
    virtual bool token_satisfies_filter (const std::string& token,
					 const std::string& prefix,
					 const char** filter) const;

    virtual void set_logger (const std::string& level);

    const std::string name;
    const std::string shortDescription; // predictor's descriptive name
    const std::string longDescription;  // predictor's exhaustive description

    const std::string PREDICTORS;

    ContextTracker* contextTracker;

    Configuration* configuration;
    
    Logger<char> logger;

private:

};


#endif // PRESAGE_PREDICTOR
