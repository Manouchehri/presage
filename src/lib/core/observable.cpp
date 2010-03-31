
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


#include "observable.h"

#include <iostream>

Observable::~Observable ()
{
    // complete
}
  
void Observable::attach (Observer* observer)
{
    observers.push_back (observer);
}

void Observable::detach (Observer* observer)
{
    observers.remove (observer);
}

void Observable::notify ()
{
    for (std::list <Observer*>::iterator it = observers.begin ();
	 it != observers.end ();
	 it++) {
	//std::cerr << "notify() observable: " << *it << std::endl;
	(*it)->update (this);
    }
}

Observable::Observable ()
{
    // complete
}


