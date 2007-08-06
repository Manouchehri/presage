
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


#ifndef SOOTH_PLUGINSTESTFIXTURE
#define SOOTH_PLUGINSTESTFIXTURE

#include <cppunit/extensions/HelperMacros.h>

#include "pluginsTestMockObjects.h"


class PluginsTestFixture : public CppUnit::TestFixture {
  public:
    PluginsTestFixture();
    virtual ~PluginsTestFixture();

    virtual void setUp()    = 0;
    virtual void tearDown() = 0;

    /** \brief Returns prediction computed by plugin.predict() method
     *         using given config and profile.
     *
     * \param config configuration used by predictive plugin.
     * \param history current context used by predictive plugin.
     * \return prediction computed by plugin method predict().
     */
    virtual Prediction runPredict(const char** config, const char** history) const;

  private:
    /** \brief Converts array of configuration values config to a
     *         configuration map.
     *
     * \note To be overridden by actual test class inheriting from *this.
     *
     * \param config array of configuration variables and values.
     * \return cocnfiguration map.
     */
    virtual ConfigMap  prepareConfigMap(const char* config[])  const = 0;
    /** \brief Converts array of configuration values config to a
     *         configuration map.
     *
     * \note To be overridden by actual test class inheriting from *this.
     * 
     * \param profile profile
     * \param ct context tracker
     * \return instance of plugin to be tested
     */
    virtual Plugin* createPlugin(Profile* profile, ContextTracker* ct) const = 0;

};

#endif // SOOTH_PLUGINSTESTFIXTURE
