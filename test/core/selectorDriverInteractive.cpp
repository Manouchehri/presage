
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

#include "core/selector.h"
#include "plugins/dictionaryPlugin.h"
#include <ncurses.h>

void print( const std::vector<std::string>& , WINDOW* );
void print( const StringSet&, WINDOW* );
void print( const Prediction&, WINDOW* );
void test( HistoryTracker&, Selector&, std::vector<std::string>&,
	   const char*, char*[], const int,
	   WINDOW*, WINDOW*, WINDOW*);


int main()
{

    initscr();
    cbreak();
    noecho();
    keypad( stdscr, TRUE );

    WINDOW* topWin = newwin( 4, COLS, 0, 0 );
    WINDOW* preWin = newwin( (LINES-7), ((COLS-5)/3), 6, 0 );
    WINDOW* selWin = newwin( (LINES-7), ((COLS-5)/3), 6, (COLS/3) );
    WINDOW* sugWin = newwin( (LINES-7), ((COLS-5)/3), 6, ( 2*(COLS/3) ) );

    wrefresh( topWin );
    wrefresh( preWin );
    wrefresh( sugWin );
    wrefresh( selWin );


    HistoryTracker historyTracker;
    Selector selector( historyTracker );
    DictionaryPlugin plugin( historyTracker );

    int c;
    std::vector<std::string> selection;

    while( ( c = getchar() ) != 27 ) {
		
	std::string u;
	u.assign( 1, static_cast<char>(c) );

	historyTracker.update( u );

	// update topWin
	wclear( topWin );
	mvwprintw( topWin, 0, 0, "Update char: %s", u.c_str() );

	mvwprintw( topWin, 1, 0, "HistoryTracer: %s",
		   historyTracker.getPastStream().c_str() );
	mvwprintw( topWin, 2, 0, "Old prefix: %s",
		   selector.getPrefix().c_str() );
	mvwprintw( topWin, 3, 0, "New prefix: %s", 
		   historyTracker.getPrefix().c_str() );
	wrefresh( topWin );

	// execute prediction
	Prediction* pPtr = new Prediction;
	*pPtr = plugin.predict();

	// execute selection
	selection = selector.select( *pPtr );

	// update preWin
	print( *pPtr, preWin );

	// update selWin
	print( selection, selWin );

	// update sugWin
	print( selector.getSuggestedWords(), sugWin );

	delete pPtr;
		
    }
	

    clear();
    endwin();

    return 0;
}


void print( const std::vector<std::string>& v, WINDOW* selWin )
{
    wclear( selWin );
    box( selWin, 0, 0 );

    mvwprintw( selWin, 0, 0, "Selection:" );

    for( unsigned int i=0; i<v.size(); i++ ) {
	mvwprintw( selWin, 2 + i, 1, "%s", v[i].data() );
    }

    wrefresh( selWin );
}

void print( const StringSet& s, WINDOW* sugWin )
{
    wclear( sugWin );
    box( sugWin, 0, 0 );	

    mvwprintw( sugWin, 0, 0, "Suggested words:" );
	
    StringSet::const_iterator i = s.begin();
    int offset = 0;
    while( i != s.end() ) {
	mvwprintw( sugWin, 2 + offset, 1, "%s", i->data() );
	i++;
	offset++;
    }

    wrefresh( sugWin );
}


void print( const Prediction& p, WINDOW* preWin )
{
    wclear( preWin );
    box( preWin, 0, 0 );
	
    mvwprintw( preWin, 0, 0, "Prediction:" );
	
    for( int i=0; i<p.getSize(); i++ ) {
	mvwprintw( preWin, 2 + i, 1, "%s", p.getSuggestion( i ).getWord().data() );
    }

    wrefresh( preWin );
}
