
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


#ifndef PRESAGE_OBSERVABLE
#define PRESAGE_OBSERVABLE

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "observer.h"

#include <list>
#include <string>

class Observable {
public:
    virtual ~Observable ();
  
    virtual void attach (Observer* observer);
    virtual void detach (Observer* observer);
    virtual void notify ();

    virtual std::string  get_name () const = 0;
    virtual std::string get_value () const = 0;
   
protected:
    Observable ();

private:
    std::list <Observer*> observers;

};

#endif // PRESAGE_OBSERVABLE
