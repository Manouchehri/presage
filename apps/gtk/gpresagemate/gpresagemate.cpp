/*********************************************************
 *  Presage, an extensible predictive text entry system
 *  ------------------------------------------------------
 *
 *  Copyright (C) 2008  John Hills 
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

/*
 * A simple GTK presage application that uses the X window system
 * XEvIE extension to intercept key presses and insert user selected
 * predictions into currently focused X windows application provided
 * the X server has a working XEvIE extension enabled.
 * 
 * /etc/X11/xorg.conf requires the following lines to enable XEvIE
 *   Section "Extensions"
 *       Option "XEVIE" "Enable"
 *   EndSection
 *
 *
 * Based on code by Matteo Vescovi and xeviedemo.c from 
 * http://freedesktop.org/wiki/Software/XEvIE
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_PTHREAD_H
# include <pthread.h>
#endif
#ifdef HAVE_X11_XLIB_H
# include <X11/Xlib.h>
#endif
#ifdef HAVE_X11_XPROTO_H
# include <X11/Xproto.h>
#endif
#ifdef HAVE_X11_X_H
# include <X11/X.h>
#endif
#ifdef HAVE_X11_EXTENSIONS_XEVIE_H
# include <X11/extensions/Xevie.h>
#endif
#ifdef HAVE_X11_XUTIL_H
# include <X11/Xutil.h>
#endif
#ifdef HAVE_X11_KEYSYMDEF_H
# include <X11/keysymdef.h>
#endif

#include <cstring>

#include <iostream>
#include <sstream>

#include <gtk/gtk.h>

#include "presage.h"

std::string suggestions;
std::string config;
unsigned int no_suggestions;

std::stringstream buffer;
class StringstreamPresageCallback : public PresageCallback {
public:
    StringstreamPresageCallback(std::stringstream& stream) : m_stream(stream) { }
    ~StringstreamPresageCallback() { }
    
    std::string get_past_stream() const { if (&m_stream) { return m_stream.str(); } else { return m_empty; } }
    std::string get_future_stream() const { return m_empty; }
    
private:
    std::stringstream& m_stream;
    const std::string m_empty;
};
StringstreamPresageCallback callback(buffer);
Presage presage(&callback);

Display *dpy;

static KeySym get_keysym (XEvent *ev) {
    XKeyEvent *key_ev;
    char buffer[64];
    int bufsize = 63;
    KeySym key;
    XComposeStatus compose;
    int char_count;

    key_ev = (XKeyEvent *)ev;

    char_count = XLookupString(key_ev, buffer, bufsize, &key, &compose);
    buffer[char_count] = '\0';

    printf ("        State: %x KeyCode: %x\n", key_ev->state & ShiftMask, key_ev->keycode);
    char_count = XLookupString(key_ev, buffer, bufsize, &key, &compose);
    buffer[char_count] = '\0';
    printf ("        Char Count: %d KeySym: %lu char: |%c|\n", char_count, key, buffer[0]);

    return key; 
}

void get_prediction( unsigned int key,
		     char* append_string,
		     int complete_flag,
		     GtkWidget* label )
{

    std::ostringstream os;
    std::string disp_string;
    std::string prediction;
    static std::vector< std::string > predictions;
    char disp_string_c[BUFSIZ];
    
    append_string[0] = 0;
 	
    if(complete_flag == TRUE){
    	// check that a prediction is available
    	if((key - XK_F1) < predictions.size()){
            prediction = presage.completion(predictions[key - XK_F1]);
            strcpy(append_string, prediction.c_str());

	    buffer << prediction;
        }
        key = ' ';
    }

    // cast key back to char in order to be inserted as a character on
    // the buffer stream
    //
    // REVISIT: this WILL break for Unicode chars
    // 
    buffer << (char) key;
    predictions = presage.predict();

    for (int i = 0; i < (int) predictions.size(); i++) {
        os << 'F' << i+1 << "   " << predictions[i] << std::endl;
    }

    disp_string = os.str();
    strcpy(disp_string_c, disp_string.c_str());
    
    if(disp_string_c[0] != 0) {

	/* get GTK thread lock */
	gdk_threads_enter ();
	
    	gtk_label_set_text(GTK_LABEL(label), disp_string_c );
	printf ("label:\n%s\n", disp_string_c);
	
	/* release GTK thread lock */
	gdk_threads_leave ();

    }
}

void init()
{
    int major, minor;

    // initialize presage
    if (suggestions.empty()) {
        suggestions = presage.config("Presage.Selector.SUGGESTIONS");
    } 
    else {
        presage.config("Presage.Selector.SUGGESTIONS", suggestions);
    }
    no_suggestions = atoi(suggestions.c_str());
    
    // initialize XEvIE
    dpy = XOpenDisplay(NULL);
    if(XevieStart(dpy))
     	printf("XevieStart(dpy) initialized\n");
    else {
     	printf("XevieStart(dpy) initialization failed\n Check that XEvIE X extension is enabled and that only one client is trying to do event interception\n");
     	exit(1);
    }

    XevieQueryVersion(dpy, &major, &minor);
    printf("XEvIE major = %d, minor = %d\n", major, minor);

    XevieSelectInput(dpy, KeyPressMask | KeyReleaseMask );
}

void shutdown()
{
    XevieEnd (dpy);
}

gint print_hi(gpointer ununsed) {
  printf("hi!\n");

  return TRUE;
}

void handle_x_event ( XEvent& event, 
		      GtkWidget* label )
{
    KeySym key;

    char completion_string_c[BUFSIZ];
    int completion_word_index = 0;
    int completion_flag = FALSE;

        switch(event.type) {
          case KeyPress:
            key = get_keysym(&event);

            if((isalpha(key) || isspace(key) || ispunct(key) || key == XK_BackSpace) && key < 256){
                XevieSendEvent(dpy, &event, XEVIE_UNMODIFIED);
                get_prediction(tolower(key), completion_string_c, FALSE, label);
                
                
                if(((key == ',') ||
                    (key == '.') ||
                    (key == '?') ||
                    (key == ';') ||
                    (key == ':')) && 
                   (completion_word_index == 0) && 
                   (completion_flag == TRUE)) {
					
                    event.xkey.keycode = XKeysymToKeycode(dpy, XK_BackSpace);
                    event.type = KeyPress; 
                    XSendEvent(dpy, InputFocus, TRUE, KeyPressMask, &event);
                    event.type = KeyRelease;
                    XSendEvent(dpy, InputFocus, TRUE, KeyReleaseMask, &event);
           
                    event.type = KeyPress; 
                    XSendEvent(dpy, InputFocus, TRUE, KeyPressMask, &event);
                    event.type = KeyRelease;
                    XSendEvent(dpy, InputFocus, TRUE, KeyReleaseMask, &event);
                    
                    event.xkey.keycode = XKeysymToKeycode(dpy, key);
                    event.type = KeyPress; 
                    XSendEvent(dpy, InputFocus, TRUE, KeyPressMask, &event);
                    event.type = KeyRelease;
                    XSendEvent(dpy, InputFocus, TRUE, KeyReleaseMask, &event);
                
                    event.xkey.keycode = XKeysymToKeycode(dpy, XK_space);
                    event.type = KeyPress; 
                    XSendEvent(dpy, InputFocus, TRUE, KeyPressMask, &event);
                    event.type = KeyRelease;
                    XSendEvent(dpy, InputFocus, TRUE, KeyReleaseMask, &event);
                }

                else if(isspace(key)) completion_word_index = 0;
                else completion_word_index++;
                
                completion_flag = FALSE;
            }
            
            // check for function keys
            else if(((key - XK_F1) >= 0) && ((key - XK_F1) <= no_suggestions-1)) {
            	
            	// inform presage that the prediction was successful.
               	// and ask presage to predict next token
	      get_prediction(tolower(key), completion_string_c, TRUE, label);
               
                char s[64];
                printf("%d\n", completion_word_index  );
                //  send the rest of the completion string and a ' ' to the client 
                while(completion_string_c[completion_word_index] != '\0'){               	
                    s[0] = completion_string_c[completion_word_index];
                    s[1] = '\0';
                    
                    event.xkey.keycode = XKeysymToKeycode(dpy, XStringToKeysym(s));               
                    event.type = KeyPress; 
                    XSendEvent(dpy, InputFocus, TRUE, KeyPressMask, &event);
                    event.type = KeyRelease;
                    XSendEvent(dpy, InputFocus, TRUE, KeyReleaseMask, &event);
               		
                    event.xkey.time++;
                          
                    completion_word_index++;
                }
                completion_word_index = 0;
                completion_flag = TRUE;
                
                event.xkey.keycode = XKeysymToKeycode(dpy, XK_space);
                event.type = KeyPress;
                XSendEvent(dpy, InputFocus, TRUE, KeyPressMask, &event);
                event.type = KeyRelease;
                XSendEvent(dpy, InputFocus, TRUE, KeyReleaseMask, &event);
            }
            else {   			
            	// anything else just send it to the client
            	XevieSendEvent(dpy, &event, XEVIE_UNMODIFIED);
                completion_word_index = 0;
                completion_flag = FALSE;
            }
            break;
            
          case KeyRelease:    			
            XevieSendEvent(dpy, &event, XEVIE_UNMODIFIED);
            break;
            
          default: 
            printf(" unknown event %x\n", event.type); 
            break;   
    	}
}

void* presage_thread (void* label)
{
    XEvent event;

    for (;;) {
	// get XEvIE event
        XNextEvent (dpy, &event);
      	printf ("Got XEvent\n");

	handle_x_event (event, GTK_WIDGET(label));

    }

    return NULL;
}

gboolean delete_event( GtkWidget *widget,
		       GdkEvent  *gdk_event,
		       gpointer   data )
{
    /* If you return FALSE in the "delete_event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    g_print ("delete event occurred\n");

    gtk_main_quit ();

    shutdown ();

    /* Change TRUE to FALSE and the main window will be destroyed with
     * a "delete_event". */
    return TRUE;
}

/* Another callback */
void destroy( GtkWidget *widget,
	      gpointer   data )
{
    g_print ("destroy event occurred\n");
}

int main ( int argc,
	   char *argv[] )
{ 
    GtkWidget *window;
    GtkWidget *label;
    pthread_t thread;

    /* init threads */
    g_thread_init (NULL);
    gdk_threads_init ();
    gdk_threads_enter ();

    /* init GTK+ */
    gtk_init (&argc, &argv);
    gtk_set_locale ();
    gtk_rc_parse ("~/.presagegtkrc");
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "gpresagemate");
    gtk_window_set_default_size (GTK_WINDOW(window), 150, 180);
    gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_NONE);

    /* When the window is given the "delete_event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
    g_signal_connect (G_OBJECT (window), "delete_event",
		      G_CALLBACK (delete_event), NULL);
    
    /* Here we connect the "destroy" event to a signal handler.  
     * This event occurs when we call gtk_widget_destroy() on the window,
     * or if we return FALSE in the "delete_event" callback. */
    g_signal_connect (G_OBJECT (window), "destroy",
		      G_CALLBACK (destroy), NULL);

    label = gtk_label_new ("gpresagemate");
    gtk_widget_set_name(label, "presage_suggestions");

    gtk_container_add (GTK_CONTAINER(window), label);

    gtk_widget_show  (window); 
    gtk_widget_show  (label);
    
    init ();

    if (0 != pthread_create (&thread,
			     NULL,
			     presage_thread,
			     label)) {
	perror("Failed to spawn presage thread");
	exit(2);
    }

    gtk_main ();

    /* clean up threads */
    gdk_threads_leave ();
    
    return 0;
}
