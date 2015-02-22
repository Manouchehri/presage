
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
  OutFile "${PRESAGE_NAME}-${PRESAGE_VERSION}-${BITNESS}bit-setup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES${BITNESS}\${PRESAGE_NAME}"
  
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

  ; bin/
  SetOutPath "$INSTDIR\bin"

  File "bin\text2ngram.exe"
  File "bin\presage_demo_text.exe"
  File "bin\presage_simulator.exe"
;  File "bin\presage_dbus_python_demo"
;  File "bin\presage_dbus_service"
  File "bin\gprompter.exe"
  File "bin\qprompter.exe"
  File "bin\libpresage-1.dll"

  ; MinGW deps
  File "bin\libstdc++-6.dll"
!if "${BITNESS}" == "64"
  File "bin\libgcc_s_seh-1.dll"
!else
  File "bin\libgcc_s_dw2-1.dll"
!endif

  ; SQLite deps
  File "bin\libsqlite3-0.dll"

  ; QT deps
  File "bin\qscintilla2.dll"
  File "bin\QtCore4.dll"
  File "bin\QtGui4.dll"
  File "bin\libwinpthread-1.dll"
  File "bin\libpng16-16.dll"

  ; GTK deps
  File "bin\libatk-1.0-0.dll"
  File "bin\libcairo-2.dll"
  File "bin\libexpat-1.dll"
  File "bin\libfontconfig-1.dll"
  File "bin\libgdk-win32-2.0-0.dll"
  File "bin\libgdk_pixbuf-2.0-0.dll"
  File "bin\libgio-2.0-0.dll"
  File "bin\libglib-2.0-0.dll"
  File "bin\libgmodule-2.0-0.dll"
  File "bin\libgobject-2.0-0.dll"
  File "bin\libgthread-2.0-0.dll"
  File "bin\libgtk-win32-2.0-0.dll"
  File "bin\libpango-1.0-0.dll"
  File "bin\libpangocairo-1.0-0.dll"
  File "bin\libpangoft2-1.0-0.dll"
  File "bin\libpangowin32-1.0-0.dll"
  File "bin\libpng14-14.dll"
  File "bin\zlib1.dll"
!if "${BITNESS}" == "64"
  File "bin\libfreetype-6.dll"
  File "bin\libintl-8.dll"
!else
  File "bin\freetype6.dll"
  File "bin\intl.dll"
!endif

  ; etc/
  SetOutPath "$INSTDIR\etc"
  File "etc\presage.xml"

  ; share/
;  SetOutPath "$INSTDIR\share\dbus-1\services"
;  File "share\dbus-1\services\org.gnome.presage.service"

  SetOutPath "$INSTDIR\share\pixmaps"
  File "share\pixmaps\gprompter.png"
  File "share\pixmaps\gprompter.xpm"
  File "share\pixmaps\pyprompter.png"
  File "share\pixmaps\pyprompter.xpm"

  SetOutPath "$INSTDIR\share\presage"
  File "share\presage\abbreviations_en.txt"
  File "share\presage\abbreviations_it.txt"
  File "share\presage\database_en.db"
  File "share\presage\database_es.db"
  File "share\presage\database_it.db"
  File "share\presage\getting_started.txt"
  File "share\presage\presage.png"
  File "share\presage\presage.svg"
  File "share\presage\presage.xpm"
  File "share\presage\python_binding.txt"
  
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
  !insertmacro ReplaceInFile "$INSTDIR\etc\presage.xml" "${LOCAL_INSTALL_WIN_DIR}" "$INSTDIR"

SectionEnd

;----
;Devel section
Section "Development files" SecDevel

  ; include/
  SetOutPath "$INSTDIR\include"
  File "include\presage.h"
  File "include\presageCallback.h"
  File "include\presageException.h"

  ; lib/
  SetOutPath "$INSTDIR\lib"
  File "lib\libpresage.a"
  File "lib\libpresage.dll.a"
  File "lib\libpresage.la"


SectionEnd

;----
;Python section
Section "Python binding" SecPython

  ; Scripts
  SetOutPath "$INSTDIR\Scripts"
  File "Scripts\presage_python_demo"
  File "Scripts\pyprompter"

  ; lib/site-packages
  SetOutPath "$INSTDIR\lib"
  File /r lib\site-packages


SectionEnd

;----
;.NET section
Section ".NET binding" SecNET

  ; bin/
  SetOutPath "$INSTDIR\bin"
  File "bin\presage.net.dll"
  File "bin\presage_csharp_demo.exe"

SectionEnd

;----
;WCF .NET service
Section "WCF .NET service" SecWCF

  ; bin/
  SetOutPath "$INSTDIR\bin"
  File "bin\presage_wcf_service.dll"
  File "bin\presage_wcf_service_console_host.exe"

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
  File "bin\NppPresage.dll"
  Goto npp_done

npp_not_found:
  MessageBox MB_OK 'Notepad++ installation not found.$\r$\nNotepad++ presage intelligent predictive text entry plugin will not be automatically installed in Notepad++ plugins directory.$\r$\nTo install manually, please copy $INSTDIR\bin\NppPresage.dll into <Notepad++ install directory>\plugins\'
  SetOutPath "$INSTDIR\bin"
  File "bin\NppPresage.dll"
  Goto npp_done

npp_done:

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecRuntime ${LANG_ENGLISH} "Install presage core C++ native library runtime and required configuration files"
  LangString DESC_SecDevel ${LANG_ENGLISH} "Install presage development files (headers and libraries)"
  LangString DESC_SecPython ${LANG_ENGLISH} "Install presage Python binding and examples Python scripts"
  LangString DESC_SecNET ${LANG_ENGLISH} "Install presage .NET binding and C# demo program"
  LangString DESC_SecWCF ${LANG_ENGLISH} "Install presage WCF .NET service"
  LangString DESC_SecNpp ${LANG_ENGLISH} "Install Notepad++ presage intelligent predictive text entry plugin"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecRuntime} $(DESC_SecRuntime)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDevel} $(DESC_SecDevel)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecPython} $(DESC_SecPython)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecNET} $(DESC_SecNET)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecWCF} $(DESC_SecWCF)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecNpp} $(DESC_SecNpp)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ; bin/
  Delete "$INSTDIR\bin\text2ngram.exe"
  Delete "$INSTDIR\bin\presage_demo_text.exe"
  Delete "$INSTDIR\bin\presage_simulator.exe"
;  Delete "$INSTDIR\bin\presage_dbus_python_demo"
;  Delete "$INSTDIR\bin\presage_dbus_service"
  Delete "$INSTDIR\bin\gprompter.exe"
  Delete "$INSTDIR\bin\qprompter.exe"
  Delete "$INSTDIR\bin\libpresage-1.dll"

  ; MinGW deps
  Delete "$INSTDIR\bin\libstdc++-6.dll"
!if "${BITNESS}" == "64"
  Delete "$INSTDIR\bin\libgcc_s_seh-1.dll"
!else
  Delete "$INSTDIR\bin\libgcc_s_dw2-1.dll"
!endif

  ; SQLite deps
  Delete "$INSTDIR\bin\libsqlite3-0.dll"

  ; QT deps
  Delete "$INSTDIR\bin\qscintilla2.dll"
  Delete "$INSTDIR\bin\QtCore4.dll"
  Delete "$INSTDIR\bin\QtGui4.dll"
  Delete "$INSTDIR\bin\libwinpthread-1.dll"
  Delete "$INSTDIR\bin\libpng16-16.dll"

  ; GTK deps
!if "${BITNESS}" == "64"
  Delete "$INSTDIR\bin\libfreetype-6.dll"
  Delete "$INSTDIR\bin\libintl-8.dll"
!else
  Delete "$INSTDIR\bin\freetype6.dll"
  Delete "$INSTDIR\bin\intl.dll"
!endif
  Delete "$INSTDIR\bin\libatk-1.0-0.dll"
  Delete "$INSTDIR\bin\libcairo-2.dll"
  Delete "$INSTDIR\bin\libexpat-1.dll"
  Delete "$INSTDIR\bin\libfontconfig-1.dll"
  Delete "$INSTDIR\bin\libgdk-win32-2.0-0.dll"
  Delete "$INSTDIR\bin\libgdk_pixbuf-2.0-0.dll"
  Delete "$INSTDIR\bin\libgio-2.0-0.dll"
  Delete "$INSTDIR\bin\libglib-2.0-0.dll"
  Delete "$INSTDIR\bin\libgmodule-2.0-0.dll"
  Delete "$INSTDIR\bin\libgobject-2.0-0.dll"
  Delete "$INSTDIR\bin\libgthread-2.0-0.dll"
  Delete "$INSTDIR\bin\libgtk-win32-2.0-0.dll"
  Delete "$INSTDIR\bin\libpango-1.0-0.dll"
  Delete "$INSTDIR\bin\libpangocairo-1.0-0.dll"
  Delete "$INSTDIR\bin\libpangoft2-1.0-0.dll"
  Delete "$INSTDIR\bin\libpangowin32-1.0-0.dll"
  Delete "$INSTDIR\bin\libpng14-14.dll"
  Delete "$INSTDIR\bin\zlib1.dll"

  ; .NET binding deps
  Delete "$INSTDIR\bin\presage.net.dll"
  Delete "$INSTDIR\bin\presage_csharp_demo.exe"

  ; WCF .NET service deps
  File "bin\presage_wcf_service.dll"
  File "bin\presage_wcf_service_console_host.exe"

  ; Notepad++ plugin
  ReadRegStr $0 HKLM "Software\Notepad++" ""
  StrCmp $0 "" npp_not_found_uninstall npp_found_uninstall

npp_found_uninstall:
  IfFileExists "$0\plugins\NppPresage.dll" 0 +2
    Delete "$0\plugins\NppPresage.dll"
npp_not_found_uninstall:
  IfFileExists "$INSTDIR\bin\NppPresage.dll" 0 +2
    Delete "$INSTDIR\bin\NppPresage.dll"

  RMDir "$INSTDIR\bin"

  ; etc/
  Delete "$INSTDIR\etc\presage.xml"
  RMDir "$INSTDIR\etc"

  ; include/
  Delete "$INSTDIR\include\presage.h"
  Delete "$INSTDIR\include\presageCallback.h"
  Delete "$INSTDIR\include\presageException.h"
  RMDir "$INSTDIR\include"

  ; lib/
  Delete "$INSTDIR\lib\libpresage.a"
  Delete "$INSTDIR\lib\libpresage.dll.a"
  Delete "$INSTDIR\lib\libpresage.la"
  RMDir /r "$INSTDIR\lib\site-packages"
  RMDir "$INSTDIR\lib"

  ; Scripts/
  Delete "$INSTDIR\Scripts\presage_python_demo"
  Delete "$INSTDIR\Scripts\pyprompter"
  RMDir "$INSTDIR\Scripts"

  ; share/
;  Delete "$INSTDIR\share\dbus-1\services\org.gnome.presage.service"
;  RMDir "$INSTDIR\share\dbus-1\services"
;  RMDir "$INSTDIR\share\dbus-1"

  Delete "$INSTDIR\share\pixmaps\gprompter.png"
  Delete "$INSTDIR\share\pixmaps\gprompter.xpm"
  Delete "$INSTDIR\share\pixmaps\pyprompter.png"
  Delete "$INSTDIR\share\pixmaps\pyprompter.xpm"
  RMDir "$INSTDIR\share\pixmaps"

  Delete "$INSTDIR\share\presage\abbreviations_en.txt"
  Delete "$INSTDIR\share\presage\abbreviations_it.txt"
  Delete "$INSTDIR\share\presage\database_en.db"
  Delete "$INSTDIR\share\presage\database_es.db"
  Delete "$INSTDIR\share\presage\database_it.db"
  Delete "$INSTDIR\share\presage\getting_started.txt"
  Delete "$INSTDIR\share\presage\presage.png"
  Delete "$INSTDIR\share\presage\presage.svg"
  Delete "$INSTDIR\share\presage\presage.xpm"
  Delete "$INSTDIR\share\presage\python_binding.txt"
  RMDir "$INSTDIR\share\presage"

  RMDir "$INSTDIR\share"

  ; uninstall
  Delete "$INSTDIR\Uninstall.exe"
  
  ; instdir
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

  ReadRegStr $R0 HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "UninstallString"
  StrCmp $R0 "" onInitDone
  ReadRegStr $R1 HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\presage" "InstallLocation"
 
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION "presage is already installed. $\n$\nClick `OK` to uninstall the previous version or `Cancel` to cancel this upgrade." IDOK onInitUninstallPrevious
  Abort
 
onInitUninstallPrevious:
  ClearErrors

  ;Run the uninstaller
  ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
 
  IfErrors onInitDone 0
    Delete $R0
    StrCmp $R1 "" onInitDone
      RMDir $R1
 
onInitDone:

FunctionEnd
