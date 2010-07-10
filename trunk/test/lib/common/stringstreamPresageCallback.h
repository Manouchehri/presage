
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


#ifndef STRINGSTREAM_PRESAGE_CALLBACK
#define STRINGSTREAM_PRESAGE_CALLBACK

#include "presageCallback.h"

/** Stringstream based callback implementation.
 *
 * This concrete callback object implementation accesses a
 * caller-provided std::stringstream object. It is provided here as an
 * example concrete implementation.
 *
 */
class StringstreamPresageCallback : public PresageCallback {
public:
    StringstreamPresageCallback(std::stringstream& stream) : m_stream(stream) { }
    ~StringstreamPresageCallback() { }
    
    std::string get_past_stream() const { if (&m_stream) { return m_stream.str(); } else { return m_empty; } }
    std::string get_future_stream() const { return m_empty; }
    
private:
    std::stringstream& m_stream;
    const std::string m_empty;
};

#endif
