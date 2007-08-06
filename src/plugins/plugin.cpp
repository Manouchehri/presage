
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>           *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.*
 *                                                                           *
\*****************************************************************************/


#include "plugins/plugin.h"

/** Creates a plugin object.
 *
 * @param prof pointer to profile
 * @param ct is a reference to HistoryTracker
 * @param pluginName is the plugin name
 * @param shortDesc is a short description of the plugin's functionality
 * @param longDesc is a long description of the plugin's functionality
 */
Plugin::Plugin(Profile* prof,
	       ContextTracker* ct,
	       const char* pluginName,
	       const char* shortDesc,
	       const char* longDesc)
	: name            (pluginName),
	  shortDescription(shortDesc ),
	  longDescription (longDesc  ),
	  contextTracker  (ct        ),
	  profile         (prof      )
{
    // NOTE: plugin implementations deriving from this class should
    // use profile to query the value of needed configuration
    // variables.
}

/** Virtual plugin destructor.
 *
 */
Plugin::~Plugin()
{}

/** Get plugin name.
 *
 */
const std::string Plugin::getName() const
{
	return name;
}

/** Get plugin short description.
 *
 */
const std::string Plugin::getShortDescription() const
{
	return shortDescription;
}

/** Get plugin long description.
 *
 */
const std::string Plugin::getLongDescription() const
{
	return longDescription;
}

/*
void Plugin::setLibFilename(const std::string s)
{
	libFilename = s;
}

std::string Plugin::getLibFilename() const
{
	return libFilename;
}

void Plugin::setLibHandle( lt_dlhandle handle)
{
	libHandle = handle;
}

lt_dlhandle Plugin::getLibHandle() const
{
	return libHandle;
}
*/


/** Convert string to double.
 *
 */
double Plugin::toDouble(const std::string str) const
{
    return atof(str.c_str());
}

/** Convert string to int.
 *
 */
int Plugin::toInt(const std::string str) const
{
    return atoi(str.c_str());
}
