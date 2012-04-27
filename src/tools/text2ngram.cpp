
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


#include "config.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <map>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#include <getopt.h>
#include <assert.h>

#include "core/tokenizer/forwardTokenizer.h"
#include "core/iso8859_1.h"
#include "core/progress.h"

#include "../lib/predictors/dbconnector/sqliteDatabaseConnector.h"

const std::string PROGRAM_NAME = "text2ngram";

typedef std::list<std::string> NgramList;

void usage();
void version();

int main(int argc, char* argv[])
{
    int next_option;

    // Setup some defaults
    //  - default to generating 1-gram counts
    int ngrams = 1;

    //  - default output to stdout (empty string signifies stdout)
    std::string output;

    const std::string TABBED_SEPARATED_VALUES = "tsv";
    const std::string SQLITE = "sqlite";
    //  - default format is tabbed separated values 
    std::string format = TABBED_SEPARATED_VALUES;

    //  - default to case sensitive
    bool lowercase = false;

    //  - default to no append
    bool append    = false;

	
    // getopt structures
    const char * const  short_options  = "n:o:f:alhv";
    const struct option long_options[] =
	{
	    { "ngrams",    required_argument, 0, 'n' },
	    { "output",    required_argument, 0, 'o' },
	    { "format",    required_argument, 0, 'f' },
	    { "append",    no_argument,       0, 'a' },
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
	case 'n': // --ngrams or -n option
	    if (atoi(optarg) > 0) {
		ngrams = atoi(optarg);
	    } else {
		usage();
	    }
	    break;
	case 'o': // --output or -o option
	    output = optarg;
	    break;
	case 'f': // --format or -f option
	    if (optarg == SQLITE
		|| optarg == TABBED_SEPARATED_VALUES) {
		format = optarg;
	    } else {
		std::cerr << "Unknown format " << optarg << std::endl << std::endl;
		usage();
		return -1;
	    }
	    break;
	case 'a': // --append or -a option
	    // append mode
            append = true;
	    break;
	case 'l': // --lowercase or -l option
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


    if ((argc - optind < 1)) {
	usage();
	return -1;
    }
	

    // ngramMap stores <token,count> pairs
    std::map<NgramList, int> ngramMap;

    for (int i = optind; i < argc; i++) {
	// do the actual processing file by file
	std::string token;
	NgramList ngram;

	// points to output file
	// print out file information
	std::cout << "Parsing " << argv[i] << "..."
		  << std::endl;

	ProgressBar<char> progressBar;

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
	    //progressBar(tokenizer.progress());
	    progressBar.update(tokenizer.progress());

	    // remove front token from ngram
	    ngram.pop_front();
	}
	
	infile.close();
    }


    std::cout << "Writing out to " << format << " format file "
	      << output << "..." << std::endl;
    if (format == TABBED_SEPARATED_VALUES) {
	// output to tabbed separated values text file
	//

	std::ofstream *outstream = 0;
	std::ostream  *prev_outstream = 0;

	if (output.c_str()) {
	    // tie outstream to file
	    outstream = new std::ofstream (output.c_str(), std::ios::out);
	    assert(outstream);
	    prev_outstream = std::cout.tie (outstream);
	}

	// write results to output stream
	ProgressBar<char> progressBar;
	long total = ngramMap.size();
	long count = 0;
	std::map<NgramList, int>::const_iterator it;
	for (it = ngramMap.begin(); it != ngramMap.end(); it++) {
	    for (NgramList::const_iterator ngram_it = it->first.begin();
		 ngram_it != it->first.end();
		 ngram_it++) {
		std::cout << *ngram_it << '\t';
	    }
	    std::cout << it->second << std::endl;
	    progressBar.update(static_cast<double>(count++)/total);
	}

	if (output.c_str()) {
	    std::cout.tie (prev_outstream);
	    outstream->close ();
	    delete outstream;
	}

    } else if (format == SQLITE) {
	// output to SQLITE
	// 

	SqliteDatabaseConnector sqliteDbCntr(output, ngrams, true);
        sqliteDbCntr.beginTransaction();
        sqliteDbCntr.createNgramTable(ngrams);

	// write results to output stream
	ProgressBar<char> progressBar;
	long total = ngramMap.size();
	long count = 0;
	std::map<NgramList, int>::const_iterator it;
	for (it = ngramMap.begin(); it != ngramMap.end(); it++) {

	    // convert from NgramList to Ngram
	    Ngram ngram;
	    for (NgramList::const_iterator jt = it->first.begin();
		 jt != it->first.end();
		 jt++) {
		ngram.push_back(*jt);
	    }

            if (append) {
                // need to check whether ngram is already in database.
                // when appending to existing database
                int count = sqliteDbCntr.getNgramCount(ngram);
                if (count > 0) {
                    // ngram already in database, update count
                    sqliteDbCntr.updateNgram(ngram, count + it->second);
                } else {
                    // ngram not in database, insert it
                    sqliteDbCntr.insertNgram(ngram, it->second);
                }
            } else {
                // insert ngram
                sqliteDbCntr.insertNgram(ngram, it->second);
            }

	    progressBar.update(static_cast<double>(count++)/total);
	}
        sqliteDbCntr.endTransaction();
    } else {
	abort();
    }


    std::cout << std::endl;

    return 0;
}


void version()
{
    std::cout
	<< PROGRAM_NAME << " (" << PACKAGE << ") version " << VERSION << std::endl
	<< "Copyright (C) Matteo Vescovi" << std::endl
	<< "This is free software; see the source for copying conditions.  There is NO" << std::endl
	<< "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl
	<< std::endl;
}


void usage()
{
    std::cout 
	<< "Usage: " << PROGRAM_NAME << " [OPTION]... infiles..." << std::endl
	<< std::endl
        << "  --output, -o O  " << "Output file name O" << std::endl
	<< "  --ngrams, -n N  " << "Specify ngram cardinality N" << std::endl
	<< "  --format, -f F  " << "Output file format F: sqlite, tsv (tabbed separated values)" << std::endl
	<< "  --lowercase, -l " << "Enable lowercase conversion mode" << std::endl
	<< "  --append, -a    " << "Open output file in append mode" << std::endl
	<< "  --help, -h      " << "Display this information" << std::endl
	<< "  --version, -v   " << "Show version information" << std::endl
	<< std::endl
	<< PROGRAM_NAME << " is free software distributed under the GPL." << std::endl
	<< "Send bug reports to " << PACKAGE_BUGREPORT << std::endl
	<< "Copyright (C) Matteo Vescovi" << std::endl;
}
