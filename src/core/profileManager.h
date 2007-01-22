
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>            *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.*
 *                                                                           *
\*****************************************************************************/        

#ifndef SOOTH_PROFILEMANAGER
#define SOOTH_PROFILEMANAGER

#include "core/historyTracker.h"
#include "core/predictor.h"
#include "core/selector.h"
//PLUMP #include "core/pluginManager.h"
#include "plugins/plugin.h"

#include "tinyxml/tinyxml.h"

#include <string>
#include <sstream> // for std::ostringstream
#include <stdio.h> // for int remove( const char* ) to remove files/dirs

const char DEFAULT_SYSTEM_PASSWD_FILE[]  = "/etc/defaultpasswd.xml";
const char DEFAULT_SYSTEM_PROFILE_FILE[] = "/etc/soothsayer.xml";
const char DEFAULT_USER_PASSWD_FILE[]    = "./etc/defaultpasswd.xml";
const char DEFAULT_USER_PROFILE_FILE[]   = "./etc/soothsayer.xml";

/** Juggles configuration files and soothsayer system initialization.
 *
 * The idea is that ProfileManager loads up an xml file containing configuration data and initializes system components according to the config file.
 * 
 * Configuration files:
 * /etc/soothsayer/config.xml
 * ~/.soothsayer/config.xml
 *
 *
 */
class ProfileManager {
 public:
    //PLUMP	ProfileManager(HistoryTracker&, Predictor&, Selector&, PluginManager&);
	ProfileManager(HistoryTracker&, Predictor&, Selector&);
	~ProfileManager();

	void interface(); /**< Display user interface on screen, get user
			     action, act accordingly by invoking appropriate
			     method to execute user command. */

	void loadPasswd();
	bool savePasswd();

	void initSoothsayer();
	void initHistoryTracker();
	void initPredictor();
	void initSelector();
    //PLUMP	void initPluginManager();

	bool authenticateUser( const std::string, const std::string );
	TiXmlNode* createUser( const std::string, const std::string );
	bool removeUser( const std::string, const std::string );
	TiXmlNode* findUser( const std::string ) const;

	//void selectProfile();
	void loadProfile();
	bool buildProfile( const std::string );
	bool createProfile( const std::string );
	bool saveProfile() const;
	void removeProfile( const std::string, const std::string );
	TiXmlNode* findProfile( TiXmlNode*, const std::string ) const;

 private:
	HistoryTracker& historyTracker;
	Predictor&      predictor;
	Selector&       selector;
    //PLUMP	PluginManager&  pluginManager;

	TiXmlDocument*  passwdDoc;
	TiXmlDocument*  profileDoc;
	
	std::string username; // active username
	std::string profile;  // active profile

	TiXmlNode* userNodePtr;
	TiXmlNode* userProfilePtr;

	std::string passwdFile;
        std::string profileFile;

	void printUsers() const;
	void printPasswd() const;
	void printProfiles() const;

	void loginInterface();
	void profileInterface();
	void pluginInterface();

	void userI();
	bool profileI(); // returns true if profile has been selected and 
	                 // Soothsayer should start
	void pluginI();
	void doPlugin();

};


#endif // SOOTH_PROFILEMANAGER
