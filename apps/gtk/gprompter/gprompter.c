
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


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <Scintilla.h>
#include <SciLexer.h>
#define GTK 1
#include <ScintillaWidget.h>

#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include "presage.h"

#include "dirs.h"


static char*       glob_filename;
static int         glob_function_keys_mode;
static const char* glob_autopunctuation_whitespace = " ";
static const char* glob_autopunctuation_chars = ".,;:'?!$%&";
static const long  glob_max_callback_context_range_size = 1024 * 2;

#define PROGRAM_NAME "gprompter"


static const char* get_past_stream (void* scintilla)
{
    ScintillaObject* sci = SCINTILLA(scintilla);
    static struct TextRange range;
    range.chrg.cpMin = 0;
    range.chrg.cpMax = scintilla_send_message(sci,
					      SCI_GETCURRENTPOS,
					      0,
					      0);
    range.chrg.cpMin = range.chrg.cpMax - glob_max_callback_context_range_size;
    if (range.chrg.cpMin < 0)
    {
	range.chrg.cpMin = 0;
    }

    free (range.lpstrText);  // safe to free on first call, static struct members init'ed to zero
    range.lpstrText = (char*) malloc (range.chrg.cpMax - range.chrg.cpMin + 1);
    /*
    g_print("malloc'ing block of size: %d\n", range.chrg.cpMax - range.chrg.cpMin + 1);
    */

    scintilla_send_message(sci,
			   SCI_GETTEXTRANGE,
			   0,
			   (sptr_t) &range);

    /*
    g_print("get_past_stream(): %s\n", range.lpstrText);
    */
	
    return range.lpstrText;
}

static const char* get_future_stream (void* scintilla)
{
    ScintillaObject* sci = SCINTILLA(scintilla);
    static struct TextRange range;
    range.chrg.cpMin = scintilla_send_message(sci,
					      SCI_GETCURRENTPOS,
					      0,
					      0);
    range.chrg.cpMax = scintilla_send_message(sci,
					      SCI_GETTEXTLENGTH,
					      0,
					      0);
    if (range.chrg.cpMax > range.chrg.cpMin + glob_max_callback_context_range_size)
    {
	range.chrg.cpMax = range.chrg.cpMin + glob_max_callback_context_range_size;
    }

    free (range.lpstrText);  // safe to free on first call, static struct members init'ed to zero
    range.lpstrText = (char*) malloc (range.chrg.cpMax - range.chrg.cpMin + 1);
    /*
    g_print("malloc'ing block of size: %d\n", range.chrg.cpMax - range.chrg.cpMin + 1);
    */

    scintilla_send_message(sci,
			   SCI_GETTEXTRANGE,
			   0,
			   (sptr_t) &range);

    /*
    g_print("get_future_stream(): %s\n", range.lpstrText);
    */

    return range.lpstrText;
}

static int exit_app(GtkWidget*w, GdkEventAny*e, gpointer p) {
   gtk_main_quit();
   return w||e||p||1;	// Avoid warnings
}

static char* stringify_prediction (char** prediction)
{
    size_t len = 0;
    size_t nchars = 0;

    size_t allocated = 128;
    char* result = (char*) malloc (sizeof(char) * allocated);

    if (result != NULL)
    {
	int i;
	char *newp = 0;
	char *wp = result;

	/* write into stringified prediction string */
	for (i = 0; prediction[i] != 0; i++)
	{
	    if (glob_function_keys_mode)
	    {
		size_t function_string_len = 4;
		char* function_string = (char*) malloc (sizeof(char) * function_string_len);

		/* stringify 'F' (i+1) ' ' into function_string */
		nchars = snprintf (function_string,
				   function_string_len,
				   "F%d ", i + 1);
		if (nchars >= function_string_len)
		{
		    /* realloc buffer */
		    char* tmp_string = (char*) realloc (function_string, nchars + 1);
		    if (tmp_string != NULL)
		    {
			function_string = tmp_string;
		    }
		    else
		    {
			/* if it cannot be reallocated, try malloc */
			free (function_string);
			function_string = malloc (sizeof(char) * (nchars + 1));
			if (function_string == NULL)
			{
			    /* we must be running out of memory... cannot recover */
			    return NULL;
			}
		    }
		    function_string_len = nchars + 1;
		    nchars = snprintf (function_string,
				       function_string_len,
				       "F%d ", i + 1);
		}

		/* realloc if necessary to write 'F\d+ ' into result */
		if (wp + function_string_len + 1 > result + allocated)
		{
		    allocated = (allocated + function_string_len) * 2;
		    newp = (char *) realloc (result, allocated);
		    if (newp == NULL)
		    {
			free (result);
			return NULL;
		    }
		    wp = newp + (wp - result);
		    result = newp;
		}

		/* write 'F\d+ ' into result */
		wp = (char*) memcpy (wp, function_string, function_string_len);
		wp += function_string_len - 1;

		free (function_string);
	    }


	    /* realloc if necessary to write 'prediction[i]\t' into result */
	    len = strlen (prediction[i]) + 1;
	    if (wp + len + 1 > result + allocated)
	    {
		allocated = (allocated + len) * 2;
		newp = (char *) realloc (result, allocated);
		if (newp == NULL)
		{
		    free (result);
		    return NULL;
		}
		wp = newp + (wp - result);
		result = newp;
	    }

	    /* write 'F\d+ ' into result */
	    wp = (char*) memcpy (wp, prediction[i], len - 1);
	    wp += len - 1;

	    *wp++ = '\t';
	}

	/* Terminate the result string */
	if (wp > result)
	{
	    *(wp - 1) = '\0';
	}
	else if (wp == result)
	{
	    *(wp++) = '\0';
	}

	/* Resize memory to the optimal size.  */
	newp = (char*) realloc (result, wp - result);
	if (newp != NULL)
	{
	    result = newp;
	}
    }
    
    return result;
}

static void show_prediction(ScintillaObject* scintilla, presage_t presage)
{
    char** prediction;
    char* list;
    presage_error_code_t result;
    
    result = presage_predict(presage, &prediction);
    if (PRESAGE_OK == result)
    {
	list = stringify_prediction (prediction);
	presage_free_string_array (prediction);
	
	g_print ("prediction: %s|\n", list);
	
	uptr_t completion_active;
	
	completion_active = scintilla_send_message (scintilla,
						    SCI_AUTOCACTIVE,
						    0,
						    0);
	
	if (completion_active)
	{
	    scintilla_send_message (scintilla,
				    SCI_AUTOCCANCEL,
				    0,
				    0);
	}

	if (strlen(list) > 0)
	{
	    scintilla_send_message (scintilla,
				    SCI_USERLISTSHOW,
				    1,
				    (sptr_t) list);
	}
	
	free (list);

    } else if (PRESAGE_SQLITE_EXECUTE_SQL_ERROR == result) {
        /* switch off learning */
	if (PRESAGE_OK == presage_config_set (presage, 
					      "Presage.Predictors.SmoothedNgramPredictor.LEARN",
					      "false"))
	{
	    /* notify user */
	    GtkWidget* dialog = gtk_message_dialog_new (NULL,
							GTK_DIALOG_DESTROY_WITH_PARENT,
							GTK_MESSAGE_ERROR,
							GTK_BUTTONS_CLOSE,
							"The adaptive text learning component of the presage predictive text entry engine has detected an error. Learning has been switched off.");
	    gtk_dialog_run (GTK_DIALOG (dialog));
	    gtk_widget_destroy (dialog);
        }
    }

    /* possible leak of prediction array of strings here */
}

static void on_char_added (struct SCNotification* nt, 
			   ScintillaObject* scintilla,
			   presage_t presage)
{
    g_print ("on_char_added()\n");
 
    g_print ("char: %c\n", nt->ch);

    /* if added char is an autopunctuation char */
    if (strchr (glob_autopunctuation_chars, nt->ch))
    {
        g_print ("autopunctuation char: %c\n", nt->ch);

        uptr_t curr_pos	= scintilla_send_message(scintilla,
						 SCI_GETCURRENTPOS,
						 0,
						 0);
	if (curr_pos > 1)
        {
            //g_print ("current pos: %d\n", curr_pos);

            uptr_t prev_pos = curr_pos - 2;
            uptr_t prev_char = scintilla_send_message(scintilla,
						      SCI_GETCHARAT,
						      prev_pos,
						      0);

	    //g_print ("prev_pos: %d\n", prev_pos);
	    //g_print ("prev_char: %c\n", prev_char);

            /* if previous char is a autopunctuation whitespace char */
            if (strchr (glob_autopunctuation_whitespace, prev_char))
            {
                g_print ("autopunctuation activated\n");

                char replacement[3] = { nt->ch, prev_char, 0 };

                scintilla_send_message(scintilla,
		                       SCI_SETSELECTION,
				       prev_pos,
				       curr_pos);

                scintilla_send_message(scintilla,
		                       SCI_REPLACESEL,
				       0,
				       (sptr_t) replacement);
            }
	}
    }
   
//    show_prediction(scintilla, presage);
}

static void on_update_ui (struct SCNotification* nt,
			  ScintillaObject* scintilla,
			  presage_t presage)
{
    g_print("on_update_ui()\n");

    show_prediction(scintilla, presage);
}

static void on_modified (struct SCNotification* nt,
			 ScintillaObject* scintilla,
			 presage_t presage)
{
    g_print("on_modified()\n");

    uptr_t result = scintilla_send_message (scintilla,
					    SCI_CANUNDO,
					    0,
					    0);
    if (result)
    {
	// enable undo menu item
	//gtk_widget_set_sensitive (file_edit_undo_item, TRUE);
    }
    else
    {
	// gray out undo menu item
	//gtk_widget_set_sensitive (file_edit_undo_item, FALSE);
    }

    result = scintilla_send_message (scintilla,
				     SCI_CANREDO,
				     0,
				     0);
    if (result)
    {
	// enable redo menu item
	//gtk_widget_set_sensitive (file_edit_redo_item, TRUE);
    }
    else
    {
	// gray out redo menu item
	//gtk_widget_set_sensitive (file_edit_redo_item, FALSE);
    }

//    if (nt->modificationType & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT))
//    {
//	g_print("about to call show_prediction() from on_modified()\n");
//	show_prediction(scintilla, presage);
//    }
}

static void on_user_list_selection(struct SCNotification* nt,
				   ScintillaObject* scintilla,
				   presage_t presage)
{
    char* selection;
    char* completion;

    g_print("on_user_list_selection()\n");

    if (glob_function_keys_mode)
    {
	// strip function key from selection
	const char* separator;

	separator = strchr (nt->text, ' ');
	if (NULL == separator)
	{
	    /* this happens when prediction is empty */
	    return;
	}
	selection = strdup (separator + 1);
    }
    else
    {
	selection = strdup (nt->text);
    }

    g_print("selected text: %s\n", nt->text);

    if (PRESAGE_OK == presage_completion (presage, selection, &completion))
    {
	uptr_t length = strlen (completion);
	sptr_t str = (sptr_t) completion;

	scintilla_send_message (scintilla,
				SCI_ADDTEXT,
				length,
				str);
    }

    free (selection);
    presage_free_string (completion);

//    g_print("added selected text, now calling show_prediction()\n");
//    show_prediction (scintilla, presage);
}

static void on_key( struct SCNotification* nt,
		    ScintillaObject* scintilla,
		    presage_t presage )
{
    g_print("on_key()\n");

    g_print("key: %i\n", nt->ch);

    if (glob_function_keys_mode)
    {
	uptr_t completion_active;

	completion_active = scintilla_send_message (scintilla,
						    SCI_AUTOCACTIVE,
						    0,
						    0);
	
	if (completion_active)
	{
	    /* figure out which function key it is */
	    int fkn;
	    char fk[4];
	    
	    fkn = nt->ch - GDK_F1 + 1;
	    g_print ("fkn: F%d\n", fkn);
	    
	    sprintf (fk, "F%d", fkn);
	    g_print ("fk: %s\n", fk);
	    
	    /* select autocompletion */
	    scintilla_send_message (scintilla,
				    SCI_AUTOCSELECT,
				    0,
				    (sptr_t) fk);
	    
	    /* and complete it */
	    scintilla_send_message (scintilla,
				    SCI_AUTOCCOMPLETE,
				    0,
				    0);
	}
    }
}

static gboolean on_scintilla_notify(GtkWidget *editor, gint wParam, gpointer lParam, gpointer data)
{
    struct SCNotification *notification;
    presage_t presage;
    ScintillaObject* scintilla;

    /*
    g_print("notify handler: ");
    */

    presage = (presage_t) data;
    scintilla = SCINTILLA(editor);

    notification = (struct SCNotification*) lParam;
    switch (notification->nmhdr.code) {
    case SCN_PAINTED:
        /* g_print("on_painted()\n"); */
	break;
    case SCN_UPDATEUI:
	on_update_ui(notification, scintilla, presage);
	break;
    case SCN_MODIFIED:
	on_modified(notification, scintilla, presage);
	break;
    case SCN_CHARADDED:
	on_char_added(notification, scintilla, presage);
	break;
    case SCN_USERLISTSELECTION:
	on_user_list_selection(notification, scintilla, presage);
	break;
    case SCN_AUTOCCANCELLED:
	g_print("on_autoccancelled()\n");
	break;
    case SCN_KEY:
	on_key(notification, scintilla, presage);
	break;
    default:
	g_print("notification->nmhdr.code: %u\n", notification->nmhdr.code);
	break;
    }

    return TRUE;
}

static void show_error_dialog( const char* message )
{
    GtkWidget* dialog;

    dialog = gtk_message_dialog_new (NULL,
				     GTK_DIALOG_DESTROY_WITH_PARENT,
				     GTK_MESSAGE_ERROR,
				     GTK_BUTTONS_CLOSE,
				     "%s",
				     message);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

static void save_to_file( ScintillaObject* scintilla,
			  const char* filename )
{
    FILE* file;

    file = fopen (filename, "w");
    if (file == NULL)
    {
	show_error_dialog ("error opening file");
    }
    else
    {
	uptr_t size;
	char* buffer;

	/* get text from scintilla */
	size = scintilla_send_message (scintilla,
				       SCI_GETTEXTLENGTH,
				       0,
				       0);

	/* allocate buffer */
	buffer = (char*) malloc (size + 1);
	if (buffer == NULL)
	{
	    show_error_dialog ("error allocating buffer");
	}
	else
	{
	    /* read text from scintilla into buffer */
	    scintilla_send_message (scintilla,
				    SCI_GETTEXT,
				    size + 1,
				    (sptr_t) buffer);

	    /* write buffer to file */
	    fwrite (buffer, size,  1, file);

	    /* TODO: check fwrite return value */
	}

	if (fclose (file))
	{
	    show_error_dialog("error closing file");
	}
    }
}

static void on_menu_file_new( GtkWidget* widget,
			      gpointer data )
{
    ScintillaObject* scintilla = SCINTILLA (data);

    free (glob_filename);

    scintilla_send_message (scintilla,
			    SCI_CLEARALL,
			    0,
			    0);
}

static void on_menu_file_open( GtkWidget* widget,
			       gpointer data)
{
    ScintillaObject* scintilla;
    GtkWidget *dialog;

    scintilla = SCINTILLA (data);

    dialog = gtk_file_chooser_dialog_new ("Open File",
//					  GTK_WINDOW (widget),
					  NULL,
					  GTK_FILE_CHOOSER_ACTION_OPEN,
					  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					  NULL);
    
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
	char* filename;
	FILE* file;

	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

	g_print ("filename: %s\n", filename);
	
	file = fopen (filename, "r");
	if (file)
	{
	    long int size;

	    /* read file size */
	    if (fseek (file, 0, SEEK_END))
	    {
		/* fseek failed */
		show_error_dialog ("fseek failed\n");
	    }
	    else
	    {
		size = ftell (file);
		if (size < 0)
		{
		    /* ftell failed */
		    show_error_dialog ("ftell failed\n");
		}
		else
		{
		    g_print("size: %ld\n", size);

		    char* buffer = (char*) malloc (sizeof(char) * size + 1);
		    if (buffer == NULL)
		    {
			/* malloc failed */
			show_error_dialog ("malloc failed\n");
		    }
		    else
		    {
			/* rewind file stream */
			rewind (file);
			
			/* read entire file into buffer */
			if (fread (buffer, size, 1, file) == 0)
			{
			    /* error occured while reading from file */
			    show_error_dialog ("error occured while reading from file\n");
			}
			else
			{
			    buffer[size] = '\0';
			    //g_print("buffer: %s\n", buffer);

			    /* buffer has entire contents of file
			       send it to scintilla */
			    scintilla_send_message (scintilla,
						    SCI_SETTEXT,
						    0,
						    (sptr_t) buffer);
			    glob_filename = strdup (filename);
			}
			
			free (buffer);
		    }
		}
	    }

	    if (fclose(file))
	    {
		/* error closing file */
		show_error_dialog ("error closing file\n");
	    }
	}
	else
	{
	    /* unable to open file */
	    show_error_dialog ("unable to open file\n");
	}

	g_free (filename);
    }
    
    gtk_widget_destroy (dialog);
}

static void on_menu_file_save_as( GtkWidget* widget,
				  gpointer data)
{
    GtkWidget *dialog;
    ScintillaObject* scintilla;
    
    scintilla = SCINTILLA (data);

    dialog = gtk_file_chooser_dialog_new ("Save File",
					  NULL,
					  GTK_FILE_CHOOSER_ACTION_SAVE,
					  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
					  NULL);
    //gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

/*
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), default_folder_for_saving);
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Untitled document");
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename_for_existing_document);
*/
    
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
	char *filename;
	
	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	save_to_file (scintilla, filename);
	g_free (filename);
    }
    
    gtk_widget_destroy (dialog);
}

static void on_menu_file_save( GtkWidget* widget,
			       gpointer data)
{
    ScintillaObject* scintilla = SCINTILLA (data);

    if (glob_filename == NULL)
    {
	on_menu_file_save_as (widget, data);
    }
    else
    {
	save_to_file (scintilla, glob_filename);
    }
}

static void on_menu_edit_undo( GtkWidget* widget,
			       gpointer data)
{
    ScintillaObject* scintilla = SCINTILLA (data);

    uptr_t can_undo = scintilla_send_message (scintilla,
					      SCI_CANUNDO,
					      0,
					      0);
    if (can_undo)
    {
	scintilla_send_message (scintilla,
				SCI_UNDO,
				0,
				0);
    }
}

static void on_menu_edit_redo( GtkWidget* widget,
			       gpointer data)
{
    ScintillaObject* scintilla = SCINTILLA (data);

    uptr_t can_redo = scintilla_send_message (scintilla,
					      SCI_CANREDO,
					      0,
					      0);
    if (can_redo)
    {
	scintilla_send_message (scintilla,
				SCI_REDO,
				0,
				0);
    }
}

static void on_menu_edit_cut( GtkWidget* widget,
			       gpointer data)
{
    ScintillaObject* scintilla = SCINTILLA (data);

    scintilla_send_message (scintilla,
			    SCI_CUT,
			    0,
			    0);
}

static void on_menu_edit_copy( GtkWidget* widget,
			       gpointer data)
{
    ScintillaObject* scintilla = SCINTILLA (data);

    scintilla_send_message (scintilla,
			    SCI_COPY,
			    0,
			    0);
}

static void on_menu_edit_paste( GtkWidget* widget,
			       gpointer data)
{
    ScintillaObject* scintilla = SCINTILLA (data);

    scintilla_send_message (scintilla,
			    SCI_PASTE,
			    0,
			    0);
}

static void on_menu_edit_selectall( GtkWidget* widget,
				    gpointer data)
{
    ScintillaObject* scintilla = SCINTILLA (data);

    uptr_t length = scintilla_send_message (scintilla,
					    SCI_GETTEXTLENGTH,
					    0,
					    0);
    
    scintilla_send_message (scintilla,
			    SCI_SETANCHOR,
			    0,
			    0);

    scintilla_send_message (scintilla,
			    SCI_SETCURRENTPOS,
			    length,
			    0);
}

static void on_menu_view_larger_text_size( GtkWidget* widget,
					   gpointer data )
{
    ScintillaObject* scintilla = SCINTILLA (data);

    uptr_t size = scintilla_send_message (scintilla,
					  SCI_STYLEGETSIZE,
					  STYLE_DEFAULT,
					  0);
    
    g_print ("text size: %lu\n", size);

    scintilla_send_message (scintilla,
			    SCI_STYLESETSIZE,
			    STYLE_DEFAULT,
			    size + 1);

    scintilla_send_message (scintilla,
			    SCI_STYLECLEARALL,
			    0,
			    0);
}

static void on_menu_view_smaller_text_size( GtkWidget* widget,
					    gpointer data )
{
    ScintillaObject* scintilla = SCINTILLA (data);

    uptr_t size = scintilla_send_message (scintilla,
					  SCI_STYLEGETSIZE,
					  STYLE_DEFAULT,
					  0);
    
    g_print ("text size: %lu\n", size);

    if (size > 1)
    {
	scintilla_send_message (scintilla,
				SCI_STYLESETSIZE,
				STYLE_DEFAULT,
				size - 1);
    }

    scintilla_send_message (scintilla,
			    SCI_STYLECLEARALL,
			    0,
			    0);
}

static void on_menu_view_word_wrap( GtkWidget* widget, gpointer data )
{
    ScintillaObject* scintilla;
    GtkToggleAction* toggle_action;
    gboolean active;


    scintilla = SCINTILLA (data);
    toggle_action = GTK_TOGGLE_ACTION (widget);
    active = gtk_toggle_action_get_active (toggle_action);

    g_print ("active: %d\n", active);

    if (active)
    {
	scintilla_send_message (scintilla,
				SCI_SETWRAPMODE,
				SC_WRAP_WORD,
				0);
    }
    else
    {
	scintilla_send_message (scintilla,
				SCI_SETWRAPMODE,
				SC_WRAP_NONE,
				0);
    }
}

static void on_menu_view_invert_colours( GtkWidget* widget, gpointer data )
{
    ScintillaObject* scintilla;
    GtkToggleAction* toggle_action;
    gboolean active;


    scintilla = SCINTILLA (data);
    toggle_action = GTK_TOGGLE_ACTION (widget);
    active = gtk_toggle_action_get_active (toggle_action);

    g_print ("invert colours active: %d\n", active);

    if (active)
    {
	scintilla_send_message (scintilla,
				SCI_STYLESETFORE,
				STYLE_DEFAULT,
				(255 | 255 << 8 | 255 << 16));
	scintilla_send_message (scintilla,
				SCI_STYLESETBACK,
				STYLE_DEFAULT,
				0);
	scintilla_send_message (scintilla,
				SCI_STYLECLEARALL,
				0,
				0);

    }
    else
    {
	scintilla_send_message (scintilla,
				SCI_STYLESETFORE,
				STYLE_DEFAULT,
				0);
	scintilla_send_message (scintilla,
				SCI_STYLESETBACK,
				STYLE_DEFAULT,
				(255 | 255 << 8 | 255 << 16));
	scintilla_send_message (scintilla,
				SCI_STYLECLEARALL,
				0,
				0);

    }

}

static void on_menu_help_contents( GtkWidget* widget,
				   gpointer data )
{
    GtkWidget* dialog = gtk_message_dialog_new (NULL,
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_CLOSE,
						"Sorry, help has not been implemented yet.");
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

static void on_menu_help_about( GtkWidget* widget,
				gpointer data )
{
    static const gchar* authors[] = {
      "Matteo Vescovi <matteo.vescovi@yahoo.co.uk>",
      NULL
    };
    static const gchar* comments = 
      "This program is intended as a demonstration of Presage ONLY.\n"
      "The Presage project aims to provide an intelligent predictive text entry "
      "platform. Its intent is NOT to provide a predictive text entry user interface.\n"
      "\n"
      "Think of Presage as the predictive backend that sits behind a shiny user "
      "interface and does all the predictive heavy lifting.\n";
    static const gchar* copyright = 
      "Copyright (C) Matteo Vescovi";
    static const gchar* license = 
      "This program is free software; you can redistribute it and/or modify\n"
      "it under the terms of the GNU General Public License as published by\n"
      "the Free Software Foundation; either version 2 of the License, or\n"
      "(at your option) any later version.\n"
      "\n"
      "This program is distributed in the hope that it will be useful,\n"
      "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
      "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
      "GNU General Public License for more details.\n"
      "\n"
      "You should have received a copy of the GNU General Public License along\n"
      "with this program; if not, write to the Free Software Foundation, Inc.,\n"
      "51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.\n";
    static const gchar* program_name = 
      "gprompter";
    static const gchar* title = 
      "About gprompter";
    static const gchar* website = 
      "http://presage.sourceforge.net";

    static const gchar* logo_image_full_filename = pkgdatadir "/presage.png";
    GdkPixbuf* logo = gdk_pixbuf_new_from_file (logo_image_full_filename, NULL);

    gtk_show_about_dialog (NULL, 
			   "artists", authors,
			   "authors", authors,
			   "comments", comments,
			   "copyright", copyright,
			   "documenters", authors,
			   "program-name", program_name,
			   "logo", logo,
			   "title", title,
			   "license", license,
			   "version", VERSION,
			   "website", website,
			   NULL);
}

/*


  "artists"                  GStrv*                : Read / Write
  "authors"                  GStrv*                : Read / Write
  "comments"                 gchar*                : Read / Write
  "copyright"                gchar*                : Read / Write
  "documenters"              GStrv*                : Read / Write
  "license"                  gchar*                : Read / Write
  "logo"                     GdkPixbuf*            : Read / Write
  "logo-icon-name"           gchar*                : Read / Write
  "program-name"             gchar*                : Read / Write
  "translator-credits"       gchar*                : Read / Write
  "version"                  gchar*                : Read / Write
  "website"                  gchar*                : Read / Write
  "website-label"            gchar*                : Read / Write
  "wrap-license"             gboolean              : Read / Write
*/


static GtkWidget* create_menu_bar (GtkWidget* window, ScintillaObject* scintilla)
{
    GtkActionGroup      *action_group;          /* Packing group for our Actions */
    GtkUIManager        *menu_manager;          /* The magic widget! */
    GError              *error;                 /* For reporting exceptions or errors */
    GtkWidget           *menubar;               /* The actual menubar */    

    const gchar* const xml =
	"<ui>"
	"  <menubar name=\"MainMenu\">"
	"    <menu name=\"FileMenu\" action=\"FileMenuAction\">"
	"      <menuitem name=\"New\" action=\"FileMenuNewAction\" />"
	"      <menuitem name=\"Open\" action=\"FileMenuOpenAction\" />"
	"      <separator />"
	"      <menuitem name=\"Save\" action=\"FileMenuSaveAction\" />"
	"      <menuitem name=\"SaveAs\" action=\"FileMenuSaveAsAction\" />"
	"      <separator />"
	"      <menuitem name=\"Quit\" action=\"FileMenuQuitAction\" />"
	"      <placeholder name=\"FileMenuAdditions\" />"
	"    </menu>"
	"    <menu name=\"EditMenu\" action=\"EditMenuAction\">"
	"      <menuitem name=\"Undo\" action=\"EditMenuUndoAction\" />"
	"      <menuitem name=\"Redo\" action=\"EditMenuRedoAction\" />"
	"      <separator />"
	"      <menuitem name=\"Cut\" action=\"EditMenuCutAction\" />"
	"      <menuitem name=\"Copy\" action=\"EditMenuCopyAction\" />"
	"      <menuitem name=\"Paste\" action=\"EditMenuPasteAction\" />"
	"      <separator />"
	"      <menuitem name=\"SelectAll\" action=\"EditMenuSelectAllAction\" />"
	"    </menu>"
	"    <menu name=\"ViewMenu\" action=\"ViewMenuAction\">"
	"      <menuitem name=\"ZoomIn\" action=\"ViewMenuZoomInAction\" />"
	"      <menuitem name=\"ZoomOut\" action=\"ViewMenuZoomOutAction\" />"
	"      <separator />"
	"      <menuitem name=\"WordWrap\" action=\"ViewMenuWordWrapAction\" />"
	"      <separator />"
	"      <menuitem name=\"InvertColours\" action=\"ViewMenuInvertColoursAction\" />"
	"    </menu>"
	"    <menu name=\"HelpMenu\" action=\"HelpMenuAction\">"
	"      <menuitem name=\"Contents\" action=\"HelpMenuContentsAction\" />"
	"      <menuitem name=\"About\" action=\"HelpMenuAboutAction\" />"
	"    </menu>"
	"  </menubar>"
	"</ui>";


    static GtkActionEntry action_entries[] = 
	{
	    { "FileMenuAction", NULL, "_File" },   /* name, stock id, label */
	    { "EditMenuAction", NULL, "_Edit" },
	    { "ViewMenuAction", NULL, "_View" },
	    { "HelpMenuAction", NULL, "_Help" },
	
	    { "FileMenuNewAction", GTK_STOCK_NEW,      /* name, stock id */
	      "_New", "<control>N",                    /* label, accelerator */
	      "Create a new document",                 /* tooltip */ 
	      G_CALLBACK (on_menu_file_new) },
	
	    { "FileMenuOpenAction", GTK_STOCK_OPEN,
	      "_Open","<control>O",  
	      "Open a file",
	      G_CALLBACK (on_menu_file_open) },
	
	    { "FileMenuSaveAction", GTK_STOCK_SAVE,
	      "_Save","<control>S",  
	      "Save the current file",
	      G_CALLBACK (on_menu_file_save) },
	
	    { "FileMenuSaveAsAction", GTK_STOCK_SAVE_AS,
	      "Save _As","<shift><control>S",  
	      "Save the current file with a different name",
	      G_CALLBACK (on_menu_file_save_as) },
	
	    { "FileMenuQuitAction", GTK_STOCK_QUIT,
	      "_Quit", "<control>Q",    
	      "Quit the program",
	      G_CALLBACK (exit_app) },
	
	    { "EditMenuUndoAction", GTK_STOCK_UNDO,
	      "_Undo","<control>Z",  
	      "Undo the last action",
	      G_CALLBACK (on_menu_edit_undo) },
	
	    { "EditMenuRedoAction", GTK_STOCK_REDO,
	      "_Redo","<shift><control>Z",  
	      "Redo the last undone action",
	      G_CALLBACK (on_menu_edit_redo) },
	
	    { "EditMenuCutAction", GTK_STOCK_CUT,
	      "Cu_t","<control>X",  
	      "Cut the selection",
	      G_CALLBACK (on_menu_edit_cut) },
	
	    { "EditMenuCopyAction", GTK_STOCK_COPY,
	      "_Copy","<control>C",  
	      "Copy the selection",
	      G_CALLBACK (on_menu_edit_copy) },
	
	    { "EditMenuPasteAction", GTK_STOCK_PASTE,
	      "_Paste","<control>V",  
	      "Paste the clipboard",
	      G_CALLBACK (on_menu_edit_paste) },
	
	    { "EditMenuSelectAllAction", GTK_STOCK_SELECT_ALL,
	      "Select _All","<control>A",  
	      "Select the entire contents",
	      G_CALLBACK (on_menu_edit_selectall) },
	
	    { "ViewMenuZoomInAction", GTK_STOCK_ZOOM_IN,
	      "Zoom _In","<control>plus",  
	      "Zoom in",
	      G_CALLBACK (on_menu_view_larger_text_size) },
	
	    { "ViewMenuZoomOutAction", GTK_STOCK_ZOOM_OUT,
	      "Zoom _Out","<control>minus",  
	      "Zoom out",
	      G_CALLBACK (on_menu_view_smaller_text_size) },
	
	    { "HelpMenuContentsAction", GTK_STOCK_HELP,
	      "_Contents","",
	      "Open the manual",
	      G_CALLBACK (on_menu_help_contents) },
	
	    { "HelpMenuAboutAction", GTK_STOCK_ABOUT,
	      "_About","",  
	      "About this application",
	      G_CALLBACK (on_menu_help_about) }
	
	};
    static guint n_action_entries = G_N_ELEMENTS (action_entries);


    GtkToggleActionEntry toggle_action_entries[] = 
	{
	    { "ViewMenuWordWrapAction", NULL,
	      "Word _Wrap","<control>W",
	      "Wrap lines",
	      G_CALLBACK (on_menu_view_word_wrap),
	      TRUE },

	    { "ViewMenuInvertColoursAction", NULL,
	      "Invert _Colours","<control>I",
	      "Invert colours",
	      G_CALLBACK (on_menu_view_invert_colours),
	      FALSE }

	};
    static guint n_toggle_action_entries = G_N_ELEMENTS (toggle_action_entries);
    

    action_group = gtk_action_group_new ("MenuActions");
    gtk_action_group_set_translation_domain (action_group, "blah");
    gtk_action_group_add_actions (action_group, action_entries, n_action_entries, scintilla);
    gtk_action_group_add_toggle_actions (action_group, toggle_action_entries, n_toggle_action_entries, scintilla);

    menu_manager = gtk_ui_manager_new ();
    gtk_ui_manager_insert_action_group (menu_manager, action_group, 0);
    
    /* Read in the UI from our XML file */
    error = NULL;
    gtk_ui_manager_add_ui_from_string (menu_manager, xml, -1, &error);
    
    if (error)
    {
	g_message ("building menus failed: %s", error->message);
	g_error_free (error);
    }
    
    /* Get the menubar */
    menubar = gtk_ui_manager_get_widget (menu_manager, "/MainMenu");

    /* Make sure that the accelerators work */
    gtk_window_add_accel_group (GTK_WINDOW (window), 
				gtk_ui_manager_get_accel_group (menu_manager));

    return menubar;
}


void print_version()
{
    printf (
	PROGRAM_NAME " (" PACKAGE ") version " VERSION "\n"
	"Copyright (C) 2009 Matteo Vescovi.\n"
	"This is free software; see the source for copying conditions.  There is NO\n"
	"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE,\n"
	"to the extent permitted by law.\n"
	);
}

void print_usage()
{
    printf (
	"Usage: " PROGRAM_NAME " [OPTION]...\n"
	"\n"
	"Gprompter is a cross-platform predictive text editor.\n"
	"Gprompter displays predictions in a contextual pop-up box as each letter is typed.\n"
	"Predictions can be easily selected and inserted in the document.\n"
	"\n"
	"  -h, --help           display this help and exit\n"
	"  -v, --version        output version information and exit\n"
	"\n"
	"Direct your bug reports to: " PACKAGE_BUGREPORT "\n"
	);
}

void parse_cmd_line_args(int argc, char* argv[])
{
    int next_option;

    // getopt structures
    const char* const short_options = "hv";

    const struct option long_options[] = {
	{ "help",         no_argument,       0, 'h' },
	{ "version",      no_argument,       0, 'v' },
	{ 0, 0, 0, 0 }
    };

    do {
        next_option = getopt_long (argc, argv, 
                                   short_options, long_options, NULL);

        switch( next_option ) {
	case 'h': // --help or -h option
	    print_usage();
	    exit (0);
	    break;
	case 'v': // --version or -v option
	    print_version();
	    exit (0);
	    break;
	case '?': // unknown option
	    print_usage();
	    exit (0);
	    break;
	case -1:
	    break;
	default:
	    abort();
	    break;
	}

    } while( next_option != -1 );
}


int main(int argc, char **argv) {
   GtkWidget* window;
   GtkWidget* vbox;
   GtkWidget* menubar;
   GtkWidget* editor;
   ScintillaObject* sci;
   presage_t presage;
   presage_error_code_t presage_code;
   char* value;

   parse_cmd_line_args (argc, argv);

   /* init globals */
   glob_filename = NULL;
   glob_function_keys_mode = 1;

   gtk_init(&argc, &argv);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "gprompter");
   gtk_signal_connect(GTK_OBJECT(window), "delete_event",
		      GTK_SIGNAL_FUNC(exit_app), 0);

   /* A vbox to put a menu and a button in: */
   vbox = gtk_vbox_new (FALSE, 0);
   gtk_container_add (GTK_CONTAINER (window), vbox);
   gtk_widget_show (vbox);

   /* create scintilla */
   editor = scintilla_new();
   sci = SCINTILLA(editor);
   scintilla_set_id(sci, 0);
   gtk_widget_set_usize(editor, 500, 300);

   /* create menubar */
   menubar = create_menu_bar(window, sci);

   /* pack menubar and scintilla */
   gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);
   gtk_widget_show (menubar);
   gtk_box_pack_start (GTK_BOX (vbox), editor, TRUE, TRUE, 0);
   gtk_widget_show (editor);

   /* create presage */
   presage_code = presage_new (get_past_stream,
			       sci,
			       get_future_stream,
			       sci,
			       &presage);
   if (PRESAGE_OK != presage_code)
   {
       /* should handle this better */
       return presage_code;
   }

   g_signal_connect (G_OBJECT (sci), SCINTILLA_NOTIFY,
		     G_CALLBACK (on_scintilla_notify), presage);

#define SSM(m, w, l) scintilla_send_message(sci, m, w, l)

   SSM(SCI_SETLEXER, SCLEX_NULL, 0);

   if (PRESAGE_OK == presage_config (presage, "Presage.Selector.SUGGESTIONS", &value))
   {
       uptr_t height = atoi (value);
       presage_free_string (value);
       SSM(SCI_AUTOCSETMAXHEIGHT, height, 0);  /* set autocompletion box height */
   }
   SSM(SCI_AUTOCSETSEPARATOR, '\t', 0);    /* set autocompletion separator */
   SSM(SCI_AUTOCSETORDER, SC_ORDER_CUSTOM, 0); /* set autocompletion order */
   SSM(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);  /* set word wrapping */
   SSM(SCI_SETMARGINWIDTHN, 0, 0);         /* hide margin */
   SSM(SCI_SETMARGINWIDTHN, 1, 0);         /* hide margin */
   SSM(SCI_SETMARGINWIDTHN, 2, 0);         /* hide margin */
   SSM(SCI_SETMARGINWIDTHN, 3, 0);         /* hide margin */
   SSM(SCI_SETMARGINWIDTHN, 4, 0);         /* hide margin */

   SSM(SCI_SETCODEPAGE, SC_CP_UTF8, 0);    /* set UTF-8 Unicode */

   SSM(SCI_INSERTTEXT, 0, (sptr_t)         /* insert some text */
       "Some text to get star"
   );

   uptr_t text_length = SSM(SCI_GETTEXTLENGTH, 0, 0);
   SSM(SCI_GOTOPOS, text_length, 0);       /* position cursor at end */
   
   gtk_widget_show_all(window);
   gtk_widget_grab_focus(GTK_WIDGET(editor));
   gtk_main();

   return 0;
}
