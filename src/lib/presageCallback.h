
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


#ifndef PRESAGE_CALLBACK
#define PRESAGE_CALLBACK

#ifdef __cplusplus
extern "C" {
#endif

    typedef const char* (*_presage_callback_get_past_stream)   (void*);
    typedef const char* (*_presage_callback_get_future_stream) (void*);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#ifndef _MSC_VER

#include <string>
#include <sstream>

/** Abstract callback object used to retrieve context from user application.
 *
 * A concrete callback implementation must implement the
 * getPastStream() and getFutureStream() method which are responsible
 * for retrieving the context from the user text buffer.
 *
 * Using a callback object decouples the user application from the
 * chosen text buffer (which could be a simple string, a graphical
 * text widget, a file stream, etc.) and frees the caller from having
 * to explicitly notify of any updates to the context. Presage will
 * invoke on the callback whenever access to contextual data is
 * required.
 *
 * getPastStream() must return a string containing the text preceding
 * the current insertion point.
 *
 * getFutureStream() must return a string containing the text
 * following the current insertion point.
 *
 */ 
class PresageCallback {
public:
    virtual ~PresageCallback() { };

    virtual std::string get_past_stream() const = 0;
    virtual std::string get_future_stream() const = 0;

protected:
    PresageCallback() { };

};


/** DEPRECATED: simple context callback implementation to ease transition into presage callback refactor.
 *
 * This concrete callback object provides an update() method signature
 * that matches legacy Presage::update(std::string) method and is
 * designed to provide a simple upgrade path from previous presage
 * releases.
 * 
 * Simply create a LegacyPresageCallback object, pass it as argument
 * to the new Presage object, and then replace all calls to
 * Presage::update(std::string) with a
 * LegacyPresageCallback::update(std::string) calls.
 * 
 * WARNING: This class is deprecated and might be removed in future releases.
 *
 */
class LegacyPresageCallback : public PresageCallback {
public:
    LegacyPresageCallback() { }
    ~LegacyPresageCallback() { }
    
    std::string get_past_stream() const { return m_stream; }
    std::string get_future_stream() const { return m_empty; }

    void update(std::string str) { for (size_t sz = 0; sz < str.size(); sz++) { update(str[sz]); } }

private:
    void update(int character) { 
        if (character == '\b'
            && ! m_stream.empty()) {
            m_stream.erase(m_stream.end() - 1);
        } else {
            m_stream.push_back(character);
        }
    }

    std::string m_stream;
    const std::string m_empty;
};

#endif /* _MSC_VER */
#endif /* _cplusplus */

#endif /* PRESAGE_CALLBACK */
