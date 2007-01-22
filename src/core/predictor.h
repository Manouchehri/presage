
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>            *
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

#ifndef SOOTH_PREDICTOR
#define SOOTH_PREDICTOR

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// PLUMP
// REVISIT uncomment this when reenabling plump
//#include "plump/src/plump.h"
//namespace plump { typedef int Plump; }

#include "core/historyTracker.h"
#include "core/prediction.h"
#include "core/combiner.h"
#include "plugins/plugin.h"

#include <stdlib.h>  // needed for abort function
//#include <dlfcn.h>   // needed for shared library dynamic loading


// Default option values
const int DEFAULT_PREDICT_TIME = 1000;
const CombinationMethod DEFAULT_COMBINATION_METHOD = LINEAR;


/** Predictor, the heart of Soothsayer system, coordinates the execution of predictive plugins and returns the combination of their predictions.
 *
 * Predictor starts the execution of the active predictive plugins,
 * monitors their execution and collects the predictions returned, or
 * terminates a predictive plugin's execution if it execedes its
 * maximum prediction time.
 * 
 * The predictions returned by the individual predictive plugins are
 * combined into a single prediction by the active Combiner. (please
 * refer to my thesis for a list of possible conbination strategies.
 *
 */
class Predictor {
  public:
//PLUMP    Predictor(HistoryTracker&,
//PLUMP              plump::Plump&);
    Predictor(HistoryTracker&);
    ~Predictor();

    /**
     * Returns the prediction created by all active predictive plugins.
     */
    Prediction predict() const;


    /**
     * Adds @param plugin to the list of active plugins.
     */
    void addPlugin(Plugin* plugin);
    /**
     * Removes @param plugin from the list of active plugins.
     */
    bool removePlugin(const Plugin* plugin);

    /**
     * Returns the maximum time plugins are allowed to execute before
     * returning a prediction.
     */
    int getPredictTime() const;
    /**
     * Sets the maximum time plugins are allowed to execute before
     * returning a prediction.
     *
     * @param milliseconds time in milliseconds
     */
    bool setPredictTime(const int milliseconds);
    /** 
     * Returns the active combination method used by predictor to
     * combine predictions returned by the active predictive plugins
     * into one prediction.
     */
    CombinationMethod getCombinationMethod() const;
    /** 
     * Sets the combination method used by predictor to combine
     * predictions returned by the active predictive plugins into one
     * prediction.
     */
    bool setCombinationMethod( const CombinationMethod );

  private:
    
    // PLUMP
    //plump::Plump& plump;

    // init methods
    bool initCombiner();

    // execute plugin function (invoked in thread)
    void *execute(void *);

    HistoryTracker &historyTracker;

    Combiner *combiner;
    std::vector<Plugin*> plugins;        // active Plugins
    std::vector<Prediction> predictions; // predictions computed by each plugin are returned here

    int PREDICT_TIME;
    CombinationMethod COMBINATION_METHOD;

    /* Prediction contains a list(or vector) of Suggestion. When a
       Predition object is passed by value, it is passed by default
       memberwise copy. That (could) creates a problem because there's
       only one list of Suggestion pointed at by different object and
       its lifetime can be inadequate.  A solution could be to
       overload the operator= or make sure it gets a proper lifetime
       (it is instantiated in the right scope).
    */
};

#endif // SOOTH_PREDICTOR
