
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


#include "presage.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#include <getopt.h>

#include <iostream>
#include <sstream>

const char PROGRAM_NAME[] = "presage_demo_text";

void disclaimer();
void parseCommandLineArgs(int argc, char** argv);
void printVersion();
void printUsage();
void print_prediction(std::vector<std::string>);

std::string config;
int suggestions = 0;

int main(int argc, char** argv)
{
    parseCommandLineArgs(argc, argv);
    disclaimer();

    // magic starts here...
    LegacyPresageCallback callback;
    Presage presage(&callback, config);


    if (suggestions) {
	// problem with this is that even if I implemented the
	// config() method in Presage class, it still would not
	// work because components read config values at construction
	// time. Which means that for this to work the Selector would
	// have to be destroyed and recreated (which would imply
	// losing current state) or would have to have an init()
	// method which re-reads the config from config.
	// 
	// Presage class could be made smart by only re-init()'ing
	// only the affected module by looking at the
	// Presage.Selector part of the configuration variable.
	//
	// All this logic probably belongs to a Configuration class,
	// which could do the mapping between variable stem and
	// component. So, Presage::config() would result in a
	// Configuration::update() call, which would update the
	// internal representation of the configuration value and then
	// invoke on the init() method of the affected component
	// (where a component is for example Selector).
	//

	// A much easier and cleaner solution would be that each
	// component avoids caching configuration variables and always
	// ask a Configuration class for the value associated to a
	// variable each time it needs it. This way configuration can
	// be programmatically changed on-the-fly.
	//

	// convert int to string using a stringstream
	std::stringstream ss;
	ss << suggestions;
	presage.config("Presage.Selector.SUGGESTIONS", ss.str());
    }

    // buffer to read user input
    const int BUFFER_SIZE = 80;
    char buffer[ BUFFER_SIZE ];

    for (;;) {
        std::cout << "> ";                       // prompt the user
        std::cin.getline (buffer, BUFFER_SIZE);  // read in string (if any)

	callback.update(buffer);  // update internal buffer
        print_prediction(
            presage.predict()      // request new prediction
	    );
        std::cout << "-- Context: " << presage.context() << '|' << std::endl;
        if (presage.context_change()) {
            std::cout << "-- Context changed" << std::endl;
        }
    }

    return 0;
}


void disclaimer()
{
    std::cout <<
        "Presage Textual Demo\n"
        "--------------------\n"
        "\n"
        "This program is intended as a demonstration of Presage ONLY.\n"
        "\n"
        "The Presage project aims to provide an intelligent predictive text entry platform.\n"
        "\n"
        "Its intent is NOT to provide a predictive text entry user interface.\n"
        "Think of Presage as the predictive backend that sits behind a shiny user interface and does all the predictive heavy lifting.\n"
        "\n" << std::endl;
}

void parseCommandLineArgs(int argc, char* argv[])
{
    int next_option;

    // getopt structures
    const char* const short_options = "c:s:hv";

    const struct option long_options[] = {
	{ "config",       required_argument, 0, 'c' },
	{ "suggestions",  required_argument, 0, 's' },
	{ "help",         no_argument,       0, 'h' },
	{ "version",      no_argument,       0, 'v' },
	{ 0, 0, 0, 0 }
    };

    do {
        next_option = getopt_long( argc, argv, 
                                   short_options, long_options, NULL );

        switch( next_option ) {
	case 'c': // --config or -c option
            config = optarg;
            break;
	case 's': // --suggestions or -s option
	    suggestions = atoi(optarg);
	    break;
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
	    break;
	}

    } while( next_option != -1 );
}

void print_prediction(std::vector<std::string> words)
{
    for( std::vector<std::string>::const_iterator i = words.begin();
         i != words.end();
         i++ )
        std::cout << *i << std::endl;
}

void printVersion()
{
    std::cout << PROGRAM_NAME << " (" << PACKAGE << ") version " << VERSION << std::endl
              << "Copyright (C) 2004 Matteo Vescovi." << std::endl
              << "This is free software; see the source for copying conditions.  There is NO" << std::endl
              << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE," << std::endl
              << "to the extent permitted by law." << std::endl;
}

void printUsage()
{
    std::cout << "Usage: " << PROGRAM_NAME << " [OPTION]..." << std::endl
              << std::endl
              << "At the prompt, type in some text. Hit enter to generate a prediction." << std::endl
              << "Any text input is valid, including no text, a single character, or a long string." << std::endl
              << std::endl
              << "  -c, --config CONFIG  use config file CONFIG" << std::endl
              << "  -s, --suggestions N  set prediction size to N suggestions" << std::endl
              << "  -h, --help           display this help and exit" << std::endl
              << "  -v, --version        output version information and exit" << std::endl
              << std::endl
              << "Direct your bug reports to: " << PACKAGE_BUGREPORT << std::endl;
}
