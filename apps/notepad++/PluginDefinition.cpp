
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
    Copyright (C) 2003  Don HO <donho@altern.org>

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


#include "PluginDefinition.h"
#include "PresageUplink.h"
#include "menuCmdID.h"

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

static bool        glob_presage_enabled = false;
static bool        glob_learn_mode = true;
static bool        glob_autopunctuation = true;
static const char* glob_autopunctuation_whitespace = " ";
static const char* glob_autopunctuation_chars = ".,;:'?!$%&";
static bool        glob_function_keys_mode = false;
static const long  glob_max_callback_context_range_size = 2048;

extern HINSTANCE hInstLib;

extern PFUNC_presage_completion presage_completion;
extern PFUNC_presage_config presage_config;
extern PFUNC_presage_config_set presage_config_set;
extern PFUNC_presage_context presage_context;
extern PFUNC_presage_context_change presage_context_change;
extern PFUNC_presage_free presage_free;
extern PFUNC_presage_free_string presage_free_string;
extern PFUNC_presage_free_string_array presage_free_string_array;
extern PFUNC_presage_new presage_new;
extern PFUNC_presage_new_with_config presage_new_with_config;
extern PFUNC_presage_predict presage_predict;
extern PFUNC_presage_prefix presage_prefix;
extern PFUNC_presage_save_config presage_save_config;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
    glob_presage_enabled = LoadPresageDLL();
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
    UnloadPresageDLL();
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//

    funcItem[CMD_ENABLED]._pFunc = on_enable;
    lstrcpy(funcItem[CMD_ENABLED]._itemName, TEXT("Enable"));
    funcItem[CMD_ENABLED]._pShKey = new ShortcutKey;
    funcItem[CMD_ENABLED]._pShKey->_isAlt = true;
    funcItem[CMD_ENABLED]._pShKey->_isCtrl = true;
    funcItem[CMD_ENABLED]._pShKey->_isShift = true;
    funcItem[CMD_ENABLED]._pShKey->_key = 'E';
    funcItem[CMD_ENABLED]._init2Check = glob_presage_enabled;

    funcItem[CMD_LEARN_MODE]._pFunc = on_learn_mode;
    lstrcpy(funcItem[CMD_LEARN_MODE]._itemName, TEXT("Learn mode"));
    funcItem[CMD_LEARN_MODE]._pShKey = new ShortcutKey;
    funcItem[CMD_LEARN_MODE]._pShKey->_isAlt = true;
    funcItem[CMD_LEARN_MODE]._pShKey->_isCtrl = true;
    funcItem[CMD_LEARN_MODE]._pShKey->_isShift = true;
    funcItem[CMD_LEARN_MODE]._pShKey->_key = 'L';
    funcItem[CMD_LEARN_MODE]._init2Check = glob_learn_mode;

    funcItem[CMD_AUTOPUNCTUATION]._pFunc = on_autopunctuation;
    lstrcpy(funcItem[CMD_AUTOPUNCTUATION]._itemName, TEXT("Autopunctuation"));
    funcItem[CMD_AUTOPUNCTUATION]._pShKey = new ShortcutKey;
    funcItem[CMD_AUTOPUNCTUATION]._pShKey->_isAlt = true;
    funcItem[CMD_AUTOPUNCTUATION]._pShKey->_isCtrl = true;
    funcItem[CMD_AUTOPUNCTUATION]._pShKey->_isShift = true;
    funcItem[CMD_AUTOPUNCTUATION]._pShKey->_key = 'A';
    funcItem[CMD_AUTOPUNCTUATION]._init2Check = glob_autopunctuation;

    funcItem[CMD_SEPARATOR_1]._pFunc = NULL;
    lstrcpy(funcItem[CMD_SEPARATOR_1]._itemName, TEXT("-----------"));
    funcItem[CMD_SEPARATOR_1]._pShKey = NULL;

    funcItem[CMD_PREDICT]._pFunc = on_predict;
    lstrcpy(funcItem[CMD_PREDICT]._itemName, TEXT("Predict"));
    funcItem[CMD_PREDICT]._pShKey = new ShortcutKey;
    funcItem[CMD_PREDICT]._pShKey->_isAlt = true;
    funcItem[CMD_PREDICT]._pShKey->_isCtrl = true;
    funcItem[CMD_PREDICT]._pShKey->_isShift = true;
    funcItem[CMD_PREDICT]._pShKey->_key = 'P';
    funcItem[CMD_PREDICT]._init2Check = false;

    funcItem[CMD_SEPARATOR_2]._pFunc = NULL;
    lstrcpy(funcItem[CMD_SEPARATOR_2]._itemName, TEXT("-----------"));
    funcItem[CMD_SEPARATOR_2]._pShKey = NULL;

    funcItem[CMD_ABOUT]._pFunc = on_about;
    lstrcpy(funcItem[CMD_ABOUT]._itemName, TEXT("About"));
    funcItem[CMD_ABOUT]._pShKey = new ShortcutKey;
    funcItem[CMD_ABOUT]._pShKey->_isAlt = true;
    funcItem[CMD_ABOUT]._pShKey->_isCtrl = true;
    funcItem[CMD_ABOUT]._pShKey->_isShift = true;
    funcItem[CMD_ABOUT]._pShKey->_key = 'B';
    funcItem[CMD_ABOUT]._init2Check = false;

    funcItem[CMD_HELP]._pFunc = on_help;
    lstrcpy(funcItem[CMD_HELP]._itemName, TEXT("Help"));
    funcItem[CMD_HELP]._pShKey = new ShortcutKey;
    funcItem[CMD_HELP]._pShKey->_isAlt = true;
    funcItem[CMD_HELP]._pShKey->_isCtrl = true;
    funcItem[CMD_HELP]._pShKey->_isShift = true;
    funcItem[CMD_HELP]._pShKey->_key = 'H';
    funcItem[CMD_HELP]._init2Check = false;

}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
    delete funcItem[CMD_ENABLED]._pShKey;
    delete funcItem[CMD_LEARN_MODE]._pShKey;
    delete funcItem[CMD_AUTOPUNCTUATION]._pShKey;
    delete funcItem[CMD_PREDICT]._pShKey;
    delete funcItem[CMD_ABOUT]._pShKey;
    delete funcItem[CMD_HELP]._pShKey;
}


//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

static const char* get_past_stream (void* scintilla)
{
    HWND sci = (HWND) scintilla;

    static struct TextRange range;
    range.chrg.cpMax = ::SendMessage(sci,
                                     SCI_GETCURRENTPOS,
                                     0,
                                     0);
    range.chrg.cpMin = range.chrg.cpMax - glob_max_callback_context_range_size;
    if (range.chrg.cpMin < 0)
    {
	range.chrg.cpMin = 0;
    }

    free (range.lpstrText);
    range.lpstrText = (char*) malloc (range.chrg.cpMax - range.chrg.cpMin + 1);
    if (range.lpstrText != NULL)
    {
        ::SendMessage(sci,
                      SCI_GETTEXTRANGE,
                      0,
                      (sptr_t) &range);
    }
        
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
    range.chrg.cpMax = ::SendMessage(sci,
                                     SCI_GETTEXTLENGTH,
                                     0,
                                     0);
    if (range.chrg.cpMax > range.chrg.cpMin + glob_max_callback_context_range_size)
    {
	range.chrg.cpMax = range.chrg.cpMin + glob_max_callback_context_range_size;
    }

    free (range.lpstrText);
    range.lpstrText = (char*) malloc (range.chrg.cpMax - range.chrg.cpMin + 1);
    if (range.lpstrText != NULL)
    {
        ::SendMessage(sci,
                      SCI_GETTEXTRANGE,
                      0,
                      (sptr_t) &range);
    }
        
    return range.lpstrText;
}

static void init_presage (HWND sci)
{
    if (presage_new && presage == 0)
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
            ::MessageBox(NULL, TEXT("presage init error"), TEXT("Presage error"), MB_OK);
            abort();
        }
    }
    else
    {
        ::MessageBox(NULL, TEXT("presage_new function not available"), TEXT("Presage error"), MB_OK);
    }
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
                if (function_string == NULL)
                {
                    /* we must be running out of memory... cannot recover */
                    return NULL;
                }

                /* stringify 'F' (i+1) ' ' into function_string */
                nchars = sprintf (function_string,
                                  "F%d ",
                                  i + 1);
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
                        function_string = (char*) malloc (sizeof(char) * (nchars + 1));
                        if (function_string == NULL)
                        {
                            /* we must be running out of memory... cannot recover */
                            return NULL;
                        }
                    }
                    function_string_len = nchars + 1;
                    nchars = sprintf (function_string,
                                      "F%d ",
                                      i + 1);
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

static void on_predict ()
{
    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND scintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    if (!presage) {
        // we only get here if the static global presage instance
        // hasn't been initialized yet.
        //
        init_presage (scintilla);

        ::SendMessage(scintilla, SCI_AUTOCSETSEPARATOR, '\t', 0);        /* set autocompletion separator */
        ::SendMessage(scintilla, SCI_AUTOCSETORDER, SC_ORDER_CUSTOM, 0); /* set autocompletion order */

        char* value = 0;
        if (PRESAGE_OK == presage_config (presage, "Presage.Selector.SUGGESTIONS", &value))
        {
            uptr_t height = atoi (value);
            presage_free_string (value);
            ::SendMessage(scintilla, SCI_AUTOCSETMAXHEIGHT, height, 0);  /* set autocompletion box height */
        }

    }

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


void on_enable()
{
    HMENU hMenu = ::GetMenu (nppData._nppHandle);
    glob_presage_enabled = !glob_presage_enabled;

    if (hMenu) {
        CheckMenuItem (hMenu,
                       funcItem[CMD_ENABLED]._cmdID,
                       MF_BYCOMMAND | (glob_presage_enabled ? MF_CHECKED : MF_UNCHECKED)
            );
    }
}


void on_learn_mode()
{
    HMENU hMenu = ::GetMenu (nppData._nppHandle);
    glob_learn_mode = !glob_learn_mode;
    const char *value = 0;
    if (glob_learn_mode) {
        value = "true";
    } else {
        value = "false";
    }
    if (PRESAGE_OK != presage_config_set (
            presage, 
            "Presage.Predictors.UserSmoothedNgramPredictor.LEARN",
            value)) {
        ::MessageBox(NULL, TEXT("Error while toggling Presage learn mode."), TEXT("Presage error"), MB_OK);
    }
    if (hMenu) {
        CheckMenuItem (hMenu,
                       funcItem[CMD_LEARN_MODE]._cmdID,
                       MF_BYCOMMAND | (glob_learn_mode ? MF_CHECKED : MF_UNCHECKED)
            );
    }
}


void on_autopunctuation()
{
    HMENU hMenu = ::GetMenu (nppData._nppHandle);
    glob_autopunctuation = !glob_autopunctuation;
    if (hMenu) {
        CheckMenuItem (hMenu,
                       funcItem[CMD_AUTOPUNCTUATION]._cmdID,
                       MF_BYCOMMAND | (glob_autopunctuation ? MF_CHECKED : MF_UNCHECKED)
            );
    }
}


void on_about()
{
    ::MessageBox(NULL, 
                 TEXT("Presage Notepad++ plugin.\n")
                 TEXT("\n")
                 TEXT("Presage, the intelligent predictive text entry system.\n")
                 TEXT("\n")
                 TEXT("Built on ") TEXT(__DATE__) TEXT(" at ") TEXT(__TIME__) TEXT("\n")
                 TEXT("\n")
                 TEXT("Copyright (C) 2013 Matteo Vescovi"),
                 TEXT("Presage Notepad++ plugin"),
                 MB_OK);
}


void on_help()
{
    if (! hInstLib)
    {
        ::MessageBox(NULL, TEXT("Presage, the intelligent predictive text entry platform, is not installed on your system. Please install presage."), TEXT("Presage Notepad++ plugin"), MB_OK);
    }
    else
    {
        ::MessageBox(NULL, TEXT("Only limited help available for now.\n\n")
                           TEXT("To insert a suggestion from the prediction autocompletion menu, press <Enter>.\n")
                           TEXT("To insert a newline, press <Ctrl-Shift-Alt-Enter>.\n")
                           TEXT("To see more choices, press <Ctrl-Shift-Alt-P>.\n")
                           TEXT("To configure NppPresage, see the Plugins->NppPlugin menu."),
                           TEXT("Presage Notepad++ plugin"), MB_OK);
    }
}

static void on_user_list_selection(struct SCNotification* nt, HWND scintilla)
{
    char* selection;
    char* completion;

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


static void on_update_ui(struct SCNotification* notification, HWND scintilla)
{
    on_predict ();
}


static void on_char_added (struct SCNotification* nt, HWND scintilla)
{
    if (glob_autopunctuation) {
        /* if added char is an autopunctuation char */
        if (strchr (glob_autopunctuation_chars, nt->ch))
        {
            uptr_t curr_pos        = ::SendMessage(scintilla,
                                                SCI_GETCURRENTPOS,
                                                0,
                                                0);
            if (curr_pos > 1)
            {
                //g_print ("current pos: %d\n", curr_pos);

                uptr_t prev_pos = curr_pos - 2;
                uptr_t prev_char = ::SendMessage(scintilla,
                                                 SCI_GETCHARAT,
                                                 prev_pos,
                                                 0);

                //g_print ("prev_pos: %d\n", prev_pos);
                //g_print ("prev_char: %c\n", prev_char);

                /* if previous char is a autopunctuation whitespace char */
                if (strchr (glob_autopunctuation_whitespace, prev_char))
                {
                    //g_print ("autopunctuation activated\n");

                    char replacement[3] = { nt->ch, prev_char, 0 };

                    ::SendMessage(scintilla,
                                  SCI_SETSELECTION,
                                  prev_pos,
                                  curr_pos);

                    ::SendMessage(scintilla,
                                  SCI_REPLACESEL,
                                  0,
                                  (sptr_t) replacement);
                }
            }
        }
    }
}


static void on_key (struct SCNotification* nt, HWND scintilla)
{
    // NOTE: this never gets invoked on Windows as SCN_KEY event is never fired

    char str[2048];
    sprintf (str, "on_key: %d\n", nt->ch);
    ::MessageBox(NULL, (LPCWSTR) str, TEXT("on_key: key:"), MB_OK);

    //::MessageBox(NULL, (LPCWSTR) nt->ch, TEXT("on_key: key:"), MB_OK);

    if (glob_function_keys_mode)
    {
        uptr_t completion_active;

        completion_active = ::SendMessage (scintilla,
                                           SCI_AUTOCACTIVE,
                                           0,
                                           0);

        if (completion_active)
        {
            /* figure out which function key it is */
            int fkn;
            char fk[4];

            fkn = nt->ch - VK_F1 + 1;
            //g_print ("fkn: F%d\n", fkn);

            sprintf (fk, "F%d", fkn);
            //g_print ("fk: %s\n", fk);

            ::MessageBox(NULL, (LPCWSTR) fk, TEXT("function key"), MB_OK);

            /* select autocompletion */
            ::SendMessage (scintilla,
                           SCI_AUTOCSELECT,
                           0,
                           (sptr_t) fk);

            /* and complete it */
            ::SendMessage (scintilla,
                           SCI_AUTOCCOMPLETE,
                           0,
                           0);
        }
    }
}


void on_notification (struct SCNotification* notification)
{
    switch (notification->nmhdr.code) {
    case NPPN_SHUTDOWN:
        commandMenuCleanUp();
        glob_presage_enabled = false;
        presage_free (presage);
        presage = 0;
        break;
    case NPPN_TBMODIFICATION:
        /* if presage DLL loading failed, gray out menu items */
        if ((notification->nmhdr.hwndFrom == nppData._nppHandle) && 
            (hInstLib == NULL))
        {
            //::MessageBox(NULL, TEXT("graying out menu items..."), TEXT("Presage debugging"), MB_OK);

            HMENU hMenu = ::GetMenu (nppData._nppHandle);
                
            ::EnableMenuItem (hMenu, funcItem[CMD_ENABLED]._cmdID, MF_BYCOMMAND | MF_GRAYED);
            ::EnableMenuItem (hMenu, funcItem[CMD_LEARN_MODE]._cmdID, MF_BYCOMMAND | MF_GRAYED);
            ::EnableMenuItem (hMenu, funcItem[CMD_AUTOPUNCTUATION]._cmdID, MF_BYCOMMAND | MF_GRAYED);
            ::EnableMenuItem (hMenu, funcItem[CMD_PREDICT]._cmdID, MF_BYCOMMAND | MF_GRAYED);
        }
        break;
    }

    // handle scintilla notifications only if presage is activated
    if (glob_presage_enabled)
    {
        // Get the current scintilla
        int which = -1;
        ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
        if (which == -1)
            return;
        HWND scintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

        switch (notification->nmhdr.code) {
        case SCN_PAINTED:
            //::MessageBox(NULL, TEXT("SCN_PAINTED"), TEXT("notification"), MB_OK);
            break;
        case SCN_UPDATEUI:
            on_update_ui (notification, scintilla);
            break;
        case SCN_MODIFIED:
            //::MessageBox(NULL, TEXT(notification->text), TEXT("SCNs_MODIFIED"), MB_OK);
            //::MessageBox(NULL, TEXT("SCN_MODIFIED"), TEXT("notification"), MB_OK);
            break;
        case SCN_CHARADDED:
            on_char_added (notification, scintilla);
            break;
        case SCN_USERLISTSELECTION:
            on_user_list_selection (notification, scintilla);
            break;
        case SCN_AUTOCCANCELLED:
            //::MessageBox(NULL, TEXT("SCN_AUTOCCANCELLED"), TEXT("notification"), MB_OK);
            break;
        case SCN_KEY:
            // NOTE: this never gets invoked on Windows as SCN_KEY event is never fired
            ::MessageBox(NULL, TEXT("SCN_KEY"), TEXT("notification"), MB_OK);
            on_key(notification, scintilla);
            break;
        default:
            //char str[2048];
            //sprintf(str, "notification->nmhdr.code: %u\n", notification->nmhdr.code);
            //::MessageBox(NULL, TEXT(str), TEXT("notification"), MB_OK);
            break;
        }
    }
}
