
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2011  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

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


#include "PresageUplink.h"

#include <tchar.h>
#include <Windows.h>
#include <Shlwapi.h>


HINSTANCE hInstLib = NULL; 

PFUNC_presage_completion presage_completion = NULL;
PFUNC_presage_config presage_config = NULL;
PFUNC_presage_config_set presage_config_set = NULL;
PFUNC_presage_context presage_context = NULL;
PFUNC_presage_context_change presage_context_change = NULL;
PFUNC_presage_free presage_free = NULL;
PFUNC_presage_free_string presage_free_string = NULL;
PFUNC_presage_free_string_array presage_free_string_array = NULL;
PFUNC_presage_new presage_new = NULL;
PFUNC_presage_new_with_config presage_new_with_config = NULL;
PFUNC_presage_predict presage_predict = NULL;
PFUNC_presage_prefix presage_prefix = NULL;
PFUNC_presage_save_config presage_save_config = NULL;


static bool read_presage_registry_key(TCHAR path[])
{
    bool    bRet = false;
    HKEY    hKey = NULL;
    DWORD   size = MAX_PATH;

    path[0] = '\0';

    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Presage"), 0, KEY_READ, &hKey))
    {
	if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, _T(""), NULL ,NULL, (LPBYTE)path, &size))
	{
	    PathAppend(path, _T("\\bin"));
	    //::MessageBox(NULL, path, _T("Presage bin path"), MB_OK);
	    bRet = true;
	}
	::RegCloseKey(hKey);
    }

    return bRet;
}

bool LoadPresageDLL(void)
{
    bool    bRet = FALSE;
    DWORD   size = MAX_PATH;
    TCHAR   pszPath[MAX_PATH];
    TCHAR   cwdPath[MAX_PATH];

    pszPath[0] = '\0';
    cwdPath[0] = '\0';

    if (! read_presage_registry_key(pszPath))
    {
	::MessageBox(NULL, _T("Error: cannot locate presage installation. Please reinstall presage."), _T("Presage not found"), MB_OK);
	return bRet;

	/* module path of notepad */
	/*
	  GetModuleFileName((HMODULE)hModule, pszPath, sizeof(pszPath));
	  PathRemoveFileSpec(pszPath);
	  PathRemoveFileSpec(pszPath);
	  PathAppend(pszPath, _T("\\libpresage-1.dll"));
	*/
    }
	
    /* change current directory to presage\bin directory to allow loading
     * presage DLL and its dependencies, then revert to previous current
     * directory
     */
    size = GetCurrentDirectory(MAX_PATH, cwdPath);
    //::MessageBox(NULL, cwdPath, _T("Notepad++ current directory"), MB_OK);
    SetCurrentDirectory(pszPath);
    //hInstLib = LoadLibrary(PRESAGE_DLL_FILENAME);
    hInstLib = LoadLibrary(_T("libpresage-1.dll"));
    if (size != 0 || size < MAX_PATH)
    {
	SetCurrentDirectory(cwdPath);
    }

    if (hInstLib != NULL)
    {
	if ((presage_completion = (PFUNC_presage_completion) GetProcAddress (hInstLib, "presage_completion"))
	    && (presage_config = (PFUNC_presage_config) GetProcAddress (hInstLib, "presage_config"))
	    && (presage_config_set = (PFUNC_presage_config_set) GetProcAddress (hInstLib, "presage_config_set"))
	    && (presage_context = (PFUNC_presage_context) GetProcAddress (hInstLib, "presage_context"))
	    && (presage_context_change = (PFUNC_presage_context_change) GetProcAddress (hInstLib, "presage_context_change"))
	    && (presage_free = (PFUNC_presage_free) GetProcAddress (hInstLib, "presage_free"))
	    && (presage_free_string = (PFUNC_presage_free_string) GetProcAddress (hInstLib, "presage_free_string"))
	    && (presage_free_string_array = (PFUNC_presage_free_string_array) GetProcAddress (hInstLib, "presage_free_string_array"))
	    && (presage_new = (PFUNC_presage_new) GetProcAddress (hInstLib, "presage_new"))
	    && (presage_new_with_config = (PFUNC_presage_new_with_config) GetProcAddress (hInstLib, "presage_new_with_config"))
	    && (presage_predict = (PFUNC_presage_predict) GetProcAddress (hInstLib, "presage_predict"))
	    && (presage_prefix = (PFUNC_presage_prefix) GetProcAddress (hInstLib, "presage_prefix"))
	    && (presage_save_config = (PFUNC_presage_save_config) GetProcAddress (hInstLib, "presage_save_config")))
	{
	    bRet = TRUE;
	}
    }

    return bRet;
}

void UnloadPresageDLL(void)
{
    presage_completion = NULL;
    presage_config = NULL;
    presage_config_set = NULL;
    presage_context = NULL;
    presage_context_change = NULL;
    presage_free = NULL;
    presage_free_string = NULL;
    presage_free_string_array = NULL;
    presage_new = NULL;
    presage_new_with_config = NULL;
    presage_predict = NULL;
    presage_prefix = NULL;
    presage_save_config = NULL;

    if (hInstLib != NULL)
        if (FreeLibrary(hInstLib))
	    hInstLib = NULL;
}
