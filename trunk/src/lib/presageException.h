
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


#ifndef PRESAGE_EXCEPTION
#define PRESAGE_EXCEPTION

#include <exception>
#include <string>

/** When thrown, provides information about an error that has occurred within presage.
 *
 * If an error occurs within presage, an exception is thrown, and
 * this is the object that encapsulates the details of the problem.
 * 
 * Application using presage should always ensure that exceptions
 * are caught by enclosing all presage methods within a try{}
 * catch(PresageException& e) {} block.
 *
 */
class PresageException : public std::exception {
public:
    PresageException(const std::string& msg) throw();
    virtual ~PresageException() throw();
    virtual const char* what() const throw();

private:
    std::string details;

};



#endif // PRESAGE_EXCEPTION
