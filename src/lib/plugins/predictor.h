
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

//PLUMP
//#include "plump/src/pluginInterface.h"

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
//PLUMP    : public plump::PluginInterface {
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

    virtual void set_logger (const std::string& level);


protected:
    const std::string name;
    const std::string shortDescription; // predictor's descriptive name
    const std::string longDescription;  // predictor's exhaustive description

    ContextTracker* contextTracker;

    Configuration* configuration;
    
    Logger<char> logger;

private:

};

////
// Excerpt taken from C++-dlopen HOWTO
//
// Loading classes is a bit more difficult because we need an instance
// of a class, not just a pointer to a function.
//
// We cannot create the instance of the class using new because the
// class is not defined in the executable, and because (under some
// circumstances) we don't even know its name.
//
// The solution is achieved through polymorphism. We define a base,
// interface class with virtual members in the executable, and a
// derived, implementation class in the module. Generally the
// interface class is abstract (a class is abstract if it has pure
// virtual functions).
//
// As dynamic loading of classes is generally used for plug-ins which
// must expose a clearly defined interface we would have had to define
// an interface and derived implementation classes anyway.
//
// Next, while still in the module, we define two additional helper
// functions, known as class factory functions. One of these functions
// creates an instance of the class and returns a pointer to it. The
// other function takes a pointer to a class created by the factory
// and destroys it. These two functions are qualified as extern "C".
//
// To use the class from the module, load the two factory functions
// using dlsym just as we loaded the the hello function; then, we can
// create and destroy as many instances as we wish.

// Class factory function types
// typedef Predictor* create_t (HistoryTracker&, Profile*);
// typedef void    destroy_t(Predictor*);


#endif // PRESAGE_PREDICTOR
