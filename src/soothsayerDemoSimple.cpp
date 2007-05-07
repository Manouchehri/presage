
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>           *
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


#include "soothsayer.h"

#include <string>
#include <vector>
#include <ncurses.h>
#include <sstream>


void drawHistoryWin( WINDOW*, std::string );
void drawMsgWin( WINDOW*, std::vector<std::string> );

int main()
{
    Soothsayer soothsayer;

    initscr();
    noecho();
    cbreak();
    keypad( stdscr, TRUE );
    clear();
    refresh();

    WINDOW* historyWin;
    WINDOW* msgWin;

    historyWin = newwin( 5, COLS, 3, 0 );
    msgWin = newwin( 10, COLS, 10, 0 );

    mvprintw( 0, 0, "Soothsayer demo program" );
    mvprintw( LINES - 1, 0, "Press F12 to quit." );
    refresh();

    drawHistoryWin( historyWin, std::string( "" ) );

    std::vector<std::string> words;
    drawMsgWin( msgWin, words );

    int c;
    char str[2];
    str[1] = '\0';
    do {
	c = getch();
	str[0] = static_cast<char>( c );
	words = soothsayer.predict( std::string( str ) );
	drawHistoryWin( historyWin, soothsayer.history() );
	drawMsgWin( msgWin, words );

    } while( c != KEY_F(12) );

    endwin();

    return 0;
}


void drawHistoryWin( WINDOW* win, std::string str )
{
    wclear( win );
    box( win, 0, 0 );
    mvwprintw( win, 1, 1, str.c_str() );
    wrefresh( win );
}


void drawMsgWin( WINDOW* win, std::vector<std::string> words )
{
    wclear( win );
    box( win, 0, 0 );
	
    int i = 1;
    std::vector<std::string>::const_iterator j = words.begin();
    while( j != words.end() ) {
	mvwprintw( win, i, 1, j->c_str() );
	i++;
	j++;
    }

    wrefresh( win );
}
