
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


#include "core/predictor.h"

// REVISIT: including SmoothedUniBiTrigramPlugin here because
// PluginManager and Plump are temporarily disabled. Plugin lifetime
// management is currently disabled. Only one plugin is used:
// SmoothedUniBiTrigramPlugin.
//
// IMPORTANT: remove following include when moving back to dynamically
// loaded plugins
//
#include "plugins/smoothedNgramPlugin.h"
#include "plugins/abbreviationExpansionPlugin.h"
#include "plugins/dummyPlugin.h"
#include "plugins/dictionaryPlugin.h"
#include "plugins/smoothedCountPlugin.h"
#include "plugins/recencyPlugin.h"
#include "plugins/dejavuPlugin.h"

Predictor::Predictor(Configuration* configuration,
		     PluginRegistry* registry,
		     ContextTracker* ct)
    : config(configuration),
      pluginRegistry(registry),
      contextTracker(ct),
      logger("Predictor", std::cerr)
{
    combiner = 0;

    // read config values
    Variable* variable;
    Value value;

    try {
	variable = new Variable("Presage.Predictor.LOGGER");
	value = config->get(*variable);
	logger << setlevel(value);
	logger << INFO << "LOGGER: " << value << endl;
	delete variable;

	variable = new Variable("Presage.Predictor.PREDICT_TIME");
	value = config->get(*variable);
	logger << INFO << "PREDICT_TIME: " << value << endl;
	setPredictTime(toInt(value));
	delete variable;

	variable = new Variable("Presage.Predictor.MAX_PARTIAL_PREDICTION_SIZE");
	value = config->get(*variable);
	logger << INFO << "MAX_PARTIAL_PREDICTION_SIZE: " << value << endl;
	max_partial_prediction_size = toInt(value);
	delete variable;

	variable = new Variable("Presage.Predictor.COMBINATION_POLICY");
	value = config->get(*variable);
	logger << INFO << "COMBINATION_POLICY: " << value << endl;
	setCombinationPolicy(value);
	delete variable;

    } catch (Configuration::ConfigurationException ex) {
	logger << ERROR << "Caught ConfigurationException: " << ex.what() << endl;
    }
}


Predictor::~Predictor()
{
    delete combiner;
}

// PLUMP callback
//void callback_predict(plump::PluginInstance* plugin, void* data)
//{
//    Prediction* p = reinterpret_cast<Prediction*>(data);
//    Plugin* predictivePlugin = reinterpret_cast<Plugin*>(plugin->instance());
//    *p = predictivePlugin->predict();
//}

Prediction Predictor::predict(unsigned int multiplier)
{
    Prediction result;

    // Here goes code to instantiate a separate thread for each Plugin
    //

    // All threads need to be synched together. One thread makes sure that
    // we are not exceeding the maximum time allowed.
    //

    // Now that the all threads have exited or have been cancelled,
    // the predictions returned by each of them are combined.
    //

    // clear out previous predictions
    predictions.clear();

    PluginRegistry::Iterator it = pluginRegistry->iterator();
    Plugin* plugin = 0;
    while (it.hasNext()) {
	plugin = it.next();
	logger << DEBUG << "Invoking predictive plugin: " << plugin->getName() << endl;
	predictions.push_back(plugin->predict(max_partial_prediction_size * multiplier));
    }

    // ...then merge predictions into a single one...
    result = combiner->combine(predictions);

    // ...and return final prediction
    return result;

    ////////
    // PLUMP
    //
    //plump.registerCallback(callback_predict, &p);
    //plump.run();
}


bool Predictor::setPredictTime( const int predictTime )
{
    // handle exception where predictTime is less than zero
    if( predictTime < 0 ) {
        logger << ERROR << "Error: attempted to set PREDICT_TIME option to "
	       << "a negative integer value. Please make sure that "
	       << "PREDICT_TIME option is set to a value greater "
	       << "than or equal to zero.\a" << endl;
        return false;
    } else {
	logger << INFO << "Setting PREDICT_TIME to " << predictTime << endl;
        PREDICT_TIME = predictTime;
        return true;
    }
}


int Predictor::getPredictTime() const
{
    return PREDICT_TIME;
}


void Predictor::setCombinationPolicy(const std::string cp)
{
    logger << INFO << "Setting COMBINATION_POLICY to " << cp << endl;
    delete combiner;
    combinationPolicy = cp;

    std::string policy = strtolower (cp);
    if (policy == "meritocracy") {
	combiner = new MeritocracyCombiner();
    } else {
	// TODO: throw exception
	logger << ERROR << "Error - unknown combination policy: "
	       << cp << endl;
    }
}


std::string Predictor::getCombinationPolicy() const
{
    return combinationPolicy;
}
