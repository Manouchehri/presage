
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


#include "pluginsTestFixture.h"

PluginsTestFixture::PluginsTestFixture()
{
    // intentionally empty
}

PluginsTestFixture::~PluginsTestFixture()
{
    // intentionally empty
}

Prediction PluginsTestFixture::runPredict(const char** config,
					  const char** history,
					  const int max_partial_prediction_size) const
{
    // convert configuration details into a Configuration object
    // 
    Configuration* configuration = prepareConfiguration(config);

    // The cast to Configuration* is necessary since only the
    // implementation of the mock ContextTracker class is different -
    // the interface cannot be changed.
    //
    // Using argument to pass an array of history tokens.
    // 
    ContextTracker contextTracker((Configuration*) history, 0, 0);

    // creating plugin object to test using the mock ContextTracker
    // and Profile objects.
    Plugin* plugin = createPlugin(configuration, &contextTracker);

    // generate prediction.
    Prediction result = plugin->predict(max_partial_prediction_size, 0);

    // plugin object is no longer needed.
    delete plugin;
    // same for configuration
    delete configuration;

    // return the prediction for comparison with expected results.
    return result;
}
