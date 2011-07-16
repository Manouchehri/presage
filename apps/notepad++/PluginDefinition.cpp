//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"

#include "presage.h"
#include <cstdio>


//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

static presage_t            presage;
static presage_error_code_t presage_status;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{

}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	presage_free (presage);
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//

	funcItem[CMD_PREDICT]._pFunc = predict;
    lstrcpy(funcItem[CMD_PREDICT]._itemName, TEXT("Predict"));
    funcItem[CMD_PREDICT]._pShKey = new ShortcutKey;
    funcItem[CMD_PREDICT]._pShKey->_isAlt = true;
    funcItem[CMD_PREDICT]._pShKey->_isCtrl = true;
    funcItem[CMD_PREDICT]._pShKey->_isShift = true;
    funcItem[CMD_PREDICT]._pShKey->_key = 'P';
    funcItem[CMD_PREDICT]._init2Check = false;

	funcItem[CMD_ABOUT]._pFunc = about;
    lstrcpy(funcItem[CMD_ABOUT]._itemName, TEXT("About"));
    funcItem[CMD_ABOUT]._pShKey = new ShortcutKey;
    funcItem[CMD_ABOUT]._pShKey->_isAlt = true;
    funcItem[CMD_ABOUT]._pShKey->_isCtrl = true;
    funcItem[CMD_ABOUT]._pShKey->_isShift = true;
    funcItem[CMD_ABOUT]._pShKey->_key = 'H';
    funcItem[CMD_ABOUT]._init2Check = false;

}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	delete funcItem[CMD_PREDICT]._pShKey;
	delete funcItem[CMD_ABOUT]._pShKey;
}


//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

static const char* get_past_stream (void* scintilla)
{
	HWND sci = (HWND) scintilla;

	static struct TextRange range;
    range.chrg.cpMin = 0;
    range.chrg.cpMax = ::SendMessage(sci,
					                 SCI_GETCURRENTPOS,
					                 0,
					                 0);

    free (range.lpstrText);
    range.lpstrText = (char*) malloc (range.chrg.cpMax - range.chrg.cpMin + 1);

    ::SendMessage(sci,
			      SCI_GETTEXTRANGE,
			      0,
			      (sptr_t) &range);
	
    return range.lpstrText;
}

static const char* get_future_stream (void* scintilla)
{
	HWND sci = (HWND) scintilla;

	static struct TextRange range;
    range.chrg.cpMin = ::SendMessage(sci,
			                         SCI_GETCURRENTPOS,
                                     0,
                                     0);
    range.chrg.cpMax = -1;

    free (range.lpstrText);
    range.lpstrText = (char*) malloc (range.chrg.cpMax - range.chrg.cpMin + 1);

    ::SendMessage(sci,
			      SCI_GETTEXTRANGE,
			      0,
			      (sptr_t) &range);
	
    return range.lpstrText;
}

static void init_presage (HWND sci)
{
	// create presage
	presage_status = presage_new (
		get_past_stream,
		sci,
		get_future_stream,
		sci,
		&presage
	);
	if (PRESAGE_OK != presage_status)
	{
		/* TODO: should handle this better */
		abort();
	}
}

static char* stringify_prediction (char** prediction)
{
	// TODO: remove hardcoding
	bool glob_function_keys_mode = true;
	
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
		nchars = sprintf (function_string,
				   "F%d ", i + 1);
		if (nchars >= function_string_len)
		{
		    /* realloc buffer */
		    function_string = (char*) realloc (function_string, nchars + 1);
		    if (function_string != NULL)
		    {
			function_string_len = nchars + 1;
			nchars = sprintf (function_string,
					   "F%d ", i + 1);
		    }
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

void predict()
{
    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND scintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

	if (!presage) {
		init_presage (scintilla);

		::SendMessage(scintilla, SCI_AUTOCSETSEPARATOR, '\t', 0);    /* set autocompletion separator */

		char* value = 0;
		if (PRESAGE_OK == presage_config (presage, "Presage.Selector.SUGGESTIONS", &value))
		{
			uptr_t height = atoi (value);
			presage_free_string (value);
			::SendMessage(scintilla, SCI_AUTOCSETMAXHEIGHT, height, 0);  /* set autocompletion box height */
		}

	}

/*
	const char *str = get_past_stream (sci);
	size_t wcstrsize = strlen(str) + 1;
    wchar_t* wcstr = (wchar_t*) malloc (wcstrsize * sizeof(wchar_t));

    // Convert char* string to a wchar_t* string.
    size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstr, wcstrsize, str, _TRUNCATE);

	::MessageBox(NULL, wcstr, TEXT("Presage Notepad++ past stream"), MB_OK);

	free (wcstr);
*/

	char** prediction = 0;
	char* list = 0;
	presage_predict (presage, &prediction);
	/*
	for (int i = 0; prediction[i] != 0; i++) {
		::MessageBoxA(NULL, (LPCSTR) prediction[i], "Prediction", MB_OK);
	}
	*/

	list = stringify_prediction (prediction);
	presage_free_string_array (prediction);

	/*
	::MessageBoxA(NULL, (LPCSTR) list, "Predictions", MB_OK);
	*/

	uptr_t completion_active;
	
	completion_active = ::SendMessage (scintilla,
						    SCI_AUTOCACTIVE,
						    0,
						    0);
	
	if (completion_active)
	{
	    ::SendMessage (scintilla,
				    SCI_AUTOCCANCEL,
				    0,
				    0);
	}
	
	::SendMessage (scintilla,
				SCI_USERLISTSHOW,
				1,
				(sptr_t) list);

	free (list);
}


void about()
{
    ::MessageBox(NULL, TEXT("Presage, the intelligent predictive text entry - Notepad++ plugin.\n\nCopyright (C) Matteo Vescovi"), TEXT("Presage Notepad++ plugin"), MB_OK);
}


static void on_user_list_selection(struct SCNotification* nt)
{
    char* selection;
    char* completion;

	// TODO: remove hardcoding
	bool glob_function_keys_mode = true;

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND scintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

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

    //g_print("selected text: %s\n", nt->text);

    if (PRESAGE_OK == presage_completion (presage, selection, &completion))
    {
		uptr_t length = strlen (completion);
		sptr_t str = (sptr_t) completion;

		::SendMessage(scintilla,
				SCI_ADDTEXT,
				length,
				str);
    }

    free (selection);
    presage_free_string (completion);

//    g_print("added selected text, now calling show_prediction()\n");
//    show_prediction (scintilla, presage);
}


static void on_update_ui(struct SCNotification* notification)
{
	predict();
}


static void on_char_added (struct SCNotification* notification)
{
	// noop
}


void on_notification (struct SCNotification* notification)
{
    switch (notification->nmhdr.code) {
		case SCN_PAINTED:
			/* g_print("on_painted()\n"); */
		break;
    case SCN_UPDATEUI:
		on_update_ui (notification);
		break;
    case SCN_MODIFIED:
		//on_modified(notification, scintilla, presage);
		break;
    case SCN_CHARADDED:
		on_char_added (notification);
		break;
    case SCN_USERLISTSELECTION:
		on_user_list_selection (notification);
		break;
    case SCN_AUTOCCANCELLED:
		//printf("on_autoccancelled()\n");
		break;
    case SCN_KEY:
		//on_key(notification, scintilla, presage);
		break;
    default:
		//printf("notification->nmhdr.code: %u\n", notification->nmhdr.code);
		break;
    }
}