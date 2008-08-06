
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

#include "core/variable.h"
#include "presageException.h"

typedef std::string Value;

/** Configuration allows to query and modify all presage runtime configuration variables.
 *
 */
class Configuration {
public:
    Configuration();
    ~Configuration();

    Value get(const Variable& variable) const;
    void set(const Variable& variable, const Value& value);

    Value operator[](const Variable& variable) const;
    //Value& operator[](const Variable& variable);

    void print() const;

    class ConfigurationException : public PresageException {
    public:
	ConfigurationException(const std::string& desc) throw() : PresageException(desc) { }
	virtual ~ConfigurationException() throw() { }

    private:
	ConfigurationException() throw() : PresageException("") { }

    };

private:
    std::map<Variable, Value>* configuration;

};

#endif // PRESAGE_CONFIGURATION
