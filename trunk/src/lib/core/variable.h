
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


#ifndef PRESAGE_VARIABLE
#define PRESAGE_VARIABLE

#include <string>
#include <vector>

#include "observable.h"

class Variable : public Observable {
public:
    Variable(const char* variable);
    Variable(const std::string& variable);
    Variable(const std::vector<std::string>& variable);
    ~Variable();

    std::string              get_name () const;
    std::vector<std::string> get_name_vector () const;

    std::string get_value () const;
    void        set_value (std::string value);
    
    size_t size() const { return m_name_vector.size(); }

    bool operator<(const Variable& other) const { return (get_name () < other.get_name ()); }

    static std::vector<std::string> string_to_vector(const std::string& str);
    static std::string vector_to_string(const std::vector<std::string>& var);

private:
    
    std::string              m_name;
    std::vector<std::string> m_name_vector;
    std::string              m_value;

};

#endif // PRESAGE_VARIABLE
