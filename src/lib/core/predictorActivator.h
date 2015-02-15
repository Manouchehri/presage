
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


#ifndef PRESAGE_PREDICTORACTIVATOR
#define PRESAGE_PREDICTORACTIVATOR

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "configuration.h"
#include "predictorRegistry.h"
#include "context_tracker/contextTracker.h"
#include "prediction.h"
#include "logger.h"
#include "dispatcher.h"

#include "combiner.h"
#include "meritocracyCombiner.h"

#include "predictors/predictor.h"


#ifdef STDC_HEADERS
# include <stdlib.h>  // needed for abort function
#endif


/** PredictorActivator, the heart of Presage system, coordinates the execution of predictors and returns the combination of their predictions.
 *
 * PredictorActivator starts the execution of the active predictors,
 * monitors their execution and collects the predictions returned, or
 * terminates a predictor's execution if it execedes its maximum
 * prediction time.
 * 
 * The predictions returned by the individual predictors are combined
 * into a single prediction by the active Combiner. (please refer to
 * my thesis for a list of possible conbination strategies.
 *
 */
class PredictorActivator : public Observer {
public:
//PLUMP    PredictorActivator(HistoryTracker&,
//PLUMP              plump::Plump&);

    /** Construct a PredictorActivator object.
     *
     *  PredictorActivator needs a reference to the ContextTracker
     *  object to forward to the predictors for context retrieval and
     *  analysis.
     *
     * @param config pointer to configuration
     * @param registry pointer to predictor registry
     * @param contextTracker pointer to ContextTracker
     */
    PredictorActivator(Configuration* config, PredictorRegistry* registry, ContextTracker* contextTracker);

    /** Destroy predictor activator.
     *
     */
    ~PredictorActivator();

    /** Runs the predictors, combine their predictions and return the resulting prediction.
     *
     * This is the heart of Presage.
     * 
     * Plump will eventually provide the implementation of sequential or parallel execution of predictors.
     *
     * @return prediction produced by the active predictors and combined by the active combiner
     */
    Prediction predict(unsigned int multiplier, const char** filter);

    /** Gets PREDICT_TIME option value.
     *
     * Returns the maximum time predictors are allowed to execute before
     * returning a prediction.
     *
     * @return value of PREDICT_TIME
     */
    int getPredictTime() const;
    
    /** Sets PREDICT_TIME option, the maximum time allowed for a predictor to return its prediction.
     *
     * @param predictTime expressed in milliseconds
     * @return true if the supplied value is valid, false otherwise
     */
    void setPredictTime(const std::string& predictTime);

    /** Gets COMBINATION_METHOD option value.
     *
     * Returns the active combination method used by predictor
     * activator to combine predictions returned by the active
     * predictors into one prediction.
     *
     * @return value of COMBINATION_METHOD
     */
    std::string getCombinationPolicy() const;

    /** Sets combination policy.
     *
     * Sets the combination policy used by predictor activator to
     * combine predictions returned by the active predictors into one
     * prediction.
     * 
     * The existing combiner object is first destroyed, then a new
     * combiner object created.
     *
     * @param policy combination policy
     */
    void setCombinationPolicy(const std::string& policy);

    /** Sets maximum partial prediction size.
     *
     * @param size maximum partial prediction size
     */
    void setMaxPartialPredictionSize (const std::string& size);

    /** Sets logger level.
     *
     * @param level logger level
     */
    void setLogger (const std::string& level);

    void parse_internal_commands (Prediction& pred);

    virtual void update (const Observable* variable);

    static const char* LOGGER;
    static const char* PREDICT_TIME;
    static const char* MAX_PARTIAL_PREDICTION_SIZE;
    static const char* COMBINATION_POLICY;

private:
    // execute predictor function (invoked in thread)
    void *execute(void *);


    Configuration*  config;
    PredictorRegistry* predictorRegistry;
    ContextTracker* contextTracker;

    Logger<char>    logger;

    Combiner*       combiner;
    std::string     combinationPolicy;

    int max_partial_prediction_size;

    std::vector<Prediction> predictions; // predictions computed by each predictor are returned here

    int predict_time;

    Dispatcher<PredictorActivator> dispatcher;
};

#endif // PRESAGE_PREDICTORACTIVATOR
