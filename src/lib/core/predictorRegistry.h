
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


#ifndef PRESAGE_PREDICTORREGISTRY
#define PRESAGE_PREDICTORREGISTRY

#include "dispatcher.h"
#include "../predictors/predictor.h"

class ContextTracker;

/** Manages instantiation and iteration through predictors and aids in generating predictions and learning.
 *
 * PredictorRegitry class holds the active predictors and provides the
 * interface required to obtain an iterator to the predictors.
 * 
 * The standard use case is: Predictor obtains an iterator from
 * PredictorRegistry and invokes the predict() or learn() method on each
 * Predictor pointed to by the iterator.
 * 
 * Predictor registry should eventually just be a simple wrapper around
 * plump.
 * 
 */
class PredictorRegistry : public Observer {
public:
    PredictorRegistry(Configuration* config);
    ~PredictorRegistry();

    virtual void update (const Observable* variable);

    class Iterator {
    public:
	Iterator(std::vector<Predictor*>&);
	~Iterator();

	bool hasNext() const;
	Predictor* next();
	
    private:
	std::vector<Predictor*>::iterator iter_end;
	std::vector<Predictor*>::iterator iter_curr;

    };

    Iterator iterator();
    void setContextTracker(ContextTracker* ct);

    class PredictorRegistryException : public PresageException {
    public:
	PredictorRegistryException(presage_error_code_t code, const std::string& desc) throw() : PresageException(code, desc) { }
	virtual ~PredictorRegistryException() throw() { }

    private:
	PredictorRegistryException() throw() : PresageException(PRESAGE_ERROR, "") { }

    };

    static const char* LOGGER;
    static const char* PREDICTORS;

private:
    void setLogger (const std::string& level);
    void setPredictors(const std::string& predictor_list);
    void addPredictor(const std::string& predictor_name);
    void removePredictor(const std::string& predictor_name);
    void removePredictors();

    Configuration*  config;
    ContextTracker* contextTracker;
    Logger<char>    logger;

    std::string             predictors_list;
    std::vector<Predictor*> predictors;        // active Predictors

    Dispatcher<PredictorRegistry> dispatcher;
};

#endif // PRESAGE_PREDICTORREGISTRY
