
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>           *
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


#include "simulator.h"

#include <iostream>

Simulator::Simulator()
{
    soothsayerPtr = new Soothsayer;

    autoSpace = true;
	
    ki = 0;
    ks = 1;
    kn = 0;

    silent_mode = false;
}


Simulator::~Simulator()
{
    delete soothsayerPtr;
}


void Simulator::simulate( std::string str )
{

    // Soothsayer predicts a word even when the prefix
    // is a null string. This initial call to the predict
    // method simulates this condition.
    bool hit = find( soothsayerPtr->predict( "" ), str );

    // If the correct predicted word is returned, then
    // we've got a hit! If this happens when the prefix is
    // a null string, kudos to the developer (me)! 
    // Soothsayer predicted the word is one try!
    // We need to update the Soothsayer object with the whole
    // string and take the trailing space into account.
    if( hit ) {
	kn += str.size() + 1; 
	ks++;
	soothsayerPtr->update( str + " " );
	if( !autoSpace ) {
	    ki++;
	}
    } else {
	// If we didn't get it right on the first guess,
	// let's guess again until we get a hit or we
	// run out of characters that make up the current
	// word we're trying to predict.
	std::string::iterator i = str.begin();

	while( i != str.end() && !hit ) {
			
	    // predict using new keystroke
	    hit =  find( soothsayerPtr->predict( *i ), str );

	    // simulate character keystroke
	    ki++;
			
	    // position iterator on next character
	    i++;
	}
		
	// If we got a hit, we've got the correct
	// predicted word after having used up some
	// of its characters to feed to Soothsayer.
	if( hit ) {

            soothsayerPtr->complete(str);
	    soothsayerPtr->update( " " );
	    kn += str.size() + 1;
	    ki++;


	    // To bad we were not able to get the right 
	    // prediction... no hit for us.
	    // That means we ran each character of the 
	    // string through Soothsayer. We only need 
	    // to cater for our beloved trailing 
	    // whitespace then.
	} else {

	    // If we're positioned at the end of the
	    // string, we've used all characters up
	    // and got the right prediction right on 
	    // our last chance.
	    // Selecting the prediction or entering
	    // space is equivalent (supposing selecting
	    // the prediction counts as one keystroke).
	    // We'll simulate entering a whitespace.
	    if( i == str.end() ) {

		soothsayerPtr->update( " " );
		ki++;
		kn += str.size() + 1;

	    } else {

		// Else we got the right prediction
		// b4 we got to the end of the string
		// Let's update Soothsayer with 
		// the remainder of the string we
		// didn't use and take care of our
		// trailing whitespace.
		std::string suffix;
		suffix.insert( suffix.begin(), i, str.end() );
		soothsayerPtr->update( suffix + " " );
		if( !autoSpace ) {
		    ki++;
		}
	    }
	}
    }
}


void Simulator::reset()
{
    delete soothsayerPtr;
    soothsayerPtr = new Soothsayer;

    ki = 0;
    ks = 1;
    kn = 0;
}


void Simulator::results() const
{
    std::cout << std::endl
              << "============================" << std::endl
	      << "Keystroke Savings Rate (KSR)" << std::endl
	      << "           ki + ks        "   << std::endl
	      << "KSR = (1 - ------- ) * 100"   << std::endl
	      << "             kn           "   << std::endl
	      << "where: "                      << std::endl
	      << "       ki = actual keystrokes" << std::endl
	      << "       ks = keystrokes required to select suggestion" << std::endl
	      << "       kn = keystrokes required with no prediction enabled" << std::endl
	      << std::endl
	      << "ki : " << ki << std::endl
	      << "ks : " << ks << std::endl
	      << "kn : " << kn << std::endl
	      << std::endl
	      << "KSR: " << getKSR() << std::endl;
}


int Simulator::getKi() const
{
    return ki;
}


int Simulator::getKs() const
{
    return ks;
}


int Simulator::getKn() const
{
    return kn;
}


double Simulator::getKSR() const
{
    return ( ( 1 - ( static_cast<double>( ki + ks ) / static_cast<double>( kn ) ) ) * 100 );
}


void Simulator::setKs( int value )
{
    if( value > 0 )
	ks = value;
}


bool Simulator::find( const std::vector<std::string>& w, const std::string& t ) const
{
    if (!silent_mode) {
	std::cout << "===> " << t << std::endl;
    }
    bool found = false;
    std::vector<std::string>::const_iterator i = w.begin();
    while( i != w.end() && !found ) {
	if (!silent_mode) {
	    std::cout << *i << std::endl;
	}
	if( *i == t )
	    found = true;
	i++;
    }

    return found;
}
 
bool Simulator::getAutoSpace() const
{
    return autoSpace;
}

void Simulator::setAutoSpace( bool b )
{
    autoSpace = b;
}

void Simulator::silentMode(bool mode)
{
    silent_mode = mode;
}
