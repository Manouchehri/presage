
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


#ifndef PRESAGE_CONTEXTCHANGEDETECTOR
#define PRESAGE_CONTEXTCHANGEDETECTOR

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "../configuration.h"

class ContextChangeDetector {
public:
    ContextChangeDetector(const std::string,
			  const std::string,
			  const std::string,
			  const std::string,
			  bool);
    ~ContextChangeDetector();

    bool context_change(const std::string& past_stream) const;
    std::string change(const std::string& past_stream) const;

    std::string get_sliding_window() const;

    void set_sliding_window_size(const std::string& str);
    void update_sliding_window(const std::string& str);
    
private:
    bool context_change_helper(const std::string& str1, const std::string& str2) const;

    static const std::string::size_type DEFAULT_SLIDING_WINDOW_SIZE;
    std::string::size_type              SLIDING_WINDOW_SIZE;
    std::string                         sliding_window;

    const std::string wordChars;
    const std::string separatorChars;
    const std::string blankspaceChars;
    const std::string controlChars;
    bool lowercase_mode;

};

#endif // PRESAGE_CONTEXTCHANGEDETECTOR
