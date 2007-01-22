
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
#include <string>
#include <sstream>


int nullCallback( void* nPtr, int argc, char** argv, char** column );
int printCallback( void* nPtr, int argc, char** argv, char** column);
int getCountCallback( void* countPtr, int argc, char** argv, char** column);


int main() 
{
	int result;



	// open database
	sqlite* db = sqlite_open( "query.db", 0777, NULL );
	assert( db != NULL );




	// create unigram table
	result = sqlite_exec( db,
			      "CREATE TABLE unigram "
			      "( word TEXT UNIQUE, count INTEGER );",
			      nullCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );
	
	



	// create bigram table
	result = sqlite_exec( db,
			      "CREATE TABLE bigram "
			      "( word_1 TEXT, word TEXT, count INTEGER );",
			      nullCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );	
	

	
	// create trigram table
	result = sqlite_exec( db,
			      "CREATE TABLE trigram "
			      "( word_2 TEXT, word_1 TEXT, "
			      "word TEXT, count INTEGER );",
			      nullCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );



	// populate test table
	const int ROWS = 10;
	for( int i = 0; i < ROWS; i++ ) {

		std::stringstream ss;
		ss << i;
		std::string sql = "INSERT INTO unigram VALUES( \"word" +
			ss.str() + "\", \"" + ss.str() + "\" );";

		result = sqlite_exec( db,
				      sql.c_str(),
				      nullCallback,
				      NULL,
				      NULL );
		assert( result == SQLITE_OK );
	}
				      




	// print out test table
	result = sqlite_exec( db,
			      "SELECT * FROM unigram;",
			      printCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );

	
	

	
	// increment word4
	int count;
	result = sqlite_exec( db,
			      "SELECT count "
			      "FROM unigram "
			      "WHERE word = \"word4\";",
			      getCountCallback,
			      &count,
			      NULL );
	assert( result == SQLITE_OK );
	
	count++;
	std::stringstream ss;
	ss << count;
	std::string sql = 
		"UPDATE unigram "
		"SET count = " + ss.str() + " "
		"WHERE word = \"word4\";";

	result = sqlite_exec( db,
			      sql.c_str(),
			      nullCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );




	// select and print word4
	result = sqlite_exec( db,
			      "SELECT count "
			      "FROM unigram "
			      "WHERE word = \"word4\";",
			      printCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );




	// close database connection
	sqlite_close( db );
	
	return 0;
}




// callback functions

int nullCallback( void* nPtr, int argc, char** argv, char** column )
{
	return 0;
}


int printCallback( void* nPtr, int argc, char** argv, char** column)
{
	for( int i = 0; i < argc; i++ ) {
		std::cout << column[i] << " -> " << argv[i] << std::endl;
	}
	
	return 0;
}

int getCountCallback( void* countPtr, int argc, char** argv, char** column)
{
	assert( argc == 1 );

	// store count value to int pointed to by countPtr
	// looks tricky because countPtr is a pointer to void, so I'm casting
	// void* to int*, then argv[ 0 ] is a c string char*, so I'm calling
	// function atoi to get an integer out of it :-)
	*static_cast<int*>(countPtr) = atoi( argv[ 0 ] );

	return 0;
}
