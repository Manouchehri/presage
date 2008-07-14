
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


#ifndef PRESAGE_MERITOCRACYCOMBINER
#define PRESAGE_MERITOCRACYCOMBINER

#include "combiner.h"

/** MeritocracyCombiner.
 *
 * Meritocracy is a system of government or other organization based
 * on demonstrated ability (merit) and talent rather than by wealth
 * (plutocracy), family connections (nepotism), class privilege,
 * cronyism, popularity (as in democracy) or other historical
 * determinants of social position and political power.
 *
 */
class MeritocracyCombiner : public Combiner {
public:
    MeritocracyCombiner();
    virtual ~MeritocracyCombiner();
    virtual Prediction combine(const std::vector< Prediction >&);
private:

};


#endif // PRESAGE_MERITOCRACYCOMBINER
