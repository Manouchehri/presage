
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


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#endif

#include <getopt.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "core/charsets.h"
#include "core/tokenizer/forwardTokenizer.h"
#include "simulator/simulator.h"

const char PROGRAM_NAME[] = "presage_simulator";

void parseCommandLineArgs(int argc, char* argv[]);
void printUsage();
void printVersion();

bool silent_mode = false;
bool case_insensitive = false;
std::string config;

// Simple callback class, stores past context stream in a stringstream
// passed in at construction time.
// 
// getPastStream() returns a string of size() up to 'width'.
// getFutureStream() return an empty string.
// 
class SimulatorPresageCallback
    : public PresageCallback 
{
public:
    SimulatorPresageCallback(std::stringstream& sstream) : m_sstream(sstream) { }
    ~SimulatorPresageCallback() { };
    
    std::string get_past_stream() const { return m_sstream.str(); }
    std::string get_future_stream() const { return m_empty; }

private:
    std::stringstream& m_sstream;
    const std::string m_empty;
};

int main(int argc, char* argv[])
{
    parseCommandLineArgs(argc, argv);

    // check we have enough arguments
    if( argc - optind < 1 ) {
	printUsage();
	exit (0);
    }

    // open infile for input
    std::ifstream infile( argv[ optind ], std::ios::in );
    if( !infile ) {
	std::cerr << "\aError: could not open file " << argv[ optind ] << std::endl;
	return 1;
    }
    
    std::stringstream sstream;    // stringstream to hold past context stream
    Simulator simulator(new SimulatorPresageCallback(sstream), sstream, config);
    simulator.silentMode(silent_mode);

    ForwardTokenizer tokenizer(infile,
			       DEFAULT_BLANKSPACE_CHARS,
			       DEFAULT_SEPARATOR_CHARS);
    tokenizer.lowercaseMode(case_insensitive);
    while(tokenizer.hasMoreTokens()) {
	simulator.simulate(tokenizer.nextToken());
    }
		
    simulator.results();

    infile.close();

    return 0;
}


void parseCommandLineArgs(int argc, char* argv[])
{
    int next_option;
	
    // getopt structures
    const char* const short_options = "c:sqihv";

    const struct option long_options[] = {
        { "config",      required_argument, 0, 'c' },
	{ "silent",      no_argument,       0, 's' },
	{ "quiet",       no_argument,       0, 'q' },
        { "insensitive", no_argument,       0, 'i' },
	{ "help",        no_argument,       0, 'h' },
	{ "version",     no_argument,       0, 'v' },
	{ 0, 0, 0, 0 }
    };

    do {
	next_option = getopt_long( argc, argv, 
				   short_options, long_options, NULL );
		
	switch( next_option ) {
          case 'c': // -- config of -f option
            config = optarg;
            break;
          case 's': // --silent or -s option
          case 'q': // --quiet or -q option
	    silent_mode = true;
	    break;
          case 'i': // --insensitive or -i option
            case_insensitive = true;
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
	}

    } while( next_option != -1 );
}

void printVersion()
{
    std::cout << PROGRAM_NAME << " (" << PACKAGE << ") version " << VERSION << std::endl
	      << "Copyright (C) Matteo Vescovi." << std::endl
	      << "This is free software; see the source for copying conditions.  There is NO" << std::endl
	      << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE," << std::endl
	      << "to the extent permitted by law." << std::endl;
}

void printUsage()
{
    std::cout << "Usage: " << PROGRAM_NAME << " [OPTION]... FILE" << std::endl
	      << std::endl
              << "  -c, --config CONFIG  use config file CONFIG" << std::endl
              << "  -i, --insensitive    case insensitive mode" << std::endl
	      << "  -q, --quiet          quiet mode, no verbose output, same as silent" << std::endl
	      << "  -s, --silent         silent mode, no verbose output, same as quiet" << std::endl
	      << "  -h, --help           display this help and exit" << std::endl
	      << "  -v, --version        output version information and exit" << std::endl
	      << std::endl
	      << "Direct your bug reports to: " << PACKAGE_BUGREPORT << std::endl;
}
