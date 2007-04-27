
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>            *
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

#include "database.h"
#include "utility.h"
#include <string>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <ctype.h>


sqlite* openDatabase( char* dbname ) {

    // TODO : check file exists, ask whether overwrite or not, etc...

    sqlite* db = sqlite_open( dbname, 0777, NULL );
    assert( db != NULL );
	
    return db;
}


int createUnigramTable( sqlite* db ) {
	
    int result;
    result = sqlite_exec( db,
			  "CREATE TABLE unigram "
			  "( word TEXT UNIQUE, count INTEGER );",
			  nullCallback,
			  NULL,
			  NULL );
    assert( result == SQLITE_OK );
	
    return result;
}


int createBigramTable( sqlite* db ) {

    int result;
    result = sqlite_exec( db,
			  "CREATE TABLE bigram "
			  "( word_1 TEXT,"
			  "  word TEXT,"
			  "  count INTEGER,"
			  "  UNIQUE( word_1, word ) "
			  ");",
			  nullCallback,
			  NULL,
			  NULL );
    assert( result == SQLITE_OK );	

    return result;
}


int createTrigramTable( sqlite* db ) {

    int result;
    result = sqlite_exec( db,
			  "CREATE TABLE trigram "
			  "( word_2 TEXT,"
			  "  word_1 TEXT,"
			  "  word TEXT,"
			  "  count INTEGER,"
			  "  UNIQUE( word_2, word_1, word ) "
			  ");",
			  nullCallback,
			  NULL,
			  NULL );
    assert( result == SQLITE_OK );

    return result;
}


void closeDatabase( sqlite* db ) {

    sqlite_close( db );

}


int incrementCount( sqlite* db, const char* word ) {

    std::string query;
    int count = 0;
    int result;


    // convert string to lower case
    int length = strlen( word );
    char* loword = new char[ length + 1 ];
    strcpy( loword, word );
    strtolower( loword );


    query = "SELECT count "
	"FROM unigram "
	"WHERE word = \"" +
	static_cast<std::string>( loword ) + "\";";
    result = sqlite_exec( db,
			  query.c_str(),
			  getCountCallback,
			  &count,
			  NULL );
    assert( result == SQLITE_OK );

    if( count == 0 ) { // unigram not yet in database
		
	// insert new unigram
	insertUnigram( db, loword, count + 1 );

    } else { // unigram present, increment its count

	// convert count integer to string through stringstream
	count++;
	std::stringstream ss;
	ss << count;
		
	query = "UPDATE unigram "
	    "SET count = " + ss.str() + " "
	    "WHERE word = \"" + loword + "\";";
	result = sqlite_exec( db,
			      query.c_str(),
			      nullCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );
		
    }
		
    return result;
}


int incrementCount( sqlite* db, const char* word_1, const char* word ) {

    std::string query;
    int count = 0;
    int result;

    // convert string to lower case
    int length = strlen( word_1 );
    char* loword_1 = new char[ length + 1 ];
    strcpy( loword_1, word_1 );
    strtolower( loword_1 );

    length = strlen( word );
    char* loword = new char[ length + 1 ];
    strcpy( loword, word );
    strtolower( loword );


    query = "SELECT count ";
    query+= "FROM bigram ";
    query+=	"WHERE word_1 = \"";
    query+=	loword_1;
    query+= "\" AND ";
    query+=	"word = \"";
    query+= loword;
    query+= "\";";
    result = sqlite_exec( db,
			  query.c_str(),
			  getCountCallback,
			  &count,
			  NULL );
    assert( result == SQLITE_OK );


    if( count == 0 ) {

	insertBigram( db, loword_1, loword, count + 1 );

    } else {

	// convert count integer to string through stringstream
	count++;
	std::stringstream ss;
	ss << count;
		
	query = "UPDATE bigram "
	    "SET count = " + ss.str() + " "
	    "WHERE word_1 = \"" + loword_1 + "\" AND "
	    "word = \"" + loword + "\";";
	result = sqlite_exec( db,
			      query.c_str(),
			      nullCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );
		
    }
	
    return result;
}


int incrementCount( sqlite* db, 
		    const char* word_2,
		    const char* word_1,
		    const char* word ) {

    std::string query;
    int count = 0;
    int result;

    // convert string to lower case
    int length = strlen( word_2 );
    char* loword_2 = new char[ length + 1 ];
    strcpy( loword_2, word_2 );
    strtolower( loword_2 );

    length = strlen( word_1 );
    char* loword_1 = new char[ length + 1 ];
    strcpy( loword_1, word_1 );
    strtolower( loword_1 );

    length = strlen( word );
    char* loword = new char[ length + 1 ];
    strcpy( loword, word );
    strtolower( loword );


	
    // I could get rid of these annoying static_casts... :-)
    query = static_cast<std::string>("SELECT count "    ) +
	static_cast<std::string>("FROM trigram "    ) +
	static_cast<std::string>("WHERE word_2 = \"") + 
	static_cast<std::string>( loword_2          ) +
	static_cast<std::string>("\" AND "          ) +
	static_cast<std::string>("word_1 = \""      ) + 
	static_cast<std::string>( loword_1          ) + 
	static_cast<std::string>("\" AND word = \"" ) + 
	static_cast<std::string>( loword            ) + 
	static_cast<std::string>("\";"              );
    result = sqlite_exec( db,
			  query.c_str(),
			  getCountCallback,
			  &count,
			  NULL );
    assert( result == SQLITE_OK );


    if( count == 0 ) {
		
	insertTrigram( db, loword_2, loword_1, loword, count + 1 );

    } else {
		
	// convert count integer to string through stringstream
	count++;
	std::stringstream ss;
	ss << count;
		
	query = "UPDATE trigram "
	    "SET count = " + ss.str() + " "
	    "WHERE word_2 = \"" + loword_2 + "\" AND "
	    "word_1 = \"" + loword_1 + "\" AND word = \"" +
	    loword + "\";";
	result = sqlite_exec( db,
			      query.c_str(),
			      nullCallback,
			      NULL,
			      NULL );
	assert( result == SQLITE_OK );
		
    }
	
    return result;
}


int insertUnigram( sqlite* db, const char* word, int count )
{

    int result;
    std::stringstream ss;
    std::string query;

    ss << count;

    // convert string to lower case
    int length = strlen( word );
    char* loword = new char[ length + 1 ];
    strcpy( loword, word );
    strtolower( loword );


    query = "INSERT INTO unigram "
	"VALUES( \"" +
	static_cast<std::string>( loword ) + "\", " +
	ss.str() + ");";
//	std::cout << std::endl << query << std::endl;
    result = sqlite_exec( db,
			  query.c_str(),
			  nullCallback,
			  NULL,
			  NULL );
    assert( result == SQLITE_OK );

    return result;

}			      


int insertBigram( sqlite* db, const char* w_1, const char* w, int count )
{
	
    int result;
    std::stringstream ss;
    std::string query;

    ss << count;

    // convert string to lower case
    int length = strlen( w_1 );
    char* low_1 = new char[ length + 1 ];
    strcpy( low_1, w_1 );
    strtolower( low_1 );

    length = strlen( w );
    char* low = new char[ length + 1 ];
    strcpy( low, w );
    strtolower( low );

    query = "INSERT INTO bigram "
	"VALUES( \"" +
	static_cast<std::string>( low_1 ) + "\", \"" +
	static_cast<std::string>( low ) + "\", " + ss.str() + " );";
//	std::cout << std::endl << query << std::endl;
    result = sqlite_exec( db,
			  query.c_str(),
			  nullCallback,
			  NULL,
			  NULL );
    assert( result == SQLITE_OK );

    return result;

}


int insertTrigram( sqlite* db,
		   const char* w_2,
		   const char* w_1,
		   const char* w,
		   int count )
{
	
    int result;
    std::stringstream ss;
    std::string query;

    ss << count;

    // convert string to lower case
    int length = strlen( w_2 );
    char* low_2 = new char[ length + 1 ];
    strcpy( low_2, w_2 );
    strtolower( low_2 );

    length = strlen( w_1 );
    char* low_1 = new char[ length + 1 ];
    strcpy( low_1, w_1 );
    strtolower( low_1 );

    length = strlen( w );
    char* low = new char[ length + 1 ];
    strcpy( low, w );
    strtolower( low );


    query = "INSERT INTO trigram "
	"VALUES( \"" +
	static_cast<std::string>( low_2 ) + "\", \"" +
	static_cast<std::string>( low_1 ) + "\", \"" +
	static_cast<std::string>( low ) + "\", " + ss.str() + " );";
//	std::cout << std::endl << query << std::endl;
    result = sqlite_exec( db,
			  query.c_str(),
			  nullCallback,
			  NULL,
			  NULL );
    assert( result == SQLITE_OK );

    return result;

}

//int getCount(sqlite* db, const char* prefix)
//{
//    std::string query = 
//	"SELECT word, count "
//	"FROM unigram "
//	"WHERE word LIKE \"" + prefix + "%\" "
//	"ORDER BY count DESC;";
//    int result = sqlite_exec(db,
//			     query,
//			     query.c_str(),
//			     getWordCount,
//			     &wordCount,
//			     NULL );
//    assert( result == SQLITE_OK || result == SQLITE_ABORT );
//
//    
//}



// callback functions //////////////////////////////////////////////////////

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
