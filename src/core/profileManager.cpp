
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


#include "core/profileManager.h"
#include "core/utility.h" // isYes isNo isTrue isFalse utility function


/** 
 *
 */
ProfileManager::ProfileManager( HistoryTracker& ht,
				Predictor& p,
				Selector& s //PLUMP,
                                //PLUMP				PluginManager& pm )
    )
    : historyTracker( ht ),
      predictor( p ),
      selector( s ) //PLUMP,
    //PLUMP	  pluginManager( pm )
{
    passwdDoc = NULL;
    profileDoc = NULL;


    // REVISIT: any reason passwd.xml should not be disposed of?
    loadPasswd();
	
    // FIXME: TODO: should load profile specified in passwd file,
    // not unconditionally load default profile file
    loadProfile();

    initHistoryTracker();
    initPredictor();
}


/** 
 *
 */
ProfileManager::~ProfileManager()
{}


/** User interface ??????
 *
 */
void ProfileManager::interface()
{
    //loginInterface();
    //profileInterface();
    
    userI();
}		

/**
 *
 */
void ProfileManager::loginInterface()
{
    std::string usrname;
    std::string passwd;
    int choice;
	
    do {

        printPasswd();

        std::cout << std::endl
                  << "Available users" << std::endl
                  << "---------------" << std::endl;
	
        printUsers();	

        std::cout << std::endl
                  << "0 - quit" << std::endl
                  << "1 - create user" << std::endl
                  << "2 - remove user" << std::endl
                  << "3 - authenticate user" << std::endl
                  << "Watcha wanna do: ";
		  
        std::cin >> choice;

        switch( choice ) {
	case 0:
            std::cout << "Exiting..." << std::endl;
            exit(0);
            break;
	case 1:
            std::cout << "Enter username: ";
            std::cin >> usrname;
            std::cout << "Enter password: ";		
            std::cin >> passwd;
		
            createUser( usrname, passwd );
            break;
	case 2:
            std::cout << "Enter username: ";
            std::cin >> usrname;
            std::cout << "Enter password: ";		
            std::cin >> passwd;

            removeUser( usrname, passwd );
            break;
	case 3:
            std::cout << "Enter username: ";
            std::cin >> usrname;
            std::cout << "Enter password: ";		
            std::cin >> passwd;

            if( authenticateUser( usrname, passwd ) )
                return;
            else
                std::cout << "Authentication failed!" 
                          << std::endl;
            break;
	default:
            std::cerr << "Wrong choice.\a Please try again."
                      << std::endl;
            break;
        }

    } while( 0 != choice );
}

/**
 *
 */
void ProfileManager::profileInterface()
{
    std::string profile;
    int choice;

    do {
        std::cout << "User: " << username << std::endl;
        std::cout << "Profiles:" << std::endl
                  << "---------" << std::endl;
        printProfiles();
	
        std::cout << std::endl
                  << "1 - go back" << std::endl
                  << "2 - create profile" << std::endl
                  << "3 - remove profile" << std::endl
                  << "4 - edit profile" << std::endl
                  << "5 - save profile" << std::endl
                  << "6 - soothsayer" << std::endl
                  << "Enter choice: ";
        std::cin >> choice;

        switch( choice ) {
	case 1:
		
            break;
	case 2:
            std::cout << "Select profile: ";
            std::cin >> profile;
            createProfile( profile );
            break;
	case 3:
            std::cout << "Select profile: ";
            std::cin >> profile;
            removeProfile( username, profile );
            break;
	case 4:
            std::cout << "Select profile: ";
            std::cin >> profile;
            pluginInterface();
            break;
	case 5:
            std::cout << "Select profile: ";
            std::cin >> profile;
            std::cout << "Not implemented\a" << std::endl;
            break;
	case 6:
            std::cout << "Registering active plugins..." 
                      << std::endl;
            //PLUMP			pluginManager.registerPlugins();
            std::cout << "Firing Soothsayer up..."
                      << std::endl;
            return;
            break;
	default:
            std::cerr << "Wrong choice.\a Please try again."
                      << std::endl;
            break;
        }
    } while ( 1 != choice );
}


/** Allows user to configure plugins' options.
 *
 */
void ProfileManager::pluginInterface()
{}


/** Authenticate user.
 *
 * User authentication requires user to enter a username and password.
 *
 */
bool ProfileManager::authenticateUser(
    const std::string usr,
    const std::string pwd
    )
{
    TiXmlNode* user = findUser( usr );
    // if we found our user, let us authenticate his/her password
    if( user ) {
        if( user->FirstChild( "password" ) ) {
            if( user->FirstChild( "password" )->FirstChild() ) {

                // if our user knows his/her password,
                // let us celebrate
                if( pwd == user->FirstChild( "password" )->FirstChild()->Value() ) {
                    std::cout << "Logging user " 
                              << usr << " in..."
                              << std::endl;
					
                    // save current user in username
                    username = usr;
					
                    // save current usernode in userNodePtr
                    userNodePtr = user;
					
                    return true;

                }
            }
        }
    }
		
    std::cerr << "Login incorrect\a" << std::endl;
    return false;
}


/** Creates a new user.
 * 
 * @name username
 * @passwd password
 *
 * Checks that username is not already in use.
 * Returns a pointer to the profiles XML container for the newly created user.
 * Returns NULL if the username specified is already in use.
 *
 */
TiXmlNode* ProfileManager::createUser(
    const std::string name,
    const std::string passwd
    )
{
    // check if username has not been taken yet
    if( NULL != findUser( name ) ) {
		
        std::cerr << "User " << name << " already exists." << std::endl
                  << "Please choose another one." << std::endl;
        return NULL;
	
    } else {
	
        // get handle to document root
        TiXmlHandle doc( passwdDoc );
		
        // get to root node
        TiXmlNode* node = doc.FirstChild("passwd").Node();
        assert( node );
		
        // insert new user node and store pointer to it in node
        node = node->InsertEndChild( TiXmlElement( "user" ) );
        assert( node );
		
        TiXmlNode* element;
        TiXmlNode* text;
		
        // insert name container
        element = node->InsertEndChild( TiXmlElement( "name" ) );
        assert( element );
        // insert name text
        text = element->InsertEndChild( TiXmlText( name.c_str() ) );
        assert( text );
		
        // insert password container
        element = node->InsertEndChild( TiXmlElement( "password" ) );
        assert( element );
        // insert password text
        text = element->InsertEndChild( TiXmlText( passwd.c_str() ) );
        assert( text );
		
        // insert empty profiles container
        element = node->InsertEndChild( TiXmlElement( "profiles" ) );
        assert( element );

        // write changes to file
        bool writeOk = passwdDoc->SaveFile();
        assert( writeOk );
		
        return element;

    }
}



/** Removes user.
 *
 */
bool ProfileManager::removeUser(const std::string usr,const std::string pwd )
{
    bool removed = false;

    //	// get handle to document root
    //	TiXmlHandle doc( passwdDoc );
    //	
    //	// get to user node
    //	TiXmlNode* node = doc.FirstChild("passwd").FirstChild("user").Node();
    //	assert( node );
    //	
    //	// iterate until right user is found or no more nodes
    //	TiXmlText* text;
    //	for( ; node ; node = node->NextSibling( "user" ) ) {
    //		
    //		TiXmlHandle handle( node );
    //		text = handle.FirstChild( "name" ).FirstChild().Text();
    //		if( text ) {
    //			// if we found our user
    //			if( text->Value() == usr ) {
    //
    //				// let us check for the right password
    //				handle = node;
    //				text = handle.FirstChild( "password" )
    //					.FirstChild().Text();
    //				if( text->Value() == pwd ) {
    //
    //					// We got it! Now for some erasing...
    //					node->Clear();
    //					removed = node->Parent()
    //						->RemoveChild( node );
    //					
    //					// Now it indispensable to break out
    //					// of for loop because our node we were
    //					// iterating on does not exist anymore
    //					break;
    //				}
    //			}
    //		}
    //	}

	
    // find our user
    TiXmlNode* user = findUser( usr );
    // if user exists
    if( NULL != user ) {
		
        // get to his password
        TiXmlHandle handle( user );
        TiXmlText* text = handle.FirstChild( "password" ).FirstChild().Text();
        // check that password matches
        if( text ) {
            if( text->Value() == pwd ) {
				
                // We got it! Now for some erasing...
                // Erase user's profiles
                TiXmlNode* profile = handle.FirstChild( "profiles" ).FirstChild( "profile" ).Node();
                for( ;
                     profile ;
                     profile=profile->NextSibling("profile")) {
                    if( profile->FirstChild() ) {
                        removeProfile( usr, profile->FirstChild()->Value() );
                    }
                }
				
                // Erase user's container's children
                user->Clear();
                // Erase user container itself
                removed = user->Parent()
                    ->RemoveChild( user );
            }
        }
    }
	
    bool writeOk = passwdDoc->SaveFile();
    assert( writeOk );

    return removed;
}


/** Finds a user identified by username if it exists.
 *
 * @n username to find.
 *
 * Returns a pointer to the Xml user container if user is found, null
 *otherwise.
 *
 */
TiXmlNode* ProfileManager::findUser( const std::string usr ) const
{
    // get handle to document root
    TiXmlHandle doc( passwdDoc );
	
    // get to user node
    TiXmlNode* node = doc.FirstChild("passwd").FirstChild("user").Node();
    //assert( node ); //commented out cuz if no user exists, it bails out
    if( NULL != node ) {
        // iterate until right user is found or no more nodes
        TiXmlText* text;
        for(  ; node ; node = node->NextSibling( "user" ) ) {
			
            TiXmlHandle handle( node );
            text = handle.FirstChild( "name" ).FirstChild().Text();
            if( text ) {
                // if we found our user, return it
                if( text->Value() == usr ) {
					
                    return node;
                }
            }
        }
    }

    // if we get here, we have not found our user, so return NULL
    return NULL;
}


/** Reads in XML configuration file.
 *
 * If XML config file does not exist, a default template XML config
 * file is created.
 */
void ProfileManager::loadPasswd()
{
    passwdDoc = new TiXmlDocument;
    assert(passwdDoc);

    passwdFile = DEFAULT_USER_PASSWD_FILE;

    bool readOk = passwdDoc->LoadFile( passwdFile.c_str() );
    if(readOk) {
            
    } else {

        passwdFile = DEFAULT_SYSTEM_PASSWD_FILE;
        readOk = passwdDoc->LoadFile( passwdFile.c_str() );

        if (!readOk) {
            std::cerr << "[ProfileManager] No existing passwd file found!" << std::endl
                      << "[ProfileManager] Generating a new passwd file..." << std::endl;
		
            passwdDoc->InsertEndChild( TiXmlDeclaration( "1.0",
                                                         "UTF-8",
                                                         "no" ) );
            passwdDoc->InsertEndChild( TiXmlElement( "passwd" ) );

            passwdDoc->SaveFile();
        }
    }
}


/** Writes XML config file to disk.
 *
 * Returns true if write was successful, false otherwise.
 */
bool ProfileManager::savePasswd()
{
    bool saveOk = passwdDoc->SaveFile();

    return saveOk;
}


/** 
 *
 */
void ProfileManager::initSoothsayer()
{

}


/** Initialises HistoryTracker.
 *
 */
void ProfileManager::initHistoryTracker()
{
	
    ///////////////////////////////////////////////////////
    // Check this code: I wrote it very late at night after
    // a long abstinence from coding!!! Be wary!!!
    // TODO TODO TODO TODO
    ///////////////////////////////////////////////////////


    // Get handle to profile document root
    TiXmlHandle doc( profileDoc );

    // Get to HistoryTracker node
    TiXmlElement* module = doc.FirstChild( "Soothsayer" ).FirstChild( "HistoryTracker" ).Element();
    assert( module ); // TODO handle with exceptions
    if( NULL != module ) {
        // we found HistoryTracker option block,
        // let's handle each option

        TiXmlNode* option;
        TiXmlText* text;

        // option MAX_BUFFER_SIZE
        option = module->FirstChild( "MAX_BUFFER_SIZE" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option MAX_BUFFER_SIZE: " << text->Value() << std::endl;
                historyTracker.setMaxBufferSize( atoi( text->Value() ) );

            }
        }
    }
}


/** Initializes Predictor.
 */
void ProfileManager::initPredictor()
{
	
    // Get handle to profile document root
    TiXmlHandle doc( profileDoc );

    // Get to Predictor node
    TiXmlElement* module = doc.FirstChild( "Soothsayer" ).FirstChild( "Predictor" ).Element();
    assert( module ); // TODO handle with exceptions
    if( NULL != module ) {
        // we found Predictor option block,
        // let's handle each option

        TiXmlNode* option;
        TiXmlText* text;

        // Option PREDICT_TIME
        option = module->FirstChild( "PREDICT_TIME" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option PREDICT_TIME: " << text->Value() << std::endl;
                predictor.setPredictTime( atoi( text->Value() ) );
            }
        }
		
        // Option COMBINATION_METHOD
        option = module->FirstChild( "COMBINATION_METHOD" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option COMBINATION_METHOD: " << text->Value() << std::endl;
                predictor.setCombinationMethod( static_cast<CombinationMethod>( atoi( text->Value() ) ) );
            }
        }
    }
}


/** Initializes Selector.
 */
void ProfileManager::initSelector()
{

    // TODO: implement set methods in Selector object. Handle yes/no option value.

    // Get handle to profile document root
    TiXmlHandle doc( profileDoc );

    // Get to Selector node
    TiXmlElement* module = doc.FirstChild( "Soothsayer" ).FirstChild( "Selector" ).Element();
    assert( module ); // TODO: handle with exceptions instead of assertions
    if( NULL != module ) {
        // we found Selector option block,
        // let's handle each option

        TiXmlNode* option;
        TiXmlText* text;

        // Option SUGGESTIONS
        option = module->FirstChild( "SUGGESTIONS" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option SUGGESTIONS: " << text->Value() << std::endl;
                selector.setSuggestions( atoi( text->Value() ) );
            }
        }
		
        // Option REPEAT_SUGGESTIONS
        option = module->FirstChild( "REPEAT_SUGGESTIONS" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option REPEAT_SUGGESTIONS: " << text->Value() << std::endl;

                if( isYes( text->Value() ) || isTrue( text->Value() ) )
                    selector.setRepeatSuggestion( true );
                else if( isNo( text->Value() ) || isFalse( text->Value() ) )
                    selector.setRepeatSuggestion( false );
            }
        }

        // Option GREEDY_SUGGESTION_THRESHOLD
        option = module->FirstChild( "GREEDY_SUGGESTION_THRESHOLD" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option GREEDY_SUGGESTION_THRESHOLD: " << text->Value() << std::endl;
                selector.setGreedySuggestionThreshold( atoi( text->Value() ) );
            }
        }
    }

}


/** Initializes PluginManager and managed Plugin objects.
 */
//PLUMP void ProfileManager::initPluginManager()
//PLUMP {
//PLUMP 	
//PLUMP }


///** Allows user to select which profile to use.
// *
// * User can:
// * - select a profile
// * - load a profile
// * - edit a profile
// * - save a profile
// * - remove a profile
// * - create a new profile
// */
//void ProfileManager::selectProfile()
//{}


/** Load and parse a profile file.
 */
void ProfileManager::loadProfile()
{
    profileDoc = new TiXmlDocument;
    assert( profileDoc );

    profileFile = DEFAULT_USER_PROFILE_FILE;
    bool readOk = profileDoc->LoadFile( profileFile.c_str() );
    if(!readOk) {
        profileFile = DEFAULT_SYSTEM_PROFILE_FILE;
        readOk = profileDoc->LoadFile( profileFile.c_str() );

        if (!readOk) {
            std::cerr << "[ProfileManager] An error occured opening profile: "
                      << profileFile << std::endl;
        }
    }

    std::cout << "[ProfileManager] Using profile " << profileFile << std::endl;

    // Handle failure to load profile
    // REVISIT: for now, just fail on assertion :-(
    assert( readOk );
}


/** Write profile to disk.
 */
bool ProfileManager::saveProfile() const
{
    bool saveOk = profileDoc->SaveFile();

    return saveOk;
}


/** Create new profile with default values.
 */
bool ProfileManager::createProfile( const std::string p )
{
    // Build XML profile file and write it to disk
    bool buildOk = buildProfile( p );

    // Insert pointer to newly created profile in XML passwdDoc
    // Find current user profiles
    TiXmlNode* user = findUser( username );
    assert( NULL != user ); // make sure we found our user

    TiXmlHandle handle( user );
    TiXmlNode* profiles = user->FirstChild( "profiles" );
    assert( profiles );
    if( profiles ) {
        TiXmlNode* profile = profiles->InsertEndChild(
            TiXmlElement( "profile" ) );
        assert( profile );
        if( profile ) {
            TiXmlNode* text = profile->InsertEndChild( 
                TiXmlText( p.c_str() ) );
            assert( text );
        }
    }

    // Save passwd file
    bool saveOk = savePasswd();

    return ( saveOk && buildOk );
}


/** Create new profile for current user.
 */
bool ProfileManager::buildProfile( const std::string p )
{
    TiXmlNode* root;
    TiXmlNode* node;
    TiXmlNode* module;
    TiXmlNode* element;
	

    // Create document
    TiXmlDocument doc( p.c_str() );

    // Insert initial mandatory declaration
    node = doc.InsertEndChild( TiXmlDeclaration( "1.0", "UTF-8", "no" ) );
    assert( node );

    // Insert root element
    root = doc.InsertEndChild( TiXmlElement( "Soothsayer" ) );
    assert( root );

    // HistoryTracker module
    module = root->InsertEndChild( TiXmlElement( "HistoryTracker" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild( TiXmlElement( "MAX_BUFFER_SIZE" ) );
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_MAX_BUFFER_SIZE;
            node = element->InsertEndChild(TiXmlText( ss.str().c_str() ));
            assert( node );
        }
    }
	
    // Selector module
    module = root->InsertEndChild( TiXmlElement( "Selector" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("SUGGESTIONS"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_SUGGESTIONS;
            node = element->InsertEndChild(TiXmlText( ss.str().c_str() ));
            assert( node );
        }
		
        element = module->InsertEndChild( TiXmlElement( "REPEAT_SUGGESTIONS" ) );
        assert( element );
        if( element ) {
            node = element->InsertEndChild( TiXmlText( "no" ) );
            assert( node );
        }
		
        element = module->InsertEndChild( TiXmlElement( "GREEDY_SUGGESTION_THRESHOLD" ) );
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_GREEDY_SUGGESTION_THRESHOLD;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
    }

    // Predictor module
    module = root->InsertEndChild( TiXmlElement( "Predictor" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("PREDICT_TIME"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_PREDICT_TIME;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
		
        element = module->InsertEndChild( TiXmlElement( "COMBINATION_METHOD" ) );
        assert( element );
        if( element ) {
            std::stringstream ss;
            ss << DEFAULT_COMBINATION_METHOD;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
    }

    // ProfileManager module
    module = root->InsertEndChild( TiXmlElement( "ProfileManager" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("DUMMY_OPTION"));
        assert( element );
        if( element ) {
            node = element->InsertEndChild( TiXmlText( "very dummy" ) );
            assert( node );
        }
    }

    //PLUMP
    /*
    // PluginManager module
    module = root->InsertEndChild( TiXmlElement( "PluginManager" ) );
    assert( module );
    if( module ) {
    element = module->InsertEndChild( TiXmlElement( "PLUGIN_PATH" ) );
    assert( element );
    if( element ) {
    std::stringstream ss;
    ss << DEFAULT_PLUGIN_PATH;
    node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
    assert( node );
    }

    element = module->InsertEndChild( TiXmlElement( "PLUGIN_SUFFIX" ) );
    assert( element );
    if( element ) {
    std::stringstream ss;
    ss << DEFAULT_PLUGIN_SUFFIX;
    node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
    assert( node );
    }
    }
    */

    // Plugin modules
    module = root->InsertEndChild( TiXmlElement( "Plugins" ) );
    assert( module );
    if( module ) {

        // handled by PluginManager object
        //PLUMP		pluginManager.createProfile( module );


    }

       
    //	doc.Print();
    bool writeOk = doc.SaveFile();

    return writeOk;
}


/** Removes a profile.
 */
void ProfileManager::removeProfile( const std::string usrname,
				    const std::string profilename )
{
    TiXmlNode* user = findUser( usrname );

    if( user ) {
        TiXmlNode* profile = findProfile( user, profilename );
        if( profile != NULL ) {
			
            // delete profileDoc object if we are removing
            // active profile
            if( NULL != profileDoc ) {
                if( profileDoc->Value() == profilename ) {
                    delete profileDoc;
                }
            }
			
            // remove profile file
            int result = remove( profilename.c_str() );
            if( result != 0 )
                perror( "Could not remove profile" );
			
            // remove element in XML file
            profile->Clear();
            TiXmlNode* profiles = profile->Parent();
            bool removeOk = profiles->RemoveChild( profile );
            assert( removeOk );
			
            savePasswd();
			
        } else {
            std::cerr << "Profile does not exist\a" << std::endl;
        }
    } else {
        std::cerr << "Something dodgy happened. System tried to remove "
                  << "a profile belonging to a user that does not exist "
                  << "in the current passwd file." << std::endl 
                  << " This is a bug, bailing out" << std::endl;
        abort();
    }
}


/** Finds profile p for user u.
 *
 * @n username da trovare
 *
 * Returns a pointer to user XML container if profile found or null
 * otherwise.
 */
TiXmlNode* ProfileManager::findProfile( TiXmlNode* user, 
					const std::string profilename ) const
{
    // get handle to profiles container
    TiXmlHandle userHandle( user );
	
    // get to profile node
    TiXmlNode* profile = userHandle.FirstChild( "profiles" ).FirstChild( "profile" ).Node();
	
    // iterate until right user is found or no more nodes
    TiXmlText* text;
    for(  ; profile ; profile = profile->NextSibling( "profile" ) ) {
		
        text = profile->FirstChild()->ToText();
        if( text ) {
            // if we found our user, return it
            if( text->Value() == profilename ) {
				
                return profile;
            }
        }
    }

    // if we get here, we have not found our profile, so return NULL
    return NULL;
}


void ProfileManager::printUsers() const
{
    TiXmlHandle docHandle( passwdDoc );
    TiXmlElement* user;
	
    user = docHandle.FirstChild( "passwd" ).FirstChild( "user" ).Element();

    for(  ; user ; user = user->NextSiblingElement( "user" ) ) {

        std::cout << " - "
                  << user->FirstChild( "name" )->FirstChild()->Value()
                  << std::endl;
    }
}

void ProfileManager::printProfiles() const
{
    // Find current user XML container
    TiXmlNode* user = findUser( username );
	
    // Get to the profiles container
    TiXmlNode* profiles = user->FirstChild( "profiles" );
    assert( profiles );
    if( profiles ) {

        // Iterate profile containers
        TiXmlNode* profile = profiles->FirstChild( "profile" );
        for( ; profile ; profile = profile->NextSibling("profile") ) {
			
            // Get profile text
            TiXmlNode* text = profile->FirstChild();
			
            std::cout << text->Value() << std::endl;

        }
    }
}

void ProfileManager::printPasswd() const
{
    TiXmlNode* passwd;
    TiXmlNode* user;
    TiXmlNode* name;
    TiXmlNode* password;
    TiXmlNode* profiles;
    TiXmlNode* profile;

    TiXmlNode* node;

    passwd = passwdDoc->RootElement();
    // make sure root of XML document is passwd
    assert( passwd->Value() == static_cast<std::string>("passwd") );

    // walk users
    for( user = passwd->FirstChild( "user" ) ;
         user ;
         user = user->NextSibling( "user" ) ) {

        std::cout << std::endl << "  " << user->Value() << std::endl;

        name = user->FirstChild( "name" );
        if( name ) {
            std::cout << "    " << name->Value() << " ";
            node = name->FirstChild();
            if( node ) {
                std::cout << node->Value() << std::endl;
            }
        }
        password = user->FirstChild( "password" );
        if( password ) {
            std::cout << "    " << password->Value() << " ";
            node = password->FirstChild();
            if( node ) {
                std::cout << node->Value() << std::endl;
            }
        }
        profiles = user->FirstChild( "profiles" );
        if( profiles ) {
            std::cout << "    " << profiles->Value() << std::endl;
            for( profile = profiles->FirstChild( "profile" );
                 profile ;
                 profile = profile->NextSibling( "profile" ) ) {
				
                std::cout << "      "
                          << profile->Value() << " ";
                node = profile->FirstChild();
                if( node ) {
                    std::cout << node->Value()
                              << std::endl;
                }
            }
        }
    }
}


void ProfileManager::doPlugin()
{
    std::cout << "View, edit, save plugin options..." << std::endl
              << "Not yet implemented!" << std::endl << std::endl;
}


void ProfileManager::pluginI()
{
    int choice;
    std::string plugin;

    const int BACK = 0;
    const int EDIT = 1;

    do {
		
        std::cout << "Plugins:" << std::endl
                  << "--------" << std::endl;
		
        std::cout << BACK << " - go back" << std::endl
                  << EDIT << " - edit plugin" << std::endl
                  << "Enter choice: ";
        std::cin >> choice;
	
        switch( choice ) {
	case BACK:
            break;
	case EDIT:
            std::cout << "Select plugin: ";
            std::cin >> plugin;
            doPlugin();
            break;
	default:
            std::cerr << "Wrong choice.\a Please try again."
                      << std::endl;
            break;
        }

    } while( choice != BACK );
}


bool ProfileManager::profileI()
{
    std::string prof;
    int choice;

    const int BACK = 0;
    const int CREATE = 1;
    const int REMOVE = 2;
    const int SAVE = 3;
    const int EDIT = 4;
    const int SELECT = 5;

    do {
        std::cout << "User: " << username << std::endl;
        std::cout << "Profiles:" << std::endl
                  << "---------" << std::endl;
        printProfiles();
	
        // print available choices
        std::cout << std::endl
                  << BACK << " - go back" << std::endl
                  << CREATE << " - create profile" << std::endl
                  << REMOVE << " - remove profile" << std::endl
                  << SAVE << " - save profile" << std::endl
                  << EDIT << " - edit profile" << std::endl
                  << SELECT << " - select profile" << std::endl	
                  << "Enter choice: ";
        std::cin >> choice;

        // process choice
        switch( choice ) {
	case BACK:
            // unset username
            username.clear();
            // unset user node pointer
            userNodePtr = NULL;
            break;
	case CREATE:
            std::cout << "Enter profile name: ";
            std::cin >> prof;
            createProfile( prof );
            break;
	case REMOVE:
            std::cout << "Select profile: ";
            std::cin >> prof;
            removeProfile( username, prof );
            break;
	case SAVE:
            std::cout << "Select profile: ";
            std::cin >> prof;
            break;
	case EDIT:
            std::cout << "Select profile: ";
            std::cin >> prof;
            pluginI();
            break;
	case SELECT:
            std::cout << "Select profile: ";
            std::cin >> prof;
            if( findProfile( userNodePtr, prof ) != NULL ) {
                profile = prof;
                //PLUMP				pluginManager.registerPlugins(); 
            } else
                std::cerr << "Profile specified does not "
                          << "exist!\a" << std::endl;
            break;
	default:
            std::cerr << "Wrong choice.\a Please try again."
                      << std::endl;
            break;
        }

    } while ( choice != BACK && choice != SELECT );


    if( choice == SELECT )
        return true;
    else 
        return false;
}


void ProfileManager::userI()
{
    std::string usrname;
    std::string passwd;
    int choice;
    bool startSoothsayer;

    const int CREATE = 0;
    const int REMOVE = 1;
    const int AUTHENTICATE = 2;
    const int QUIT = 3;

	
    do {
        // clear startSoothsayer flag
        startSoothsayer = false;
		
        std::cout << "passwd file printout:" << std::endl
                  << "---------------------" << std::endl;
        printPasswd();

        std::cout << std::endl
                  << "Available users" << std::endl
                  << "---------------" << std::endl;
        printUsers();	

        std::cout << std::endl
                  << CREATE << " - create user" << std::endl
                  << REMOVE << " - remove user" << std::endl
                  << AUTHENTICATE << " - authenticate user" <<std::endl
                  << QUIT << " - quit" << std::endl
                  << "Enter choice: ";
        std::cin >> choice;

        switch( choice ) {
	case CREATE:
            std::cout << "Enter username: ";
            std::cin >> usrname;
            std::cout << "Enter password: ";		
            std::cin >> passwd;
            createUser( usrname, passwd );
            break;
	case REMOVE:
            std::cout << "Enter username: ";
            std::cin >> usrname;
            std::cout << "Enter password: ";		
            std::cin >> passwd;

            removeUser( usrname, passwd );
            break;
	case AUTHENTICATE:
            std::cout << "Enter username: ";
            std::cin >> usrname;
            std::cout << "Enter password: ";		
            std::cin >> passwd;

            if( authenticateUser( usrname, passwd ) )
                startSoothsayer = profileI();
            else
                std::cerr << "Authentication failed!" 
                          << std::endl;
            break;
	case QUIT:
            std::cout << "Exiting..." << std::endl;
            exit(0);
            break;
	default:
            std::cerr << "Wrong choice.\a Please try again."
                      << std::endl;
            break;
        }

    } while( choice != QUIT && startSoothsayer == false );
}
