
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


#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <assert.h>

#include "tokenizer.h"
#include "progress.h"

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
    if( argc - optind < 2 ) {
	printUsage();
	exit(0);
    }
	

    // open outfile for output
    std::ofstream outfile( argv[ optind ], std::ios::out );
    assert( outfile );


    // do the actual processing file by file
    std::string token;
    bool done;
	
    Tokenizer* tokenizerPtr;
    for( int i = optind + 1; i < argc; i++ ) { // optind + 1 because optind
	// points to output file
	// print out file information
	std::cout << std::endl
		  << "Parsing file " << argv[i]
		  << std::endl << std::endl;

	printProgressHeading();

	// initialize escape variable done
	done = false;
	// create tokenizer object and open input file stream
	tokenizerPtr = new Tokenizer( argv[i] );

	while( !done ) {
			
	    // extract token from input stream
	    tokenizerPtr->tokenize( &token, &done );
			
	    // update progress bar
	    progressBar( tokenizerPtr->getProgress() );

	    // insert token into output stream
	    outfile << token << std::endl;
	}

	delete tokenizerPtr;
    }

    std::cout << std::endl << std::endl;

    // close output file
    outfile.close();

    return 0;
}






void printVersion()
{
    std::cout << "Version 0.1" << std::endl;
}


void printUsage()
{
    std::cout << "Usage: " << program_name
	      << " [OPTIONS] [OUTFILE] [INFILES]..."
	      << std::endl << std::endl
	      << program_name 
	      << " is free software distributed under the GPL."
	      << std::endl
	      << "Copyright by Matteo Vescovi" << std::endl;
}


