
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


#ifndef PRESAGE_DISPATCHER
#define PRESAGE_DISPATCHER

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <map>
#include <string>
#include <iostream>

#include "observable.h"

/** Dispatches observable notifications.
 *
 */
template <class class_t>
class Dispatcher {
public:
    typedef void (class_t::* mbr_func_ptr_t) (const std::string& value);
    typedef std::map<std::string, mbr_func_ptr_t> dispatch_map_t;

    Dispatcher(class_t* obj)
    {
	object = obj;
    }

    ~Dispatcher()
    {
	for (std::list<Observable*>::iterator it = observables.begin();
	     it != observables.end();
	     it++) {
	    (*it)->detach (object);
	}

    }

    void map (Observable* var, const mbr_func_ptr_t& ptr)
    {
	var->attach (object);
	observables.push_back (var);
	dispatch_map[var->get_name ()] = ptr;
	dispatch (var);

	//std::cerr << "[Dispatcher] mapped " << var->string() << " => " 
	//	  << object << "->" << ptr << std::endl;
    }

    void dispatch (const Observable* var) 
    {
	mbr_func_ptr_t handler_ptr = dispatch_map[var->get_name ()];
	if (handler_ptr) {
	    ((object)->*(handler_ptr)) (var->get_value ());
	} else {
	    std::cerr << "[Dispatcher] Unable to handle notification from observable: "
		      << var->get_name () << " - " << var->get_value() << std::endl;
	}
    }

private:
    class_t* object;
    dispatch_map_t dispatch_map;
    std::list<Observable*> observables;

};

#endif // PRESAGE_DISPATCHER
