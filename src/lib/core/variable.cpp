
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

// REVISIT: maybe remove this ctr?
Variable::Variable()
{
    // nothing to do
}

Variable::Variable(const char* variable)
{
    m_variable = string_to_variable(variable);
}

Variable::Variable(const std::string& variable)
{
    m_variable = string_to_variable(variable);
}

Variable::Variable(const std::vector<std::string>& variable)
{
    m_variable = variable;
}

Variable::~Variable()
{
    // nothing to do
}
    
std::string Variable::string() const
{
    return variable_to_string(m_variable);
}

std::vector<std::string> Variable::variable() const
{
    return m_variable;
}

/**
 * Tokenize string on '.' char
 *
 * foo.bar.foobar
 *
 * |foo|bar|foobar|
 *
 */
std::vector<std::string> Variable::string_to_variable(const std::string& str) const
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

/*
    std::string::size_type start_pos = 0;
    std::string::size_type end_pos = str.find_first_of(SEPARATOR);
    while (start_pos != std::string::npos && end_pos != std::string::npos) {
	result.push_back(str.substr(start_pos, end_pos - start_pos));
	start_pos = end_pos + 1;
	end_pos = str.find_first_of(SEPARATOR, start_pos);
    }
*/

    // DEBUG
    // std::cout << "string_to_variable():" << std::endl
    // 	      << "string  : " << str << std::endl
    // 	      << "variable: ";
    // for (size_t i = 0; i < result.size(); i++) {
    // 	std::cout << result[i];
    // 	if (i < result.size() - 1) {
    // 	    std::cout << '.';
    // 	}
    // }
    // std::cout << "| variable size: " << result.size() << std::endl;
    // std::cout << std::endl;
    // DEBUG

    return result;
}

std::string Variable::variable_to_string(const std::vector<std::string>& variable) const
{
    std::string result;
    for (size_t i = 0; i < variable.size(); i++) {
        result += variable[i];
        if (i < variable.size() - 1) {
            result += '.';
        }
    }

    // DEBUG
    // std::cout << "variable_to_string():" << std::endl
    // 	      << "variable: ";
    // for (size_t i = 0; i < variable.size(); i++) {
    // 	std::cout << variable[i];
    // 	if (i < variable.size() - 1) {
    // 	    std::cout << '.';
    // 	}
    // }
    // std::cout << "| variable size: " << variable.size() << std::endl;
    // std::cout << "string  : " << result << std::endl;
    // DEBUG

    return result;
}
