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


//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND sci = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

   // create presage
   presage_status = presage_new (get_past_stream,
			                     sci,
			                     get_future_stream,
			                     sci,
			                     &presage);
   if (PRESAGE_OK != presage_status)
   {
       /* should handle this better */
       abort();
   }

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
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("predict"), predict, NULL, false);
    setCommand(1, TEXT("Hello (with dialog)"), helloDlg, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void predict()
{
    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND sci = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    // Say hello now :
    // Scintilla control has no Unicode mode, so we use (char *) here
    //::SendMessage(sci, SCI_SETTEXT, 0, (LPARAM)"Hello, Notepad++!");

	const char *str = get_past_stream (sci);

	size_t wcstrsize = strlen(str) + 1;

    wchar_t* wcstr = (wchar_t*) malloc (wcstrsize * sizeof(wchar_t));

    // Convert char* string to a wchar_t* string.
    size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstr, wcstrsize, str, _TRUNCATE);

	::MessageBox(NULL, wcstr, TEXT("Presage Notepad++ past stream"), MB_OK);

	char** prediction = 0;
	presage_predict (presage, &prediction);
	for (int i = 0; prediction[i] != 0; i++) {
		::MessageBoxA(NULL, (LPCSTR) prediction[i], "Prediction", MB_OK);
	}
	presage_free_string_array (prediction);

	free (wcstr);
}

void helloDlg()
{
    ::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
}
