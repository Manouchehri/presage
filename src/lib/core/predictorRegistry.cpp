
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

#ifdef USE_SQLITE
# include "predictors/smoothedNgramPredictor.h"
#endif
#include "predictors/ARPAPredictor.h"
#include "predictors/abbreviationExpansionPredictor.h"
#include "predictors/dummyPredictor.h"
#include "predictors/dictionaryPredictor.h"
#include "predictors/recencyPredictor.h"
#include "predictors/dejavuPredictor.h"

#include <set>
#include <algorithm>

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
    if (contextTracker != ct)
    {
	contextTracker = ct;
	setPredictors (predictors_list);
    }
}

void PredictorRegistry::setPredictors(const std::string& predictorList)
{
    predictors_list = predictorList;
    logger << INFO << "PREDICTORS: " << predictors_list << endl;

    if (contextTracker) {
	// predictors need tracker, only initialize them if available

	// build set of names of active predictors
	std::set<std::string> active_predictors;
	for (std::vector<Predictor*>::const_iterator it = predictors.begin();
	     it != predictors.end();
	     it++)
	{
	    active_predictors.insert((*it)->getName());
	}

	// build set of names of desired predictors
	std::set<std::string> desired_predictors;
        std::stringstream ss(predictors_list);
        std::string predictor;
	while (ss >> predictor)
	{
	    desired_predictors.insert(predictor);
	}

	// build set of names of predictors to remove
	std::set<std::string> predictors_to_remove;
	std::set_difference(active_predictors.begin(), active_predictors.end(),
			    desired_predictors.begin(), desired_predictors.end(),
			    std::inserter(predictors_to_remove, predictors_to_remove.begin()));

	// build set of names of predictors to add
	std::set<std::string> predictors_to_add;
	std::set_difference(desired_predictors.begin(), desired_predictors.end(),
			    active_predictors.begin(), active_predictors.end(),
			    std::inserter(predictors_to_add, predictors_to_add.begin()));

	// remove predictors
	for (std::set<std::string>::const_iterator it = predictors_to_remove.begin();
	     it != predictors_to_remove.end();
	     it++)
	{
	    removePredictor(*it);
	}

	// add predictors
	for (std::set<std::string>::const_iterator it = predictors_to_add.begin();
	     it != predictors_to_add.end();
	     it++)
	{
	    addPredictor(*it);
	}
    }
}

void PredictorRegistry::addPredictor(const std::string& predictorName)
{
    Predictor*  predictor = 0;
    const char* name = predictorName.c_str();
    std::string predictor_class_variable_key = "Presage.Predictors." + predictorName + ".PREDICTOR";
    Variable*   predictor_class_variable = 0;

    // TODO: this will have to do for now, until a proper predictor
    // framework (i.e. plump) is integrated into presage. Until then,
    // all known predictors have to be listed here and explicitly
    // created based on their name.
    //

    try
    {
	predictor_class_variable = config->find (predictor_class_variable_key);

	std::string predictor_class = predictor_class_variable->get_value();

	if (predictor_class == "AbbreviationExpansionPredictor")
	{
	    predictor = new AbbreviationExpansionPredictor(config, contextTracker, name);
	}
	else if (predictor_class == "DummyPredictor") 
	{
	    predictor = new DummyPredictor(config, contextTracker, name);
	}
	else if (predictor_class == "DictionaryPredictor" )
	{
	    predictor = new DictionaryPredictor(config, contextTracker, name);
#ifdef USE_SQLITE
	}
	else if (predictor_class == "SmoothedNgramPredictor") 
	{
	    predictor = new SmoothedNgramPredictor(config, contextTracker, name);
#endif
	}
	else if (predictor_class == "RecencyPredictor") 
	{
	    predictor = new RecencyPredictor(config, contextTracker, name);
	}
	else if (predictor_class == "DejavuPredictor")
	{
	    predictor = new DejavuPredictor(config, contextTracker, name);
	}
	else if (predictor_class == "ARPAPredictor") 
	{
	    predictor = new ARPAPredictor(config, contextTracker, name);
	}
	else
	{
	    logger << ERROR <<  predictor_class_variable_key << " class \""
		   << predictor_class << "\" is unknown." << endl;
	}
    }
    catch (PresageException ex) 
    {
	logger << ERROR << ex.what() << endl
	       << ERROR << "Predictor " + predictorName + " failed to initialize." << endl;
    }

    if (predictor != 0) 
    {
	predictors.push_back (predictor);
	logger << INFO << "Activated predictor: " << predictorName << endl;
    }
    else
    {
	logger << FATAL << "Unable to initialize predictor: " << predictorName << endl;
	throw PredictorRegistryException(PRESAGE_INIT_PREDICTOR_ERROR, "Unable to initialize predictor: " + predictorName);
    }
}

void PredictorRegistry::removePredictor(const std::string& predictor_name)
{
    logger << DEBUG << "Removing predictor: " << predictor_name << endl;
    std::vector<Predictor*>::iterator it = predictors.begin();
    while (it != predictors.end())
    {
	if ((*it)->getName() == predictor_name)
	{
	    delete *it;
	    it = predictors.erase(it);
	    logger << DEBUG << "Removed predictor: " << predictor_name << endl;
	}
	else
	{
	    ++it;
	}
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
    ++iter_curr;
    return result;
}

void PredictorRegistry::update (const Observable* variable)
{
    logger << DEBUG << "About to invoke dispatcher: " << variable->get_name () << " - " << variable->get_value() << endl;

    dispatcher.dispatch (variable);
}
