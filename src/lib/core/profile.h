
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


#ifndef PRESAGE_PROFILE
#define PRESAGE_PROFILE

#include "config.h"

#include <tinyxml.h>

#include <vector>
#include <string>
#include <map>

typedef std::string Value;
#include "configuration.h"
#include "presageException.h"

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
     * \param filename where profile is saved to
     */
    Profile(const std::string& filename);

    /** Profile destructor.
     *
     * Destructor deallocates the Configuration object passed in costructor.
     *
     */
    virtual ~Profile();

    /** Writes configuration from XML DOM document into configuration.
     * 
     */
    void read_into_configuration(Configuration* configuration);

    /* Reads from configuration and writes to XML DOM document.
     *
     */
    void read_from_configuration (Configuration* configuration);

    bool file_read_ok () const;

    bool write_to_file () const;

    class ProfileException : public PresageException {
    public:
	ProfileException(presage_error_code_t code, const std::string& desc) throw() : PresageException(code, desc) { }
	virtual ~ProfileException() throw() { }
	
    private:
	ProfileException() throw() : PresageException(PRESAGE_ERROR, "") { }

    };

protected:
    void init_configuration(Configuration* config, TiXmlDocument* node);
    void visit_node(Configuration* config, TiXmlNode* node, std::vector<std::string> variable);

    TiXmlDocument* xmlProfileDoc;
    std::string xml_filename;
    bool xml_profile_read_ok;
};

#endif // PRESAGE_PROFILE
