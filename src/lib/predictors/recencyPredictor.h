
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


#ifndef PRESAGE_RECENCYPREDICTOR
#define PRESAGE_RECENCYPREDICTOR

#include "predictor.h"
#include "../core/logger.h"
#include "../core/dispatcher.h"

/** Recency predictor, a recency promotion statistical predictor.
 *
 * RecencyPredictor, based on recency promotion principle, generates
 * predictions by assigning exponentially decaying probability values
 * to previously encountered tokens. Tokens are assigned a probability
 * value that decays exponentially with their distance from the
 * current token, thereby promoting context recency

 * (from Wikipedia) A quantity is said to be subject to exponential
 * decay if it decreases at a rate proportional to its
 * value. Symbolically, this can be expressed as the following
 * differential equation, where N is the quantity and λ is a positive
 * number called the decay constant.
 *
 *  frac{dN}{dt} = -lambda N.
 *
 * The solution to this equation is below:
 *
 *  N(t) = N_0 e^{-lambda t}
 *
 * Here N(t) is the quantity at time t, and N0 = N(0) is the (initial)
 * quantity, at time t = 0.
 *
 * This is the form of the equation that is most commonly used to
 * describe exponential decay. The constant of integration N0 denotes
 * the original quantity at t = 0. (The notation λ for the decay
 * constant is a remnant of the usual notation for an eigenvalue. In
 * this case, λ is the eigenvalue of the opposite of the
 * differentiation operator with N(t) as the corresponding
 * eigenfunction).
 *
 */
class RecencyPredictor : public Predictor, public Observer {
public:
    RecencyPredictor (Configuration*, ContextTracker*, const char*);
    ~RecencyPredictor();

    virtual Prediction predict (const size_t size, const char** filter) const;

    virtual void learn (const std::vector<std::string>& change);

    virtual void update (const Observable* variable);

private:
    void set_lambda           (const std::string& value);
    void set_n_0              (const std::string& value);
    void set_cutoff_threshold (const std::string& value);

    std::string LOGGER;
    std::string LAMBDA;
    std::string N_0;
    std::string CUTOFF_THRESHOLD;
    
    double lambda;
    double n_0;
    size_t cutoff_threshold;

    Dispatcher<RecencyPredictor> dispatcher;

};

#endif // PRESAGE_RECENCYPREDICTOR
