
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

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <list>

const char PROGRAM_NAME[] = "soothsayerDemo";

void parseCommandLineArgs(int argc, char** argv);
void printUsage();
void printVersion();

void disclaimer();

void draw_title_win(WINDOW*);
void draw_history_win(WINDOW*, std::string);
void draw_function_keys(WINDOW*);
void draw_previous_suggestions(std::vector<std::string>, bool, const int, int);
int  getGreatestSuggestionLength(std::vector< std::string > suggestions);

const int SUGGESTIONS = 6;

/** Demo program using ncurses.
 *
 * This demo displays the text entered in a top windows that stretches
 * across the screen. The current prediction is displayed immediately
 * underneath the text window, at the leftmost position.
 *
 * The previous predictions are displayed in cronological order to the
 * right of the current prediction.
 *
 * Subsequent predictions shifted to the right, so that the current
 * prediction is always on the left hand side.
 * Context switches are marked in some way (either a vertical bar or a
 * box enclosing the other prediction boxes).
 *
 */
int main(int argc, char** argv)
{
    parseCommandLineArgs(argc, argv);

    // magic starts here
    Soothsayer soothsayer;

    // ncurses 
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    clear();
    refresh();

    disclaimer();

    // ncurses title window
    const int TITLE_WIN_HEIGHT  = 6;
    const int TITLE_WIN_WIDTH   = COLS;
    const int TITLE_WIN_BEGIN_Y = 0;
    const int TITLE_WIN_BEGIN_X = 0;
    WINDOW* title_win = newwin(TITLE_WIN_HEIGHT, TITLE_WIN_WIDTH, TITLE_WIN_BEGIN_Y, TITLE_WIN_BEGIN_X);
    draw_title_win(title_win);

    // ncurses history window
    const int HISTORY_WIN_HEIGHT  = 5;
    const int HISTORY_WIN_WIDTH   = COLS;
    const int HISTORY_WIN_BEGIN_Y = TITLE_WIN_BEGIN_Y + TITLE_WIN_HEIGHT + 1;
    const int HISTORY_WIN_BEGIN_X = 0;
    WINDOW* history_win = newwin(HISTORY_WIN_HEIGHT, HISTORY_WIN_WIDTH, HISTORY_WIN_BEGIN_Y, HISTORY_WIN_BEGIN_X);
    draw_history_win(history_win, std::string(""));

    // ncurses function keys window
    const int FUNCTION_WIN_HEIGHT  = 6 + 2;
    const int FUNCTION_WIN_WIDTH   = 4;
    const int FUNCTION_WIN_BEGIN_Y = HISTORY_WIN_BEGIN_Y + HISTORY_WIN_HEIGHT + 1;
    const int FUNCTION_WIN_BEGIN_X = 0;
    WINDOW* function_win = newwin(FUNCTION_WIN_HEIGHT, FUNCTION_WIN_WIDTH, FUNCTION_WIN_BEGIN_Y, FUNCTION_WIN_BEGIN_X);
    draw_function_keys(function_win);

    mvprintw(LINES - 1, 0, "Press F12 to quit.");
    refresh();


    std::vector<std::string> words;
    int c;
    char str[2];
    str[1] = '\0';
    do {
	c = getch();
	if ((KEY_F0 < c) && (c <= KEY_F(words.size())) && (c - KEY_F0 <= words.size())) {
	    // prediction was successful. user pressed the function
	    // key corresponding to desired token. selecting
	    // suggestion.
	    std::string message = "Last selected word: " + words[c - KEY_F0 - 1];
	    mvprintw(LINES - 3, 0, message.c_str());
            clrtoeol();
	    move(LINES, COLS);

	    // inform soothsayer that the prediction was successful.
	    soothsayer.complete(words[c - KEY_F0 - 1]);
	    // ask soothsayer to predict next token
	    words = soothsayer.predict(' ');

	} else {
	    // prediction unsuccessful. get next character from user
	    // and elaborate a new prediction.
	    str[0] = static_cast<char>( c );
	    words = soothsayer.predict(std::string(str));

	    // refresh ncurses screen
	    refresh();
	}
	draw_history_win(history_win, soothsayer.history());
	draw_previous_suggestions(words,
				  soothsayer.contextChange(),
                                  HISTORY_WIN_BEGIN_Y + HISTORY_WIN_HEIGHT + 1,
                                  FUNCTION_WIN_BEGIN_X + FUNCTION_WIN_WIDTH + 1 );
    } while( c != KEY_F(12) );


    delwin(title_win);
    delwin(history_win);
    delwin(function_win);
    endwin();

    return 0;
}


void draw_history_win(WINDOW* win, std::string str)
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

void draw_function_keys(WINDOW* win)
{
    wclear(win);
    box(win, 0, 0);
    for (int i = 1; i <= SUGGESTIONS; i++) {
        std::stringstream ss;
        ss << 'F' << i;
        mvwprintw(win, i, 1, ss.str().c_str());
    }
    wrefresh(win);
}

void draw_previous_suggestions(std::vector<std::string> words, bool contextChange,
			       const int starty, int startx)
{
    static std::list< std::vector<std::string> > previousSuggestions;
    static std::vector< WINDOW* > windows;

    // clear out existing windows
    for (std::vector< WINDOW* >::iterator winit = windows.begin();
         winit != windows.end();
         winit++) {
        wclear(*winit);
        wrefresh(*winit);
        delwin(*winit);
    }
    windows.clear();

    if (contextChange) {
	// insert a context change marker in the list of previous
	// suggestions
	// 
	std::vector< std::string > marker;
	for (int i = 0; i < 6; i++) {
	    marker.push_back("|");
	}
	previousSuggestions.insert(previousSuggestions.begin(), marker);
    }

    previousSuggestions.insert(previousSuggestions.begin(), words);

    for (std::list< std::vector<std::string> >::const_iterator listit = previousSuggestions.begin();
	 (listit != previousSuggestions.end() && startx < COLS); // don't draw window off the screen
	 listit++) {

	int height = listit->size() + 2;
	int width = getGreatestSuggestionLength(*listit) + 2;

	WINDOW* win = newwin(height, width, starty, startx);
	wclear(win);
	box(win, 0, 0);

	int line = 1;
	for (std::vector<std::string>::const_iterator strit = listit->begin();
	     strit != listit->end();
	     strit++) {
	    
	    mvwprintw(win, line, 1, strit->c_str());
	    line++;
	}

	wrefresh(win);
        windows.push_back(win);
	startx += width + 2;
    }
}

int getGreatestSuggestionLength(std::vector< std::string > suggestions)
{
    int result = 0;
    for (std::vector< std::string >::const_iterator it = suggestions.begin();
	 it != suggestions.end();
	 it++) {
	if (it->size() > result) {
	    result = it->size();
	}
    }
    return result;
}

void disclaimer()
{
    int topBottomBorder = (LINES/8);
    int borderWinHeight = 15;
    int borderWinWidth  = 70;
    int sideBorder      = (COLS - borderWinWidth) / 2;
    WINDOW* borderWin     = newwin(borderWinHeight, borderWinWidth, topBottomBorder, sideBorder);
    WINDOW* disclaimerWin = newwin(borderWinHeight-2, borderWinWidth-2, topBottomBorder+1, sideBorder+1);
    box(borderWin, 0, 0);
    wrefresh(borderWin);
    wprintw(disclaimerWin,
	    "Soothsayer Demo\n"
	    "---------------\n"
	    "This program is intended as a demonstration of Soothsayer ONLY.\n"
	    "\n"
	    "The Soothsayer project aims to provide an intelligent predictive\n"
	    "text entry platform.\n"
	    "\n"
	    "Its intent is NOT to provide a predictive text entry user interface.\n"
	    "Think of Soothsayer as the predictive backend that sits behind a\n"
	    "shiny user interface and does all the predictive heavy lifting.\n"
	);
    mvwprintw(disclaimerWin, (borderWinHeight-2)-1, 1, "Press any key to continue...");
    wrefresh(disclaimerWin);

    getch();

    delwin(disclaimerWin);
    delwin(borderWin);

    clear();
    refresh();
}

void draw_title_win(WINDOW* title_win)
{
    wclear(title_win);
    box(title_win, 0, 0);
    mvwprintw(title_win, 1, 1, "Soothsayer Demo ", VERSION);
    mvwprintw(title_win, 2, 1, "Copyright (C) Matteo Vescovi");
    mvwprintw(title_win, 3, 1, "This is free software; see the source for copying conditions.  There is NO");
    mvwprintw(title_win, 4, 1, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
    wrefresh(title_win);
}

void parseCommandLineArgs(int argc, char* argv[])
{
    int next_option;
	
    // getopt structures
    const char* const short_options = "hv";

    const struct option long_options[] = {
	{ "help",   0, NULL, 'h' },
	{ "version",0, NULL, 'v' },
	{ NULL,     0, NULL,   0 }
    };

    do {
	next_option = getopt_long( argc, argv, 
				   short_options, long_options, NULL );
		
	switch( next_option ) {
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
    std::cout << PACKAGE << " version " << VERSION << std::endl
	      << "Copyright (C) 2004 Matteo Vescovi." << std::endl
	      << "This is free software; see the source for copying conditions.  There is NO" << std::endl
	      << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE," << std::endl
	      << "to the extent permitted by law." << std::endl;
}

void printUsage()
{
    std::cout << "Usage: " << PROGRAM_NAME << std::endl
	      << std::endl
	      << "Begin typing. soothsayer will attempt to predict the desired word." << std::endl
	      << "After each keystroke, soothsayer will return a number of predictions." << std::endl
	      << "If the desired word appears in the prediction list, select it by pressing the" << std::endl
	      << "corresponding function key." << std::endl
	      << std::endl
	      << "  -h, --help     display this help and exit" << std::endl
	      << "  -v, --version  output version information and exit" << std::endl
	      << std::endl
	      << "Direct your bug reports to: " << PACKAGE_BUGREPORT << std::endl;
}
