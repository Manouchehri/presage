
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


#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H

//
// All difinitions of plugin interface
//
#include "PluginInterface.h"

//-------------------------------------//
//-- STEP 1. DEFINE YOUR PLUGIN NAME --//
//-------------------------------------//
// Here define your plugin name
//
const TCHAR NPP_PLUGIN_NAME[] = TEXT("Presage");

//-----------------------------------------------//
//-- STEP 2. DEFINE YOUR PLUGIN COMMAND NUMBER --//
//-----------------------------------------------//
//
// Here define the number of your plugin commands
//
enum MENU_COMMANDS
  {
    CMD_ENABLED = 0,
    CMD_LEARN_MODE,
    CMD_AUTOPUNCTUATION,
    CMD_SEPARATOR_1,
    CMD_PREDICT,
    CMD_SEPARATOR_2,
    CMD_ABOUT,
    CMD_HELP,
    NB_MENU_COMMANDS
  };

const int nbFunc = NB_MENU_COMMANDS;


//
// Initialization of your plugin data
// It will be called while plugin loading
//
void pluginInit(HANDLE hModule);

//
// Cleaning of your plugin
// It will be called while plugin unloading
//
void pluginCleanUp();

//
//Initialization of your plugin commands
//
void commandMenuInit();

//
//Clean up your plugin commands allocation (if any)
//
void commandMenuCleanUp();

//
// Your plugin command functions
//
void on_enable();
void on_predict();
void on_learn_mode();
void on_autopunctuation();
void on_about();
void on_help();

void on_notification (struct SCNotification* notification);


#endif //PLUGINDEFINITION_H
