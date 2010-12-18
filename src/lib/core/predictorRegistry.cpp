
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


#include "predictorRegistry.h"

// REVISIT: including predictors here because Plump is temporarily
// disabled. Predictor lifetime management is currently disabled.
//
// IMPORTANT: remove following includes when moving back to dynamically
// loaded predictors.
//
#ifdef USE_SQLITE
# include "predictors/smoothedNgramPredictor.h"
#endif
#include "predictors/ARPAPredictor.h"
#include "predictors/abbreviationExpansionPredictor.h"
#include "predictors/dummyPredictor.h"
#include "predictors/dictionaryPredictor.h"
#include "predictors/recencyPredictor.h"
#include "predictors/dejavuPredictor.h"

const char* PredictorRegistry::LOGGER  = "Presage.PredictorRegistry.LOGGER";
const char* PredictorRegistry::PREDICTORS = "Presage.PredictorRegistry.PREDICTORS";

PredictorRegistry::PredictorRegistry(Configuration* configuration)
    : config(configuration),
      contextTracker(0),
      logger("PredictorRegistry", std::cerr),
      dispatcher(this)
{
    // build notification dispatch map
    dispatcher.map (config->find (LOGGER), & PredictorRegistry::setLogger);
    dispatcher.map (config->find (PREDICTORS), & PredictorRegistry::setPredictors);
}


PredictorRegistry::~PredictorRegistry()
{
    removePredictors();
}

void PredictorRegistry::setLogger (const std::string& value)
{
    logger << setlevel (value);
    logger << INFO << "LOGGER: " << value << endl;
}


void PredictorRegistry::setContextTracker(ContextTracker* ct) {
    if (contextTracker != ct) {
	contextTracker = ct;
	removePredictors ();
	setPredictors (predictors_list);
    }
}

void PredictorRegistry::setPredictors(const std::string& predictorList)
{
    predictors_list = predictorList;
    logger << INFO << "PREDICTORS: " << predictors_list << endl;

    if (contextTracker) {
	// predictors need tracker, only initialize them if available

        removePredictors();

        std::stringstream ss(predictors_list);
        std::string predictor;
	while (ss >> predictor) {
	    logger << INFO << "Initializing predictive predictor: " << predictor << endl;
	    addPredictor(predictor);
	}
    }
}

void PredictorRegistry::addPredictor(const std::string& predictorName)
{
    Predictor* predictor = 0;
    try {
	// TODO: this will have to do for now, until a proper predictor
	// framework (i.e. plump) is integrated into presage. Until
	// then, all known predictors have to be listed here and explicitly
	// created based on their name.
	//
	if (predictorName == "AbbreviationExpansionPredictor") {
	    predictor = new AbbreviationExpansionPredictor(config, contextTracker);
	} else if (predictorName == "DummyPredictor") {
	    predictor = new DummyPredictor(config, contextTracker);
	} else if (predictorName == "DictionaryPredictor" ) {
	    predictor = new DictionaryPredictor(config, contextTracker);
#ifdef USE_SQLITE
	} else if (predictorName == "SmoothedNgramPredictor") {
	    predictor = new SmoothedNgramPredictor(config, contextTracker);
#endif
	} else if (predictorName == "RecencyPredictor") {
	    predictor = new RecencyPredictor(config, contextTracker);
	} else if (predictorName == "DejavuPredictor") {
	    predictor = new DejavuPredictor(config, contextTracker);
	} else if (predictorName == "ARPAPredictor") {
	      predictor = new ARPAPredictor(config,contextTracker);
	}
    } catch (PresageException ex) {
	logger << ERROR << "Predictor " + predictorName + " failed to initialize." << endl;
    }

    if (predictor != 0) {
	predictors.push_back (predictor);
	logger << INFO << "Activated predictive predictor: " << predictorName << endl;
    } else {
	// TODO: raise exception
	logger << FATAL << "Unable to initialize predictor: " << predictorName << endl;
	throw PredictorRegistryException(PRESAGE_INIT_PREDICTOR_ERROR, "Unable to initialize predictor: " + predictorName);
    }
}

void PredictorRegistry::removePredictors()
{
    for (size_t i = 0; i < predictors.size(); i++) {
	logger << DEBUG << "Removing predictor: " << predictors[i]->getName() << endl;
	delete predictors[i];
    }
    predictors.clear();
}

PredictorRegistry::Iterator PredictorRegistry::iterator()
{
    return Iterator(predictors);
}


////
// Iterator
PredictorRegistry::Iterator::Iterator(std::vector<Predictor*>& cont)
    : iter_end(cont.end()),
      iter_curr(cont.begin())
{}

PredictorRegistry::Iterator::~Iterator()
{}

bool PredictorRegistry::Iterator::hasNext() const
{
    bool result = (iter_end != iter_curr);

    return result;
}

Predictor* PredictorRegistry::Iterator::next()
{
    Predictor* result = *iter_curr;
    iter_curr++;
    return result;
}

void PredictorRegistry::update (const Observable* variable)
{
    logger << DEBUG << "About to invoke dispatcher: " << variable->get_name () << " - " << variable->get_value() << endl;

    dispatcher.dispatch (variable);
}
