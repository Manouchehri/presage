
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

#ifndef DATABASE_H
#define DATABASE_H


#include <sqlite.h>


sqlite* openDatabase( char* );
int createUnigramTable( sqlite* );
int createBigramTable( sqlite* );
int createTrigramTable( sqlite* );
void closeDatabase( sqlite* );

int incrementCount( sqlite*, const char* );
int incrementCount( sqlite*, const char*, const char* );
int incrementCount( sqlite*, const char*, const char*, const char* );

int insertUnigram( sqlite*, const char*, int );
int insertBigram(  sqlite*, const char*, const char*, int );
int insertTrigram( sqlite*, const char*, const char*, const char*, int );

int nullCallback( void* nPtr, int argc, char** argv, char** column );
int printCallback( void* nPtr, int argc, char** argv, char** column);
int getCountCallback( void* countPtr, int argc, char** argv, char** column);


#endif
