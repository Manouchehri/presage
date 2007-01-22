
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
#include <sqlite.h>
#include <assert.h>

#include <unistd.h> // for unlink()

#include <dirent.h> // for opendir(), readdir(), closedir(), DIR, dirent
 
#include <string.h>

#include "tools.h"
#include "core/utility.h"

void printExpectedOutput(const char* const);

int main()
{
	const char FILENAME[] = "test.db";

	int result = 0;
	DIR* dp;
	struct dirent *ep;

	// check if file exists in current directory
	dp = opendir ("./");
	if (dp != NULL) {
		while ((ep = readdir (dp)) && (result == 0)) {
			if (strcmp(ep->d_name, FILENAME) == 0) {
				result = 1;
			}
		}
		(void) closedir (dp);
	} else {
		perror ("Couldn't open current directory");
	}
	
	// remove file if it exists
	if (result) {
		result = unlink(FILENAME);
		assert(result == 0);
	}

	// create test database
	sqlite* db = sqlite_open( FILENAME, 0777, NULL );
	assert(db != 0);

	result = sqlite_exec( db,
			      "CREATE TABLE unigram ("
			      " word TEXT UNIQUE,"
			      " count INTEGER "
			      ");",
			      NULL,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );
	


	// insert unigram (foo, 3)
	insertUnigram( db, "foo", 3 );

	// should print count = 3
        printExpectedOutput("count == 3");
	result = sqlite_exec( db,
			      "SELECT word, count "
			      "FROM unigram "
			      "WHERE word = \"foo\";",
			      printCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );


	// test increment function when unigram exists
	incrementCount( db, "foo" );

	// should print count = 4
        printExpectedOutput("count == 4");
	result = sqlite_exec( db,
			      "SELECT word, count "
			      "FROM unigram "
			      "WHERE word = \"foo\";",
			      printCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );


	// test increment count of unigram which does not exist
	incrementCount( db, "bar" );
	
	// should print out count = 1
        printExpectedOutput("count == 1");
	result = sqlite_exec( db,
			      "SELECT word, count "
			      "FROM unigram "
			      "WHERE word = \"bar\";",
			      printCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );




	// test strtolower
	char mixString[] = "foObaRFOoBAr_foo_fOOBArFOo!! Foo BAr";
        printExpectedOutput(mixString);
	std::cout << mixString << std::endl;

        printExpectedOutput("foobarfoobar_foo_foobarfoo!! foo bar");
	std::cout << strtolower( mixString ) << std::endl;
	

	sqlite_close( db );
	
	return 0;
}

void printExpectedOutput(const char* const message)
{
    const int  WIDTH     = 50;
    const char SEPARATOR = '=';

    std::cout << std::endl << std::endl;
    for (int i = 0; i < WIDTH; i++) {
        std::cout << SEPARATOR;
    }
    std::cout << std::endl;

    std::cout << message << std::endl;

    for (int i = 0; i < WIDTH; i++) {
        std::cout << SEPARATOR;
    }
    std::cout << std::endl;
}
