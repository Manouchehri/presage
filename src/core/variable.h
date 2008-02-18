
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

#ifndef SOOTH_VARIABLE
#define SOOTH_VARIABLE

#include <string>
#include <vector>

class Variable {
public:
    Variable();
    Variable(const std::string& variable);
    Variable(const std::vector<std::string>& variable);
    ~Variable();
    
    std::string              string()   const;
    std::vector<std::string> variable() const;

    void push_back(const std::string& str) { m_variable.push_back(str); }
    void pop_back() { m_variable.pop_back(); }
    size_t size() const { return m_variable.size(); }

    bool operator<(const Variable& other) const { return (string() < other.string()); }
//    bool operator>(const Variable& other) const { return (string() > other.string()); }
//    bool operator==(const Variable& other) const { return !((*this) < other) && !((*this) > other); }

private:
    std::vector<std::string> string_to_variable(const std::string& str) const;
    std::string variable_to_string(const std::vector<std::string>& var) const;
    
    std::vector<std::string> m_variable;
    
};

#endif // SOOTH_VARIABLE
