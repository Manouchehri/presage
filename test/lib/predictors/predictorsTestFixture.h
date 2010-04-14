
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


#ifndef PRESAGE_PREDICTORSTESTFIXTURE
#define PRESAGE_PREDICTORSTESTFIXTURE

#include <cppunit/extensions/HelperMacros.h>

#include "predictorsTestMockObjects.h"


class PredictorsTestFixture : public CppUnit::TestFixture {
  public:
    PredictorsTestFixture();
    virtual ~PredictorsTestFixture();

    virtual void setUp()    = 0;
    virtual void tearDown() = 0;

    /** \brief Returns prediction computed by predictor.predict() method
     *         using given config and profile.
     *
     * \param config configuration used by predictor.
     * \param history current context used by predictor.
     * \param maximum partial prediction size (default 100).
     * \return prediction computed by predictor method predict().
     */
    virtual Prediction runPredict(const char** config,
				  const char** history,
				  const int max_partial_prediction_size = 100) const;

  private:
    /** \brief Converts array of configuration values config to a
     *         configuration map.
     *
     * \note To be overridden by actual test class inheriting from *this.
     *
     * \param config array of configuration variables and values.
     * \return cocnfiguration map.
     */
    virtual Configuration* prepareConfiguration(const char* config[])  const = 0;

    /** \brief Creates a predictor instance for testing.
     *
     * \note To be overridden by actual test class inheriting from *this.
     * 
     * \param config configuration
     * \param ct context tracker
     * \return instance of predictor to be tested
     */
    virtual Predictor* createPredictor(Configuration* config, ContextTracker* ct) const = 0;

};

#endif // PRESAGE_PREDICTORSTESTFIXTURE
