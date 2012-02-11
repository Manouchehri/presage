
;;;;;;;;;;
;  Presage, an extensible predictive text entry system
;  ---------------------------------------------------
;
;  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
;
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License along
;  with this program; if not, write to the Free Software Foundation, Inc.,
;  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

; Presage NSIS Modern User Interface Installer for Windows 

; (Based on NSIS Start Menu Folder Selection Example Script written by
; Joost Verburg)

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Include string replace function

  !include StrRep.nsh
  !include ReplaceInFile.nsh


;--------------------------------
;Global Variables

  Var StartMenuFolder

;--------------------------------
;Defines

!include defines.nsh

;--------------------------------
;General

  ;Name and file
  Name "${PRESAGE_NAME}-${PRESAGE_VERSION}"
  OutFile "${PRESAGE_NAME}-${PRESAGE_VERSION}-setup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\${PRESAGE_NAME}"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Presage" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "license.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Presage" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
  !insertmacro MUI_PAGE_INSTFILES

  !define MUI_FINISHPAGE_RUN $INSTDIR/bin/qprompter
  !define MUI_FINISHPAGE_RUN_TEXT "Run qprompter"
  !define MUI_FINISHPAGE_RUN_NOTCHECKED
  !define MUI_FINISHPAGE_SHOWREADME $INSTDIR/share/presage/getting_started.txt
  !define MUI_FINISHPAGE_SHOWREADME_TEXT "Show getting started text file"
  !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
  !define MUI_FINISHPAGE_LINK "For the latest news and updates, visit the website"
  !define MUI_FINISHPAGE_LINK_LOCATION http://presage.sourceforge.net/
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

;----
;Runtime section
Section "-Runtime" SecRuntime

  SetOutPath "$INSTDIR"

  File /r bin
  File /r etc
  File /r share
  
  ;Store installation folder
  WriteRegStr HKCU "Software\Presage" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ;Add uninstall information to Add/Remove Programs
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "DisplayName" "${PRESAGE_NAME}"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "InstallLocation" "$INSTDIR"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "Publisher" "http://presage.sourceforge.net"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "DisplayVersion" "${PRESAGE_VERSION}"
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "NoModify" "1"
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "NoRepair" "1"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\qprompter.lnk" "$INSTDIR\bin\qprompter.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\gprompter.lnk" "$INSTDIR\bin\gprompter.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END


 ;!insertmacro ReplaceInFile SOURCE_FILE SEARCH_TEXT REPLACEMENT
  !insertmacro ReplaceInFile "$INSTDIR\etc\presage.xml" "c:\presage" "$INSTDIR"

SectionEnd

;----
;Devel section
Section "Development files" SecDevel

  SetOutPath "$INSTDIR"
  File /r include
  SetOutPath "$INSTDIR\lib"
  File /r lib\libpresage.*

SectionEnd

;----
;Python section
Section "Python binding" SecPython

  SetOutPath "$INSTDIR"
  File /r Scripts
  SetOutPath "$INSTDIR\lib"
  File /r lib\site-packages

SectionEnd

;----
;Notepad++ plugin section
Section "Notepad++ plugin" SecNpp

  ReadRegStr $0 HKLM "Software\Notepad++" ""
  StrCmp $0 "" npp_not_found npp_found

npp_found:
  ;MessageBox MB_OK 'Notepad++ installation directory is "$0"'
  IfFileExists "$0\plugins\*.*" 0 npp_not_found
  SetOutPath "$0\plugins"
  File NppPresage.dll
  Goto npp_done

npp_not_found:
  MessageBox MB_OK 'Notepad++ installation not found.$\r$\nNotepad++ presage intelligent predictive text entry plugin will not be automatically installed in Notepad++ plugins directory.$\r$\nTo install manually, please copy $INSTDIR\bin\NppPresage.dll into <Notepad++ install directory>\plugins\'
  SetOutPath "$INSTDIR\bin"
  File NppPresage.dll
  Goto npp_done

npp_done:

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecRuntime ${LANG_ENGLISH} "Install presage core C++ native library runtime and required configuration files"
  LangString DESC_SecDevel ${LANG_ENGLISH} "Install presage development files (headers and libraries)"
  LangString DESC_SecPython ${LANG_ENGLISH} "Install presage Python binding and examples Python scripts"
  LangString DESC_SecNpp ${LANG_ENGLISH} "Install Notepad++ presage intelligent predictive text entry plugin"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecRuntime} $(DESC_SecRuntime)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDevel} $(DESC_SecDevel)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecPython} $(DESC_SecPython)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecNpp} $(DESC_SecNpp)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  RMDir /r "$INSTDIR\bin"
  RMDir /r "$INSTDIR\etc"
  RMDir /r "$INSTDIR\include"
  RMDir /r "$INSTDIR\lib"
  RMDir /r "$INSTDIR\Scripts"
  RMDir /r "$INSTDIR\share"
  RMDir /r "$INSTDIR\var"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\qprompter.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\gprompter.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  DeleteRegValue HKCU "Software\Presage" "Start Menu Folder"
  DeleteRegKey /ifempty HKCU "Software\Presage"

  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "DisplayName"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "UninstallString"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "InstallLocation"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "Publisher"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "DisplayVersion"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "NoModify"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "NoRepair"

  DeleteRegKey /ifempty HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage"

SectionEnd


Function .onInit
FunctionEnd
