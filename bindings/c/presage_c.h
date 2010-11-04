
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

/*

To build the presage C DLL with MinGW/MSYS:

g++ -shared -o presage_c.dll presage_c.cxx -Wl,--output-def,presage_c.def,--out-implib,libpresage_c.a -I/c/presage/include -L/c/presage/lib -lpresage

 */

#ifndef PRESAGE_C
#define PRESAGE_C

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _presage* presage;

    typedef const char* (*_presage_callback_get_past_stream)  ();
    typedef const char* (*_presage_callback_get_future_stream)();

    presage      presage_new                 (_presage_callback_get_past_stream past,
					      _presage_callback_get_future_stream future);
    
    void         presage_delete              (presage prsg);
    
    void         presage_free_string         (char* str);
    void         presage_free_string_array   (char** str);

    char**       presage_predict             (presage prsg);
    
    char*        presage_completion          (presage prsg,
					      const char* token);

    char*        presage_context             (presage prsg);

    int          presage_context_change      (presage prsg);

    char*        presage_prefix              (presage prsg);

    char*        presage_config              (presage prsg,
					      const char* variable);

    void         presage_config_set          (presage prsg,
					      const char* variable,
					      const char* value);

    void         presage_save_config         (presage prsg);

#ifdef __cplusplus
}
#endif

#endif /* PRESAGE_C */
