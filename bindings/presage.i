
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


/* Build instructions

   swig -c++ -python -I../src presage.i
   g++ -c -fpic presage_wrap.cxx ../src/presage.cpp -I/usr/include/python2.4 -I/usr/lib/python2.4/config -I../src
   g++ -shared presage.o presage_wrap.o -L../../build/src/.libs -lpresage -o _presage.so

   # Test with
   python runme.py

 */


%module(directors="1") presage

%include "std_vector.i"
%include "std_string.i"

%feature("director") PresageCallback;

%{
#include "presage.h"
#include "presageException.h"
#include "presageCallback.h"
%}

namespace std {
    %template() vector< string >;
}

/* Let's just grab the original header file here */
%include "presage.h"
%include "presageException.h"
%include "presageCallback.h"
