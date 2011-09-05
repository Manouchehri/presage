
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
#include <windows.h>
#include <Shlwapi.h>


HINSTANCE   hInstLib = NULL; 


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


bool LoadPresageDLL(void)
{
    bool    bRet = FALSE;
    HKEY    hKey = NULL;
    DWORD   size = MAX_PATH;
    TCHAR   pszPath[MAX_PATH];
	HMODULE hModule = NULL;

    pszPath[0] = '\0';

    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Presage"), 0, KEY_READ, &hKey))
    {
		if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, _T(""), NULL ,NULL, (LPBYTE)pszPath, &size))
		{
			_tcscat(pszPath, _T("\\bin\\libpresage-1.dll"));
			::MessageBox(NULL, pszPath, TEXT("Presage path to DLL"), MB_OK);
		}
		::RegCloseKey(hKey);
	}
	else
	{
        /* module path of notepad */
		GetModuleFileName((HMODULE)hModule, pszPath, sizeof(pszPath));
		PathRemoveFileSpec(pszPath);
		PathRemoveFileSpec(pszPath);
		PathAppend(pszPath, _T("\\libpresage-1.dll"));
	}

	hInstLib = LoadLibrary(pszPath);

	if (hInstLib != NULL)
	{
		presage_completion = (PFUNC_presage_completion) GetProcAddress (hInstLib, "presage_completion");
		presage_config = (PFUNC_presage_config) GetProcAddress (hInstLib, "presage_config");
		presage_config_set = (PFUNC_presage_config_set) GetProcAddress (hInstLib, "presage_config_set");
		presage_context = (PFUNC_presage_context) GetProcAddress (hInstLib, "presage_context");
		presage_context_change = (PFUNC_presage_context_change) GetProcAddress (hInstLib, "presage_context_change");
		presage_free = (PFUNC_presage_free) GetProcAddress (hInstLib, "presage_free");
		presage_free_string = (PFUNC_presage_free_string) GetProcAddress (hInstLib, "presage_free_string");
		presage_free_string_array = (PFUNC_presage_free_string_array) GetProcAddress (hInstLib, "presage_free_string_array");
		presage_new = (PFUNC_presage_new) GetProcAddress (hInstLib, "presage_new");
		presage_new_with_config = (PFUNC_presage_new_with_config) GetProcAddress (hInstLib, "presage_new_with_config");
		presage_predict = (PFUNC_presage_predict) GetProcAddress (hInstLib, "presage_predict");
		presage_prefix = (PFUNC_presage_prefix) GetProcAddress (hInstLib, "presage_prefix");
		presage_save_config = (PFUNC_presage_save_config) GetProcAddress (hInstLib, "presage_save_config");

		bRet = TRUE;
	}

    return bRet;
}

void UnloadPresageDLL(void)
{
    if (hInstLib != NULL)
        FreeLibrary(hInstLib);
}
