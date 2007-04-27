
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>            *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.*
 *                                                                           *
\*****************************************************************************/


#ifndef SOOTH_SIMULATOR
#define SOOTH_SIMULATOR

#include "soothsayer.h"

/** Evaluates how good Soothsayer is at doing its job (in other words, its ability to reduce keystrokes the user is required to type).
 *
 * Simulator computes the KSR (Keystrokes Savings Rate) obtained by
 * using Soothsayer preditive ability compared to manually entering
 * text.
 * 
 * The KSR is computed as ( 1 - ( ki + ks ) / kn ) * 100, where
 * ki is the number of input characters actually typed
 * ks is the number of keystrokes needed to select among the predictions presented
 * kn is the number of keystrokes that would be needed if the whole text was typed without any prediction aid.
 *
 * The simulator is built with a reference to a Soothsayer object
 * which simulates and takes a string holding the text to be run
 * through the simulation.  Multiple calls to the simulate method can
 * take place. Each call runs the string through the simulator. The
 * results of each simulate call update the internal state of the
 * simulator object.  The results of the simulation can be retrieved
 * at any time by invoking the results method or the get methods.
 *
 */
class Simulator {
public:
    Simulator();
    ~Simulator();
	
    void simulate( std::string );
    void reset();
    void results() const;
    int getKi() const;
    int getKs() const;
    int getKn() const;
    double getKSR() const;

    bool getAutoSpace() const;
    void setAutoSpace( bool );

    void setKs( int );

private:
    Soothsayer* soothsayerPtr;

    bool find( const std::vector<std::string>&, const std::string& ) const;

    bool autoSpace; // determines whether predictive system is able to insert a space at the end of the word

    int ki;
    int ks;
    int kn;

};


#endif // SOOTH_SIMULATOR
