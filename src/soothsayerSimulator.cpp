
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

#include <iostream>
#include <fstream>
#include <string.h>

#include <getopt.h>

#include "core/charsets.h"
#include "core/tokenizer/forwardTokenizer.h"
#include "simulator/simulator.h"


std::string program_name;

void printUsage();
void printVersion();

int main( int argc, char* argv[] ) {
	
    program_name = argv[0];
    int next_option;

	
    // getopt structures
    const char* const short_options = "hv";

    const struct option long_options[] = {
	{ "help",   0, NULL, 'h' },
	{ "version",0, NULL, 'v' },
	{ NULL,     0, NULL,   0 }
    };

    do {
	next_option = getopt_long( argc, argv, 
				   short_options, long_options, NULL );
		
	switch( next_option ) {
	case 'h': // --help or -h option
	    printUsage();
	    exit( 0 );
	    break;
	case 'v': // --version or -v option
	    printVersion();
	    exit( 0 );
	    break;
	case '?': // unknown option
	    printUsage();
	    exit( 0 );
	    break;
	case -1:
	    break;
	default:
	    abort();
	}

    } while( next_option != -1 );


    // check we have enough arguments
    if( argc - optind < 1 ) {
	printUsage();
	exit(0);
    }
	

    // open infile for input
    std::ifstream infile( argv[ optind ], std::ios::in );
    if( !infile )
	std::cerr << "\aError: could not open file " << argv[ optind ] << std::endl;
    assert( infile );

    Simulator simulator;

    ForwardTokenizer tokenizer(infile,
			       DEFAULT_BLANKSPACE_CHARS,
			       DEFAULT_SEPARATOR_CHARS);
    while(tokenizer.hasMoreTokens()) {
	simulator.simulate(tokenizer.nextToken());
    }
		
    simulator.results();

    infile.close();

    return 0;
}




void printVersion()
{
    std::cout << "Version 0.1" << std::endl;
}


void printUsage()
{
    std::cout << "Usage: " << program_name
	      << " [INFILE]"
	      << std::endl << std::endl
	      << " where each line of INFILE contains a single token"
	      << std::endl << std::endl
	      << program_name 
	      << " is free software distributed under the GPL."
	      << std::endl
	      << "Copyright by Matteo Vescovi" << std::endl;
}


