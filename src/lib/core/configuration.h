
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


#ifndef PRESAGE_CONFIGURATION
#define PRESAGE_CONFIGURATION

#include <map>

#include "variable.h"
#include "../presageException.h"

/** Configuration allows to query and modify all presage runtime configuration variables.
 *
 */
class Configuration {
public:
    Configuration();
    ~Configuration();

    Variable* find (const std::string& variable) const;
    void insert (const std::string& variable, const std::string& value);
    void remove (const std::string& variable);

    Variable* operator[](const std::string& variable) const;

    void print() const;

    std::map<std::string, Variable*>::const_iterator begin () const;
    std::map<std::string, Variable*>::const_iterator end   () const;

    class ConfigurationException : public PresageException {
    public:
	ConfigurationException(presage_error_code_t error_code, const std::string& desc) throw() : PresageException(error_code, desc) { }
	virtual ~ConfigurationException() throw() { }

    private:
	ConfigurationException() throw() : PresageException (PRESAGE_ERROR, "") { }

    };

private:
    std::map<std::string, Variable*>* configuration;

};

#endif // PRESAGE_CONFIGURATION
