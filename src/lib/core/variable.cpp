
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


#include "core/variable.h"

#include <iostream>

Variable::Variable(const char* name)
{
    m_name = name;
    m_name_vector = string_to_vector (name);
}

Variable::Variable(const std::string& name)
{
    m_name = name;
    m_name_vector = string_to_vector(name);
}

Variable::Variable(const std::vector<std::string>& name)
{
    m_name = vector_to_string (name);
    m_name_vector = name;
}

Variable::~Variable()
{
    // nothing to do
}

std::string Variable::get_name () const
{
    return m_name;
}

std::vector<std::string> Variable::get_name_vector () const
{
    return m_name_vector;
}

std::string Variable::get_value () const
{
    return m_value;
}

void Variable::set_value (std::string value)
{
    m_value = value;
    
    notify ();     // notify all observers
}

/**
 * Tokenize string on '.' char
 *
 * foo.bar.foobar
 *
 * |foo|bar|foobar|
 *
 */
std::vector<std::string> Variable::string_to_vector(const std::string& str)
{
    const char SEPARATOR = '.';
    
    std::vector<std::string> result;

    size_t length = str.size();
    size_t i = 0;
    std::string acc;
    while (i < length) {
	if (str[i] == SEPARATOR) {
	    result.push_back(acc);
	    acc.clear();
	} else {
	    acc += str[i];
	}
	i++;
    }
    if (!acc.empty()) {
	result.push_back(acc);
    }

    return result;
}

std::string Variable::vector_to_string(const std::vector<std::string>& variable)
{
    std::string result;
    for (size_t i = 0; i < variable.size(); i++) {
        result += variable[i];
        if (i < variable.size() - 1) {
            result += '.';
        }
    }

    return result;
}
