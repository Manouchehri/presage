
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

#include "soothsayer.h"

#include <iostream>
#include <getopt.h>

const char PROGRAM_NAME[] = "soothsayerDemoText";

void parseCommandLineArgs(int argc, char** argv);
void printVersion();
void printUsage();

void print_prediction(std::vector<std::string>);
void disclaimer();

int main(int argc, char** argv)
{
    parseCommandLineArgs(argc, argv);

    disclaimer();

    Soothsayer soothsayer;

    const int BUFFER_SIZE = 80;
    char buffer[ BUFFER_SIZE ];

    while(1) {
	std::cout << "> ";
	std::cin.getline( buffer, BUFFER_SIZE );

	print_prediction(soothsayer.predict(static_cast<std::string>(buffer)));
	if (soothsayer.contextChange()) {
	    std::cout << "-- Context changed" << std::endl;
	}
    }
    return 0;
}


void print_prediction(std::vector<std::string> words)
{
    for( std::vector<std::string>::const_iterator i = words.begin();
	 i != words.end();
	 i++ )
	std::cout << *i << std::endl;
}

void disclaimer()
{
    std::cout <<
	"Soothsayer Textual Demo\n"
	"-----------------------\n"
	"\n"
	"This program is intended as a demonstration of Soothsayer ONLY.\n"
	"\n"
	"The Soothsayer project aims to provide an intelligent predictive text entry platform.\n"
	"\n"
	"Its intent is NOT to provide a predictive text entry user interface.\n"
	"Think of Soothsayer as the predictive backend that sits behind a shiny user interface and does all the predictive heavy lifting.\n"
	"\n" << std::endl;
}

void parseCommandLineArgs(int argc, char* argv[])
{
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
	    exit (0);
	    break;
	case 'v': // --version or -v option
	    printVersion();
	    exit (0);
	    break;
	case '?': // unknown option
	    printUsage();
	    exit (0);
	    break;
	case -1:
	    break;
	default:
	    abort();
	}

    } while( next_option != -1 );
}

void printVersion()
{
    std::cout << PACKAGE << " version " << VERSION << std::endl
	      << "Copyright (C) 2004 Matteo Vescovi." << std::endl
	      << "This is free software; see the source for copying conditions.  There is NO" << std::endl
	      << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE," << std::endl
	      << "to the extent permitted by law." << std::endl;
}

void printUsage()
{
    std::cout << "Usage: " << PROGRAM_NAME << std::endl
	      << std::endl
	      << "  -h, --help     display this help and exit" << std::endl
	      << "  -v, --version  output version information and exit" << std::endl
	      << std::endl
	      << "Direct your bug reports to: " << PACKAGE_BUGREPORT << std::endl;
}
