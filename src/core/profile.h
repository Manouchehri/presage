
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

#ifndef SOOTH_PROFILE
#define SOOTH_PROFILE

#include <string>
#include <vector>
#include <map>

#include "tinyxml/tinyxml.h"

typedef std::string               Value;
typedef std::vector<std::string>  Variable;
typedef std::map<Variable, Value> Configuration;

/** Profile provides access to the active profile configuration variables.
 *
 * Profile only provides an accessor method to read configuration
 * values. The class is immutable. It is not possible to modify
 * configuration values. Configuration values are set when the Profile
 * is constructed.
 *
 * Profile acts as an interface to ProfileManager. It converts simple
 * requests for configuration variable values from client objects
 * (such as Plugin objects) into requests to ProfileManager.  There is
 * no need to have a map containing the variable, value pairs. It is
 * sufficient to keep a reference to ProfileManager and have
 * ProfileManager expose an interface that Profile can use to query
 * variable values.
 *
 */
class Profile {
public:
    /** Profile constructor.
     *
     * \param xmlProfileDoc Profile takes ownership of the configuration object.
     */
    Profile(TiXmlDocument* xmlProfileDoc);

    /** Profile destructor.
     *
     * Destructor deallocates the Configuration object passed in costructor.
     *
     */
    ~Profile();

    /** Get configuration value associated to configuration variable.
     *
     * \param variable configuration variable
     * \return value associated to variable
     */
    Value getConfig(const Variable& variable);

    class ProfileException {
    public:
	ProfileException() {}
	ProfileException(std::string desc) { details = desc; }
	~ProfileException() {}
	
	std::string what() const { return details; }

    private:
	std::string details;
    };

private:
    void visitNode(TiXmlNode* node, Variable variable);

    void initConfiguration(TiXmlDocument* node);

    std::string stringifyVariable(const Variable& variable) const;

    // debug methods
    void printConfiguration() const;
    void printVariable(const Variable& variable) const;

    Configuration* configuration;
};

#endif // SOOTH_PROFILE
