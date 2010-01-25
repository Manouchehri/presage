
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
#define PLAT_GTK 1
#include <ScintillaWidget.h>

#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include "presage.h"

#include "dirs.h"


char* glob_filename;
int   glob_function_keys_mode;

#define PROGRAM_NAME "gprompter"


// TODO: reuse previously allocated char* buffer
class ScintillaPresageCallback
    : public PresageCallback {
public:
    ScintillaPresageCallback(ScintillaObject* sci) { m_scintilla = sci; }
    virtual ~ScintillaPresageCallback() { }

    virtual std::string get_past_stream() const {
	TextRange range;
	range.chrg.cpMin = 0;
	range.chrg.cpMax = scintilla_send_message(m_scintilla,
						  SCI_GETCURRENTPOS,
						  0,
						  0);
	
	range.lpstrText = new char[range.chrg.cpMax - range.chrg.cpMin + 1];

	scintilla_send_message(m_scintilla,
			       SCI_GETTEXTRANGE,
			       0,
			       (sptr_t) &range);
	
	std::string result = range.lpstrText;
	delete range.lpstrText;
	return result;
    }

    virtual std::string get_future_stream() const {
	TextRange range;
	range.chrg.cpMin = scintilla_send_message(m_scintilla,
						  SCI_GETCURRENTPOS,
						  0,
						  0);
	range.chrg.cpMax = -1;
	
	range.lpstrText = new char[range.chrg.cpMax - range.chrg.cpMin + 1];

	scintilla_send_message(m_scintilla,
			       SCI_GETTEXTRANGE,
			       0,
			       (sptr_t) &range);
	
	std::string result = range.lpstrText;
	delete range.lpstrText;
	return result;
    }

private:
    ScintillaObject* m_scintilla;

};

static int exit_app(GtkWidget*w, GdkEventAny*e, gpointer p) {
   gtk_main_quit();
   return w||e||p||1;	// Avoid warnings
}

static void show_prediction(ScintillaObject* scintilla, Presage* presage)
{
    // TODO: this should go in separate function
    std::vector<std::string> result = presage->predict();
    std::stringstream ss;
    for (size_t i = 0; i < result.size(); i++)
    {
	if (glob_function_keys_mode)
	{
	    ss << 'F' << i + 1 << ' ';
	}
	ss << result[i] << '\t';
    }
    char* list = strdup (ss.str().c_str());

    if (list[strlen(list) - 1] == ' ') {
	list[strlen(list) - 1] = '\0';
    }
    // end TODO

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

    scintilla_send_message (scintilla,
			    SCI_USERLISTSHOW,
			    1,
			    (sptr_t) list);

    free (list);
}

static void on_char_added (SCNotification* nt, 
			   ScintillaObject* scintilla,
			   Presage* presage)
{
    g_print("on_char_added()\n");
 
    g_print("char: %c\n", nt->ch);
   
//    show_prediction(scintilla, presage);
}

static void on_update_ui (SCNotification* nt,
			  ScintillaObject* scintilla,
			  Presage* presage)
{
    g_print("on_update_ui()\n");

    show_prediction(scintilla, presage);
}

static void on_modified (SCNotification* nt,
			 ScintillaObject* scintilla,
			 Presage* presage)
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

static void on_user_list_selection(SCNotification* nt, ScintillaObject* scintilla, Presage* presage)
{
    char* selection;

    g_print("on_user_list_selection()\n");

    if (glob_function_keys_mode)
    {
	// strip function key from selection
	char* separator;

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

    std::string completion = presage->completion (selection);

    free (selection);

    uptr_t length = completion.size();
    sptr_t str = (sptr_t) completion.c_str();

    scintilla_send_message (scintilla,
			    SCI_ADDTEXT,
			    length,
			    str);

//    g_print("added selected text, now calling show_prediction()\n");
//    show_prediction (scintilla, presage);
}

static void on_key( SCNotification* nt,
		    ScintillaObject* scintilla,
		    Presage* presage )
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
    Presage* presage;
    ScintillaObject* scintilla;

    g_print("notify handler: ");

    presage = (Presage*) data;
    scintilla = SCINTILLA(editor);

    notification = (struct SCNotification*) lParam;
    switch (notification->nmhdr.code) {
    case SCN_PAINTED:
	g_print("on_painted()\n");
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
    
    g_print ("text size: %d\n", size);

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
    
    g_print ("text size: %d\n", size);

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

static void on_menu_view_word_wrap( GtkWidget* widget,
				    gpointer data )
{
    ScintillaObject* scintilla;
    GtkCheckMenuItem* check_menu_item;
    gboolean active;


    scintilla = SCINTILLA (data);
    check_menu_item = GTK_CHECK_MENU_ITEM (widget);
    active = gtk_check_menu_item_get_active (check_menu_item);

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
    static GdkPixbuf* logo = gdk_pixbuf_new_from_file (logo_image_full_filename, NULL);

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

static GtkWidget* create_menu_bar(GtkWidget* window, ScintillaObject* scintilla)
{
  GtkWidget* menubar;
  GtkWidget* menu;
  GtkWidget* menu_item;

  menubar = gtk_menu_bar_new ();

#define CREATE_STOCK_MENU_ITEM(stock_id, item, menu, callback, data) \
  item = gtk_image_menu_item_new_from_stock (stock_id, NULL);	     \
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);		     \
  g_signal_connect (G_OBJECT (item),				     \
                    "activate",					     \
		    G_CALLBACK (callback),			     \
		    (gpointer) data);				     \
  gtk_widget_show (item);

#define CREATE_SEPARATOR(item, menu)			\
  item = gtk_separator_menu_item_new ();		\
  gtk_menu_shell_append (GTK_MENU_SHELL(menu), item);	

  /* File */
  menu = gtk_menu_new ();

  CREATE_STOCK_MENU_ITEM(GTK_STOCK_NEW, menu_item, menu, on_menu_file_new, scintilla);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_OPEN, menu_item, menu, on_menu_file_open, scintilla);
  CREATE_SEPARATOR      (menu_item, menu);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_SAVE, menu_item, menu, on_menu_file_save, scintilla);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_SAVE_AS, menu_item, menu, on_menu_file_save_as, scintilla);
  CREATE_SEPARATOR      (menu_item, menu);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_QUIT, menu_item, menu, exit_app, NULL);

  menu_item = gtk_menu_item_new_with_mnemonic ("_File");
  gtk_widget_show (menu_item);

  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item), menu);

  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menu_item);

  
  /* Edit */
  menu = gtk_menu_new();

  CREATE_STOCK_MENU_ITEM(GTK_STOCK_UNDO, menu_item, menu, on_menu_edit_undo, scintilla);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_REDO, menu_item, menu, on_menu_edit_redo, scintilla);
  CREATE_SEPARATOR      (menu_item, menu);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_CUT, menu_item, menu, on_menu_edit_cut, scintilla);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_COPY, menu_item, menu, on_menu_edit_copy, scintilla);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_PASTE, menu_item, menu, on_menu_edit_paste, scintilla);
  CREATE_SEPARATOR      (menu_item, menu);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_SELECT_ALL, menu_item, menu, on_menu_edit_selectall, scintilla);

  menu_item = gtk_menu_item_new_with_mnemonic ("_Edit");
  gtk_widget_show (menu_item);

  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item), menu);

  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menu_item);


  /* View */
  menu = gtk_menu_new();

  CREATE_STOCK_MENU_ITEM(GTK_STOCK_ZOOM_IN, menu_item, menu, on_menu_view_larger_text_size, scintilla);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_ZOOM_OUT, menu_item, menu, on_menu_view_smaller_text_size, scintilla);
  CREATE_SEPARATOR      (menu_item, menu);
  /* create word wrap check menu item */
  menu_item = gtk_check_menu_item_new_with_mnemonic ("_Word Wrap");
  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menu_item), TRUE);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item),
                    "toggled",
		    G_CALLBACK (on_menu_view_word_wrap),
		    (gpointer) scintilla);
  gtk_widget_show (menu_item);
  /* done creating word wrap check menu item */

  menu_item = gtk_menu_item_new_with_mnemonic ("_View");
  gtk_widget_show (menu_item);

  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item), menu);

  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menu_item);


  /* Help */
  menu = gtk_menu_new();

  CREATE_STOCK_MENU_ITEM(GTK_STOCK_HELP, menu_item, menu, on_menu_help_contents, scintilla);
  CREATE_STOCK_MENU_ITEM(GTK_STOCK_ABOUT, menu_item, menu, on_menu_help_about, scintilla);

  menu_item = gtk_menu_item_new_with_mnemonic ("_Help");
  gtk_widget_show (menu_item);

  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item), menu);

  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menu_item);



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
   ScintillaPresageCallback* callback = new ScintillaPresageCallback(sci);
   Presage* presage = new Presage(callback);

   g_signal_connect (G_OBJECT (sci), SCINTILLA_NOTIFY,
		     G_CALLBACK (on_scintilla_notify), presage);

#define SSM(m, w, l) scintilla_send_message(sci, m, w, l)

   SSM(SCI_STYLECLEARALL, 0, 0);
   SSM(SCI_SETLEXER, SCLEX_NULL, 0);

   SSM(SCI_INSERTTEXT, 0, (sptr_t)
       "Some text to get star"
   );

   uptr_t text_length = SSM(SCI_GETTEXTLENGTH, 0, 0);
   SSM(SCI_GOTOPOS, text_length, 0);       /* position cursor at end */

   uptr_t height = atoi(presage->config("Presage.Selector.SUGGESTIONS").c_str());
   SSM(SCI_AUTOCSETMAXHEIGHT, height, 0);  /* set autocompletion box height */
   SSM(SCI_AUTOCSETSEPARATOR, '\t', 0);    /* set autocompletion separator */
   SSM(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);  /* set word wrapping */
   SSM(SCI_SETMARGINWIDTHN, 0, 0);         /* hide margin */
   SSM(SCI_SETMARGINWIDTHN, 1, 0);         /* hide margin */
   SSM(SCI_SETMARGINWIDTHN, 2, 0);         /* hide margin */
   SSM(SCI_SETMARGINWIDTHN, 3, 0);         /* hide margin */
   SSM(SCI_SETMARGINWIDTHN, 4, 0);         /* hide margin */

/* SETCODEPAGE to UTF8 produces garbled text on Win32/GTK
 */
#ifndef _WIN32
   SSM(SCI_SETCODEPAGE, SC_CP_UTF8, 0);    /* set UTF-8 Unicode */
#endif


   gtk_widget_show_all(window);
   gtk_widget_grab_focus(GTK_WIDGET(editor));
   gtk_main();

   return 0;
}
