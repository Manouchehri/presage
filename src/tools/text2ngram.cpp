
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

#include "config.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <map>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <getopt.h>
#include <assert.h>

#include "tokenizer/forwardTokenizer.h"
#include "iso8859_1.h"
#include "progress.h"

typedef std::list<std::string> Ngram;
//typedef std::string Ngram;

std::string program_name;

void usage();
void version();

int main(int argc, char* argv[]) {
	
    program_name = argv[0];
    int next_option;
    int ngrams = 1;
    bool lowercase = false;
	
    // getopt structures
    const char * const  short_options  = "n:lhv";
    const struct option long_options[] =
	{
	    { "ngrams",    required_argument, 0, 'n' },
	    { "lowercase", no_argument,       0, 'l' },
	    { "help",      no_argument,       0, 'h' },
	    { "version",   no_argument,       0, 'v' },
	    { 0,           0,                 0, 0   }
	};

    do {
	next_option = getopt_long(argc,
				  argv, 
				  short_options,
				  long_options,
				  NULL);
		
	switch (next_option) {
	case 'n':
	    std::cout << "optarg " << optarg << std::endl;
	    if (atoi(optarg) > 0) {
		ngrams = atoi(optarg);
	    } else {
		usage();
	    }
	    break;
	case 'l':
	    lowercase = true;
	    break;
	case 'h': // --help or -h option
	    usage();
	    exit (0);
	    break;
	case 'v': // --version or -v option
	    version();
	    exit (0);
	    break;
	case '?': // unknown option
	    usage();
	    exit (0);
	    break;
	case -1:
	    break;
	default:
	    std::cerr << "Error: unhandled option." << std::endl;
	    exit(0);
	}

    } while (next_option != -1);


    // check we have enough arguments
    if( argc - optind < 2 ) {
	usage();
	exit(0);
    }
	

    // open outfile for output
    std::ofstream outfile(argv[optind], std::ios::out);
    assert(outfile);
	
    for (int i = optind + 1; i < argc; i++) { // optind + 1 because optind
	// do the actual processing file by file
	std::string token;
	Ngram ngram;

	// ngramMap stores <token,count> pairs
	std::map<Ngram, int> ngramMap;
	
	// points to output file
	// print out file information
	std::cout << "Parsing file " << argv[i]
		  << std::endl << std::endl;

	printProgressHeading();

	// create tokenizer object and open input file stream
	std::ifstream infile(argv[i]);
	ForwardTokenizer tokenizer(infile,
				   " \f\n\r\t\v",
				   "`~!@#$%^&*()_-+=\\|]}[{'\";:/?.>,<");
	tokenizer.lowercaseMode(lowercase);

	// take care of first N-1 tokens
	for (int i = 0; (i < ngrams - 1 && tokenizer.hasMoreTokens()); i++) {
	    ngram.push_back(tokenizer.nextToken());
	}

	while (tokenizer.hasMoreTokens()) {
	    // extract token from input stream
	    token = tokenizer.nextToken();

	    // update ngram with new token
	    ngram.push_back(token);
			
	    // update map with new token occurrence
	    ngramMap[ngram] = ngramMap[ngram] + 1;
		    
	    // update progress bar
	    progressBar(tokenizer.progress());

	    // remove front token from ngram
	    ngram.pop_front();
	}
	
	infile.close();

	// write results to output stream
	std::map<Ngram, int>::const_iterator it;
	for (it = ngramMap.begin(); it != ngramMap.end(); it++) {
	    for (Ngram::const_iterator ngram_it = it->first.begin();
		 ngram_it != it->first.end();
		 ngram_it++) {
		outfile << *ngram_it << '\t';
	    }
	    outfile << it->second << std::endl;
	}
    }

    std::cout << std::endl;

    return 0;
}


void version()
{
    std::cout
	<< program_name << ' ' << PACKAGE_VERSION << std::endl
	<< "Copyright (C) Matteo Vescovi" << std::endl
	<< "This is free software; see the source for copying conditions.  There is NO" << std::endl
	<< "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl
	<< std::endl;
}


void usage()
{
    std::cout 
	<< program_name << ' ' << VERSION << std::endl
	<< "Usage: " << program_name << " [options] outfile infile..." << std::endl
	<< "Options:" << std::endl
	<< "--ngrams, -n N  " << "Specify ngram cardinality" << std::endl
	<< "--lowercase, -l " << "Enable lowercase conversion mode" << std::endl
	<< "--help, -h      " << "Display this information" << std::endl
	<< "--version, -v   " << "Show version information" << std::endl
	<< std::endl
	<< program_name << " is free software distributed under the GPL." << std::endl
	<< "Send bug reports to " << PACKAGE_BUGREPORT << std::endl
	<< "Copyright (C) Matteo Vescovi" << std::endl;
}
