
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

// PLUMP
// REVISIT uncomment this when reenabling plump
//#include "plump/src/plump.h"
//namespace plump { typedef int Plump; }

#include "core/contextTracker.h"
#include "core/prediction.h"
#include "core/configuration.h"
#include "core/logger.h"

#include "core/combiner.h"
#include "core/meritocracyCombiner.h"

#include "plugins/plugin.h"


#ifdef STDC_HEADERS
# include <stdlib.h>  // needed for abort function
#endif

//#include <dlfcn.h>   // needed for shared library dynamic loading


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

    /** Construct a predictor object.
     *
     *  Predictor needs a reference to the ContextTracker object to
     *  forward to the predictive plugins for context retrieval and
     *  analysis.
     *
     * @param config pointer to configuration
     * @param contextTracker pointer to ContextTracker
     */
    Predictor(Configuration* config, ContextTracker* contextTracker);

    /** Destroy predictor.
     *
     */
    ~Predictor();

    /** Runs the predictive plugins, combine their predictions and return the resulting prediction.
     *
     * This is the heart of Soothsayer.
     * 
     * Plump will eventually provide the implementation of sequential or parallel execution of plugins.
     *
     * @return prediction produced by the active predictive plugins and combined by the active combiner
     */
    Prediction predict();

    /** Set active predictive plugins.
     *
     * @param pluginList space separated list of predictive plugins
     */
    void setPlugins(const std::string& pluginList);

    /** Adds a predictive plugin to the active runtime plugins list.
     *
     * @param pluginName plugin name string
     */
    void addPlugin(const std::string& pluginName);

    /** Removes all predictive plugins from the active plugins list.
     *
     */
    void removePlugins();

    /** Gets PREDICT_TIME option value.
     *
     * Returns the maximum time plugins are allowed to execute before
     * returning a prediction.
     *
     * @return value of PREDICT_TIME
     */
    int getPredictTime() const;
    
    /** Sets PREDICT_TIME option, the maximum time allowed for a predictive plugin to return its prediction.
     *
     * @param predictTime expressed in milliseconds
     * @return true if the supplied value is valid, false otherwise
     */
    bool setPredictTime(const int predictTime);

     /** Gets COMBINATION_METHOD option value.
      *
      * Returns the active combination method used by predictor to
      * combine predictions returned by the active predictive plugins
      * into one prediction.
      *
      * @return value of COMBINATION_METHOD
      */
    std::string getCombinationPolicy() const;

    /** Sets combination policy.
     *
     * Sets the combination policy used by predictor to combine
     * predictions returned by the active predictive plugins into one
     * prediction.
     * 
     * The existing combiner object is first destroyed, then a new
     * combiner object created.
     *
     * @param policy combination policy
     */
    void setCombinationPolicy(const std::string policy);

  private:
    // PLUMP
    //plump::Plump& plump;


    // execute plugin function (invoked in thread)
    void *execute(void *);


    Configuration*  config;
    ContextTracker* contextTracker;

    Logger<char>    logger;

    Combiner*       combiner;
    std::string     combinationPolicy;

    int max_partial_prediction_size;

    std::vector<Plugin*> plugins;        // active Plugins
    std::vector<Prediction> predictions; // predictions computed by each plugin are returned here

    int PREDICT_TIME;

};

#endif // PRESAGE_PREDICTOR
