
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

#ifndef SOOTH_PREDICTOR
#define SOOTH_PREDICTOR

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

// PLUMP
// REVISIT uncomment this when reenabling plump
//#include "plump/src/plump.h"
//namespace plump { typedef int Plump; }

#include "core/historyTracker.h"
#include "core/prediction.h"
#include "core/profile.h"
#include "core/combiner.h"

#include "plugins/plugin.h"

#ifdef STDC_HEADERS
# include <stdlib.h>  // needed for abort function
#endif

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

    /** Construct a predictor object.
     *
     *  Predictor needs a reference to the HistoryTracker object to
     *  forward to the predictive plugins for context retrieval and
     *  analysis.
     *
     *  @param ht is a reference to HistoryTracker
     */
    Predictor(HistoryTracker&);

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
    Prediction predict() const;


    /** Adds a Plugin object to the active runtime plugins list.
     *
     * @param pPtr is a pointer to a Plugin
     */
    void addPlugin(Plugin* plugin);

    /** Removes a Plugin object from the active runtime plugins list.
     *
     * @param pPtr is a pointer to the Plugin to be removed
     * @return true if successful, false otherwise
     */
    bool removePlugin(const Plugin* plugin);

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
     *
     * @param milliseconds time in milliseconds
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
    CombinationMethod getCombinationMethod() const;

    /** Sets COMBINATION_METHOD option.
     *
     * Sets the combination method used by predictor to combine
     * predictions returned by the active predictive plugins into one
     * prediction.
     *
     * @param cm method used to combine predictions computed by each multiple predictive plugins into a single prediction
     * @return true if the supplied value is valid, false otherwise
     */
    bool setCombinationMethod(const CombinationMethod);

    /** Set Profile interface to currently active profile.
     * 
     * Profile provides an interface to the active profile. Each
     * component can pull the values of the configuration variables it
     * is interested in from the Profile object, rather than having
     * the configuration variable values pushed down from
     * ProfileManager.
     *
     * Eventually, all components should start pulling their config
     * instead of having the config pushed down their throat.
     *
     * @param profile Profile to use.
     */
    void setProfile(Profile*);

  private:
    // PLUMP
    //plump::Plump& plump;

    /** Initializes the Combiner object.
     *
     *  @param combinationMethod is an enum that identifies what Combiner object to use
     */
    bool initCombiner();

    // execute plugin function (invoked in thread)
    void *execute(void *);


    HistoryTracker &historyTracker;
    Profile* profile;

    Combiner* combiner;
    std::vector<Plugin*> plugins;        // active Plugins
    std::vector<Prediction> predictions; // predictions computed by each plugin are returned here

    int PREDICT_TIME;
    CombinationMethod COMBINATION_METHOD;

};

#endif // SOOTH_PREDICTOR
