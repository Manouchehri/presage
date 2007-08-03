
/******************************************************************************\
 *                                                                            *
 * Soothsayer, an extensible predictive text entry system                     *
 * ------------------------------------------------------                     *
 *                                                                            *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>             *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*
 *                                                                            *
\******************************************************************************/        


#include "pluginsTestFixture.h"

PluginsTestFixture::PluginsTestFixture()
{
    // intentionally empty
}

PluginsTestFixture::~PluginsTestFixture()
{
    // intentionally empty
}

Prediction PluginsTestFixture::runPredict(const char** config, const char** history) const
{
    // convert configuration details into an object that the mock
    // Profile object can understand - in this case, an std::map of
    // std::string's.
    ConfigMap configMap = prepareConfigMap(config);

    // pass the configuration map to the mock profile object. The cast
    // to TiXmlDocument* is necessary since only the implementation of
    // the mock Profile class is different - the interface cannot be
    // changed. The mock Profile object's constructor will cast its
    // argument back to ConfigMap* before use.
    Profile profile((TiXmlDocument*) &configMap);

    // similarly, the mock HistoryTracker object's interface is
    // unchanged, therefore casting Profile* argument and using it
    // to pass an array of history tokens.
    HistoryTracker ht((Profile*) history);

    // creating plugin object to test using the mock HistoryTracker
    // and Profile objects.
    Plugin* plugin = createPlugin(&profile, &ht);

    // generate prediction.
    Prediction result = plugin->predict();

    // plugin object is no longer needed.
    delete plugin;

    // return the prediction for comparison with expected results.
    return result;
}
