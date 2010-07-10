
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


#include "presage.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* Solaris 10 needs to have NOMACROS defined to avoid conflict between
   curses and standard template library code.
 */
#ifndef NOMACROS
# define NOMACROS
# include <curses.h>
# undef NOMACROS
#else
# include <curses.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#include <getopt.h>

#include <iostream>
#include <sstream>
#include <list>

const char PROGRAM_NAME[] = "presage_demo";

// prototypes
//
void parseCommandLineArgs(int argc, char** argv);
void printUsage();
void printVersion();

void disclaimer();

void draw_title_win(WINDOW*);
void draw_context_win(WINDOW*, std::string);
void draw_function_keys(WINDOW*);
void draw_previous_suggestions(std::vector<std::string>, bool, const int, int);
size_t getGreatestSuggestionLength(std::vector< std::string > suggestions);

// globals
std::string       suggestions;
std::string       config;
std::stringstream buffer;    // text buffer, a real application would
			     // use something a little more
			     // sophisticated than a stringstream

/** Callback object for presage demo application.
 *  
 * We need to provide a callback class to allow presage to query the
 * application's text buffer. In a real world application, this would
 * fetch the text from whatever object stores the text composition
 * (i.e. a GUI widget in a graphical interface)
 *
 * For the purpose of this demonstration program, the callback class
 * will retrieve contextual data from a standard stringstream object.
 *
 */
class PresageDemoCallback : public PresageCallback {
public:
    PresageDemoCallback(std::stringstream& buffer) : m_buffer(buffer) { }

    std::string get_past_stream() const { return m_buffer.str(); }
    std::string get_future_stream() const { return empty; }

private:
    std::stringstream& m_buffer;
    const std::string empty;

};

/** Demo program using curses.
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
    PresageCallback* callback = new PresageDemoCallback(buffer); 
    Presage presage(callback, config);

    // configuration variable may be read and written programmatically
    if (suggestions.empty()) {
	suggestions = presage.config("Presage.Selector.SUGGESTIONS");
    } else {
	presage.config("Presage.Selector.SUGGESTIONS", suggestions);
    }

    // curses 
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    clear();
    refresh();

    disclaimer();

    // curses title window
    const int TITLE_WIN_HEIGHT  = 6;
    const int TITLE_WIN_WIDTH   = COLS;
    const int TITLE_WIN_BEGIN_Y = 0;
    const int TITLE_WIN_BEGIN_X = 0;
    WINDOW* title_win = newwin(TITLE_WIN_HEIGHT, TITLE_WIN_WIDTH, TITLE_WIN_BEGIN_Y, TITLE_WIN_BEGIN_X);
    draw_title_win(title_win);

    // curses context window
    const int CONTEXT_WIN_HEIGHT  = 5;
    const int CONTEXT_WIN_WIDTH   = COLS;
    const int CONTEXT_WIN_BEGIN_Y = TITLE_WIN_BEGIN_Y + TITLE_WIN_HEIGHT + 1;
    const int CONTEXT_WIN_BEGIN_X = 0;
    WINDOW* context_win = newwin(CONTEXT_WIN_HEIGHT, CONTEXT_WIN_WIDTH, CONTEXT_WIN_BEGIN_Y, CONTEXT_WIN_BEGIN_X);
    draw_context_win(context_win, std::string(""));

    // curses function keys window
    const int FUNCTION_WIN_HEIGHT  = atoi(suggestions.c_str()) + 2;
    const int FUNCTION_WIN_WIDTH   = 4;
    const int FUNCTION_WIN_BEGIN_Y = CONTEXT_WIN_BEGIN_Y + CONTEXT_WIN_HEIGHT + 1;
    const int FUNCTION_WIN_BEGIN_X = 0;
    WINDOW* function_win = newwin(FUNCTION_WIN_HEIGHT, FUNCTION_WIN_WIDTH, FUNCTION_WIN_BEGIN_Y, FUNCTION_WIN_BEGIN_X);
    draw_function_keys(function_win);

    mvprintw(LINES - 1, 0, "Press F12 to quit.");
    refresh();


    std::vector<std::string> words;
    unsigned int c = ' ';
    do {
	size_t size = words.size();
	if ((KEY_F0 < c) && (c <= KEY_F(size)) && (c - KEY_F0 <= size)) {
	    // prediction was successful. user pressed the function
	    // key corresponding to desired token. selecting
	    // suggestion.
	    std::string message = "Last selected word: " + words[c - KEY_F0 - 1];
	    mvprintw(LINES - 3, 0, message.c_str());
            clrtoeol();
	    move(LINES, COLS);

	    // update buffer with prediction completion
	    buffer << presage.completion(words[c - KEY_F0 - 1]);
	    // ask presage to predict next token
	    words = presage.predict();

	} else {
	    // prediction unsuccessful. get next character from user
	    // and elaborate a new prediction.
	    buffer << static_cast<char>(c);
	    words = presage.predict();

	    // refresh curses screen
	    refresh();
	}
	draw_context_win(context_win, presage.context());
	draw_previous_suggestions(words,
				  presage.context_change(),
                                  CONTEXT_WIN_BEGIN_Y + CONTEXT_WIN_HEIGHT + 1,
                                  FUNCTION_WIN_BEGIN_X + FUNCTION_WIN_WIDTH + 1 );
        c = getch();

    } while( c != KEY_F(12) );

    delwin(title_win);
    delwin(context_win);
    delwin(function_win);
    endwin();

    return 0;
}


void draw_context_win(WINDOW* win, std::string str)
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
    for (int i = 1; i <= atoi(suggestions.c_str()); i++) {
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
	for (int i = 0; i < atoi(suggestions.c_str()); i++) {
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

size_t getGreatestSuggestionLength(std::vector< std::string > suggestions)
{
    size_t result = 0;
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
	    "Presage Demo\n"
	    "------------\n"
	    "This program is intended as a demonstration of Presage ONLY.\n"
	    "\n"
	    "The Presage project aims to provide an intelligent predictive\n"
	    "text entry platform.\n"
	    "\n"
	    "Its intent is NOT to provide a predictive text entry user interface.\n"
	    "Think of Presage as the predictive backend that sits behind a\n"
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
    mvwprintw(title_win, 1, 1, "Presage Demo ", VERSION);
    mvwprintw(title_win, 2, 1, "Copyright (C) Matteo Vescovi");
    mvwprintw(title_win, 3, 1, "This is free software; see the source for copying conditions.  There is NO");
    mvwprintw(title_win, 4, 1, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
    wrefresh(title_win);
}

void parseCommandLineArgs(int argc, char* argv[])
{
    int next_option;
	
    // getopt structures
    const char* const short_options = "c:s:hv";

    const struct option long_options[] = {
        { "config",       required_argument, 0, 'c' },
	{ "suggestions",  required_argument, 0, 's' },
	{ "help",         no_argument,       0, 'h' },
	{ "version",      no_argument,       0, 'v' },
	{ 0, 0, 0, 0 }
    };

    do {
	next_option = getopt_long( argc, argv, 
				   short_options, long_options, NULL );
		
	switch( next_option ) {
        case 'c': // --config or -c option
            config = optarg;
            break;
	case 's': // --suggestions or -s option
	    suggestions = optarg;
	    break;
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
    std::cout << PROGRAM_NAME << " (" << PACKAGE << ") version " << VERSION << std::endl
	      << "Copyright (C) 2004 Matteo Vescovi." << std::endl
	      << "This is free software; see the source for copying conditions.  There is NO" << std::endl
	      << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE," << std::endl
	      << "to the extent permitted by law." << std::endl;
}

void printUsage()
{
    std::cout << "Usage: " << PROGRAM_NAME << " [OPTION]..." << std::endl
	      << std::endl
	      << "Begin typing. presage will attempt to predict the desired word." << std::endl
	      << "After each keystroke, presage will return a number of predictions." << std::endl
	      << "If the desired word appears in the prediction list, select it by pressing the" << std::endl
	      << "corresponding function key." << std::endl
	      << std::endl
              << "  -c, --config CONFIG  use config file CONFIG" << std::endl
              << "  -s, --suggestions N  set prediction size to N suggestions" << std::endl
	      << "  -h, --help           display this help and exit" << std::endl
	      << "  -v, --version        output version information and exit" << std::endl
	      << std::endl
	      << "Direct your bug reports to: " << PACKAGE_BUGREPORT << std::endl;
}
