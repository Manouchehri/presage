
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>            *
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

#include <iostream>
#include <string>
#include "core/suggestion.h"


void testLessThan( const Suggestion&, const Suggestion& );
void testNotLessThan( const Suggestion& sl, const Suggestion& sr );
void test( const bool, const bool );

int main() {
    std::string str1("Foo");
    std::string str2("bar");

    Suggestion s1;
    Suggestion s2(str1,0.5);
    Suggestion s3(str2,0.3);
    Suggestion s4("scesa",0.2);
	
    std::cout << s1.getWord() << ' ' << s1.getProbability() << std::endl;
    std::cout << s2.getWord() << ' ' << s2.getProbability() << std::endl;
    std::cout << s3.getWord() << ' ' << s3.getProbability() << std::endl;
    std::cout << s4.toString() << std::endl;

    std::cout << "Testing overloaded == operator..." << std::endl
	      << "s2 == s2 evaluates to " << (s2 == s2) << std::endl
	      << "s2 == s3 evaluates to " << (s2 == s3) << std::endl;
	
    std::cout << "Testing assignment operator (default)..." << std::endl
	      << "Assign s3 to s2: s2 = s3" << std::endl;

    s2 = s3;

    std::cout << "s2 == s3 evaluates to " << (s2 == s3) << std::endl;

    std::cout << "Testing overloaded << operator..." << std::endl
	      << "s2: " << s2 << std::endl
	      << "s4: " << s4 << std::endl;

    Suggestion s5( "foobar", 0.1 );
    Suggestion s6( "foobar", 0.5 );
    Suggestion s7( "zoo", 0.3 );

    std::cout << "Testing overloaded < operator..." << std::endl;
	
    testLessThan( s5, s6 );
    testLessThan( s5, s7 );
    testNotLessThan( s5, s5 );
    testNotLessThan( s6, s5 );
    testNotLessThan( s7, s5 );

    return 0;

}


void testLessThan( const Suggestion& sl, const Suggestion& sr )
{
    std::cout << sl << " < " << sr << " returns " << ( sl < sr )
	      << std::endl;
    test( sl < sr, true );
}

void testNotLessThan( const Suggestion& sl, const Suggestion& sr )
{
    std::cout << sl << " < " << sr << " returns " << ( sl < sr )
	      << std::endl;
    test( sl < sr, false );
}

void test( const bool bl, const bool br )
{
    std::cout << ( bl == br ? "[pass]" : "[fail]" ) << std::endl;
}
