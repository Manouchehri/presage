
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

#include "tokenizer.h"


FileTokenizer::FileTokenizer( char* filename )
{
    infile.open( filename, std::ios::in );
    if( !infile ) {
	std::cerr << "File " << filename << " could not be opened"
		  << '\a' << std::endl; 
	exit( 1 );
    }

    setLowercase( true );

    streamLength = getStreamLength();
}

FileTokenizer::~FileTokenizer()
{
    if( infile )
	infile.close();
}

bool FileTokenizer::getLowercase() const
{
    return lowercase;
}

void FileTokenizer::setLowercase( bool b )
{
    lowercase = b;
}

bool FileTokenizer::isValid( const int c ) const
{
    if( 
//	    c == digit0 ||
//	    c == digit1 ||
//	    c == digit2 ||
//	    c == digit3 ||
//	    c == digit4 ||
//	    c == digit5 ||
//	    c == digit6 ||
//	    c == digit7 ||
//	    c == digit8 ||
//	    c == digit9 ||
	    
	c == Aletter ||
	c == Bletter ||
	c == Cletter ||
	c == Dletter ||
	c == Eletter ||
	c == Fletter ||
	c == Gletter ||
	c == Hletter ||
	c == Iletter ||
	c == Jletter ||
	c == Kletter ||
	c == Lletter ||
	c == Mletter ||
	c == Nletter ||
	c == Oletter ||
	c == Pletter ||
	c == Qletter ||
	c == Rletter ||
	c == Sletter ||
	c == Tletter ||
	c == Uletter ||
	c == Vletter ||
	c == Wletter ||
	c == Xletter ||
	c == Yletter ||
	c == Zletter ||

	c == aletter ||
	c == bletter ||
	c == cletter ||
	c == dletter ||
	c == eletter ||
	c == fletter ||
	c == gletter ||
	c == hletter ||
	c == iletter ||
	c == jletter ||
	c == kletter ||
	c == lletter ||
	c == mletter ||
	c == nletter ||
	c == oletter ||
	c == pletter ||
	c == qletter ||
	c == rletter ||
	c == sletter ||
	c == tletter ||
	c == uletter ||
	c == vletter ||
	c == wletter ||
	c == xletter ||
	c == yletter ||
	c == zletter ||

	c == Agrave  ||
	c == Aacute  ||
	c == Acirc   ||
	c == Atilde  ||
	c == Auml    ||
	c == Aring   ||
	c == AElig   ||
	c == Ccedil  ||
	c == Egrave  ||
	c == Eacute  ||
	c == Ecirc   ||
	c == Euml    ||
	c == Igrave  ||
	c == Iacute  ||
	c == Icirc   ||
	c == Iuml    ||
	c == ETH     ||
	c == Ntilde  ||
	c == Ograve  ||
	c == Oacute  ||
	c == Ocirc   ||
	c == Otilde  ||
	c == Ouml    ||
//	    c == times   ||
	c == Oslash  ||
	c == Ugrave  ||
	c == Uacute  ||
	c == Ucirc   ||
	c == Uuml    ||
	c == Yacute  ||
	c == THORN   ||
	c == szlig   ||
	c == agrave  ||
	c == aacute  ||
	c == acirc   ||
	c == atilde  ||
	c == auml    ||
	c == aring   ||
	c == aelig   ||
	c == ccedil  ||
	c == egrave  ||
	c == eacute  ||
	c == ecirc   ||
	c == euml    ||
	c == igrave  ||
	c == iacute  ||
	c == icirc   ||
	c == iuml    ||
	c == eth     ||
	c == ntilde  ||
	c == ograve  ||
	c == oacute  ||
	c == ocirc   ||
	c == otilde  ||
	c == ouml    ||
//	    c == divide  ||	    
	c == oslash  ||
	c == ugrave  ||
	c == uacute  ||
	c == ucirc   ||
	c == uuml    ||
	c == yacute  ||
	c == thorn   ||
	c == yuml    ||

	// these chars are also punctuation
	c == apostrophe ||
	c == percent

	)

	return true;
    else
	return false;
}



bool FileTokenizer::isSeparator( const int c ) const
{

    if( 
	c == digit0 ||
	c == digit1 ||
	c == digit2 ||
	c == digit3 ||
	c == digit4 ||
	c == digit5 ||
	c == digit6 ||
	c == digit7 ||
	c == digit8 ||
	c == digit9 ||

	c == space       ||
	c == exclamation ||
	c == quote       ||
	c == hashsign    ||
	c == dollar      ||
	c == percent     ||
	c == ampersand   ||
	c == apostrophe  ||
	c == openbracket ||
	c == closebracket||
	c == asterisk    ||
	c == plus        ||
	c == comma       ||
	c == minus       ||
	c == period      ||
	c == slash       ||

	c == colon       ||
	c == semicolon   ||
	c == lessthan    ||
	c == equals      ||
	c == greaterthan ||
	c == question    ||
	c == at          ||

	c == opensquare  ||
	c == backslash   ||
	c == closesquare ||
	c == pointer     ||
	c == underscore  ||
	c == grave       ||

	c == leftbrace   ||
	c == verticalbar ||
	c == rightbrace  ||
	c == tilde       ||

	c == nbsp        ||
	c == iexcl       ||
	c == cent        ||
	c == pound       ||
	c == curren      ||
	c == yen         ||
	c == brvbar      ||
	c == sect        ||
	c == uml         ||
	c == copyr       ||
	c == ordf        ||
	c == laquo       ||
	c == notsign     ||
	c == shy         ||
	c == reg         ||
	c == macr        ||
	c == deg         ||
	c == plusmn      ||
	c == sup2        ||
	c == sup3        ||
	c == acute       ||
	c == micro       ||
	c == para        ||
	c == middot      ||
	c == cedil       ||
	c == sup1        ||
	c == ordm        ||
	c == raquo       ||
	c == frac14      ||
	c == frac12      ||
	c == frac34      ||
	c == iquest      ||

	// control codes
	c == NUL        ||
	c == SOH        ||
	c == STX        ||
	c == ETX        ||
	c == EOT        ||
	c == ENQ        ||
	c == ACK        ||
	c == BEL        ||
	c == BS         ||
	c == HT         ||
	c == NL         ||
	c == VT         ||
	c == NP         ||
	c == CR         ||
	c == SO         ||
	c == SI         ||
	c == DLE        ||
	c == DC1        ||
	c == DC2        ||
	c == DC3        ||
	c == DC4        ||
	c == NAK        ||
	c == SYN        ||
	c == ETB        ||
	c == CAN        ||
	c == EM         ||
	c == SUB        ||
	c == ESC        ||
	c == FS         ||
	c == GS         ||
	c == RS         ||
	c == US         ||

	// unused codes
	c == DEL         ||
	c == PAD         ||
	c == HOP         ||
	c == BPH         ||
	c == NBH         ||
	c == IND         ||
	c == NEL         ||
	c == SSA         ||
	c == ESA         ||
	c == HTS         ||
	c == HTJ         ||
	c == VTS         ||
	c == PLD         ||
	c == PLU         ||
	c == RI          ||
	c == SS2         ||
	c == SS3         ||
	c == DCS         ||
	c == PU1         ||
	c == PU2         ||
	c == STS         ||
	c == CCH         ||
	c == MW          ||
	c == SPA         ||
	c == EPA         ||
	c == SOS         ||
	c == SGCI        ||
	c == SCI         ||
	c == CSI         ||
	c == ST          ||
	c == OSC         ||
	c == PM          ||
	c == APC         
	)

	return true;
    else
	return false;
}


/** tokenizer main function
 *  see flow chart in doc/ directory
 */
void FileTokenizer::tokenize(std::string* s, bool* done)
{
    int current;

    if( infile.good() ) { // good() if bad,fail and eof bit are not set

	do {
	    s->clear();
			
	    do {
//				std::cout << " ( " << infile.tellg() << ") ";
		current = infile.get();
//				std::cout << " ( " << infile.tellg() << ") ";

		if( isValid( current ) ) {
					
		    if( lowercase ) {
			current = tolower( current );
		    }

		    s->push_back( current );
		}
				
	    } while( !isSeparator( current ) && !infile.eof() );
			
	} while ( s->empty() && !infile.eof() );
		
    }
	
    if( infile.eof() )
	*done = true;

}



float FileTokenizer::getProgress()
{
    if( !infile.eof() ) {
	// calculate percentage done and return it
	unsigned long int curPos = infile.tellg();
	return ((static_cast<float>(curPos) / streamLength) * 100.0);

    } else {
	return 100;

    }
}

unsigned long int FileTokenizer::getStreamLength()
{
    // store current seek pointer position
    unsigned long int prevPos = infile.tellg();

    // position get pointer to end of file
    infile.seekg( 0, std::ios::end );
    // read end position
    unsigned long int endPos = infile.tellg();

    // reposition seek get pointer to original position
    infile.seekg( prevPos, std::ios::beg );

    // return how many bytes are in the stream
    return endPos;
}

unsigned long int FileTokenizer::getStreamPosition()
{
    return infile.tellg();
}
