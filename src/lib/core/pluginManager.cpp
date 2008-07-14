
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

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


#include "pluginManager.h"

/** Constructor
 *
 * Initializes HistoryTracker and Predictor handles.
 * Creates pool of available Plugin objects by invoking init() method.
 *
 */
PluginManager::PluginManager( HistoryTracker& ht, Predictor& p )
    : historyTracker( ht ), predictor( p )
{
    PLUGIN_PATH = DEFAULT_PLUGIN_PATH;
    PLUGIN_SUFFIX = DEFAULT_PLUGIN_SUFFIX;

    // Initialise libltdl libtool dynamic loading library
    // Destructor will take care of calling lt_dlexit(),
    // effectively destroying the dynamically loaded modules.
    int ok;
    ok = lt_dlinit();
    assert( ok == 0 ); // Should deal with this using exceptions...

    // Initialise PluginManager
    init();
}


/** Destructor
 *
 * To prevent memory leaks, plugin objects have to be explicitly
 * destroyed. This is accomplished by invoking destroyPlugin() method
 * on each existing plugin object. The dynamically loaded shared
 * library corresponding to each plugin is also unloaded.
 *
 */
PluginManager::~PluginManager()
{
    // Create plugins iterator
    std::vector< Plugin* >::const_iterator i;


    for( i=plugins.begin(); i!=plugins.end(); i++) {

        // Destroy existing Plugin objects and close library
        destroyPlugin( *i );

        // Remove pointer to Plugin in Predictor object
        predictor.removePlugin( *i );

    }
    // Clear out existing plugins vector (not strictly necessary)
    plugins.clear();

    // Clean libltdl library up
    lt_dlexit();

}


/** Initialize plugin manager
 *
 * Scans the contents of the plugin directory and loads every valid
 * shared library and creates the corresponding plugin object via
 * init() method invocation.
 *
 */
void PluginManager::init()
{
    DIR* dirPtr;
    struct dirent* dirEntryPtr;

    dirPtr = opendir( PLUGIN_PATH.c_str() );

    if( dirPtr != NULL ) {
        while( ( dirEntryPtr = readdir( dirPtr ) ) ) {
            if( checkSuffix(dirEntryPtr->d_name, PLUGIN_SUFFIX.c_str() ) ) {

                std::cout << "Found Plugin: "
                          << PLUGIN_PATH
                          << dirEntryPtr->d_name << std::endl;
				
                // create Plugin object
                Plugin* pPtr;
                pPtr = createPlugin( dirEntryPtr->d_name );
				
                // store pointer to Plugin object in plugins
                plugins.push_back( pPtr );

            }
        }
        (void) closedir( dirPtr );
    } else {
        perror( "Couldn't open the directory" );
    }
		
}


/** Checks that string @str has suffix @suffix.
 * @str string to check
 * @suffix suffix
 *
 * Returns true if string @str has suffix @suffix, false otherwise.
 *
 * Length of string and suffix are compared. Length of string has to be greater than length of suffix.
 *
 * If string is '.so', checkSuffix() returns false.
 *
 */
bool PluginManager::checkSuffix( const char* str, const char* suffix ) const
{
    int str_length    = strlen( str );
    int suffix_length = strlen( suffix );

    if( str_length > suffix_length &&
        strcmp( suffix, &str[ str_length - suffix_length ] ) == 0 )
        return true;
    else
        return false;
}


/** Load options into plugin
 *
 */
void PluginManager::loadOptions()
{
    // TODO <===================================================
}


/** Creates a DOM containing each plugin and its options.
 *
 * Parameter @root is the container of the plugins elements.
 *
 * Plugin options differ for each Plugin object. Plugin objects are
 * expected to supply a description of the options that they accept by
 * invoking getOptionTemplate(). Based on this information, XML
 * elements are added to the DOM. Options default values are obtained
 * through the getOptionDefault() call.
 *
 */
void PluginManager::createProfile( TiXmlNode* root )
{
    TiXmlNode* plugin;
    TiXmlNode* element;
    TiXmlNode* node;

    std::vector< Plugin* >::const_iterator i;
    std::vector<std::string> optionList;
    std::vector<std::string>::const_iterator j;

    // for each Plugin in plugins
    for( i = plugins.begin() ; i != plugins.end() ; i++ ) {
		
        // create plugin container 
        plugin = root->InsertEndChild( 
            TiXmlElement( (*i)->getName().c_str() ) );
        assert( plugin );
        if( plugin ) {

            // get list of options available to current plugin
            optionList = (*i)->getOptionTemplate();

            // for each option in the list
            for( j = optionList.begin();
                 j != optionList.end() ;
                 j++ ) {

                // option name is string in optionList
                element = plugin->InsertEndChild(
                    TiXmlElement( j->c_str() ) );
                assert( element );
                if( element ) {
					
                    // text is option default value
                    node = element->InsertEndChild( TiXmlText( (*i)->getOptionDefault( *j ).c_str() ) );
                    assert( node );
                }
            }
        }
    }
					
			


    //	plugin = plugins->InsertEndChild( TiXmlElement( "DummyPlugin" ) );
    //	assert( plugin );
    //	if( plugin ) {
    //		element = plugin->InsertEndChild( TiXmlElement( "ACTIVE" ) );
    //		assert( element );
    //		if( element ) {
    //			node = element->InsertEndChild( TiXmlText( "yes" ) );
    //			assert( node );
    //		}
    //	}

}


/** Register all active Plugin objects with Predictor.
 *
 * Iterates through all available Plugin objects and registers Plugin
 * object marked as active with Predictor.  A Plugin object is active
 * iff its ACTIVE option value is set to "yes".
 *
 */
void PluginManager::registerPlugins() const
{
    std::vector< Plugin* >::const_iterator i;
    std::string value;

    for( i = plugins.begin(); i != plugins.end(); i++ ) {
		
        value = (*i)->getOptionValue( "ACTIVE" );

        if( value == "yes" ) {
			
            std::cout << "Registering " << (*i)->getName()
                      << " with object Predictor..." << std::endl;

            predictor.addPlugin( *i );
        } else {
			
            std::cout << (*i)->getName() << " not active. "
                      << "Skipping registration..." << std::endl;
        }
    }
}

/**
 *
 */
void PluginManager::unregisterPlugins() const
{
    // TODO <===================================================
}


/** Dynamically load a valid shared library and instantiates the Plugin object it implements.
 *
 * @param libplugin is the qualified name of the shared library.
 *
 * Dynamically loads the library and resolves the exported symbols
 * required to instantiate the Plugin object.
 * Returns the newly created Plugin object.
 *
 */
Plugin* PluginManager::createPlugin( const std::string libplugin )
{
    std::cout << "Loading " << libplugin << " plugin library up..."
              << std::endl;

    // Create library name ===> PLUGIN _PATH should be global variable <===
    //                                        or configurable
    std::string lib_name = PLUGIN_PATH + libplugin;
		
    // load the library
    //	void* lib_handle = dlopen( lib_name.data(),
    //				   RTLD_LAZY);
    lt_dlhandle lib_handle;
    lib_handle = lt_dlopen( lib_name.data() );
    if ( !lib_handle ) {
        std::cerr << "Cannot load library: " << lt_dlerror() << '\n';
        abort();
    }
		
    // load the create symbol
    create_t* create = (create_t*) lt_dlsym( lib_handle, "create" );
    if ( !create ) {
        std::cerr << "Cannot load symbols: " << lt_dlerror() << '\n';
        abort();
    }
		
    // create an instance of the class
    Plugin* pluginPtr = create( historyTracker );
    assert( pluginPtr != NULL );

    // save library name
    pluginPtr->setLibFilename( lib_name );

    // save library handle
    pluginPtr->setLibHandle( lib_handle );

    std::cout << "Plugin " << pluginPtr->getName() << " created."
              << std::endl;

    // return pointer to newly created plugin
    return pluginPtr;
}


/** Destroys Plugin object and unloads the corresponding library.
 *
 * @param pluginPtr is a pointer to the Plugin object to be destroyed
 * 
 * The handle to the corresponding library to be unloaded is obtained
 * from the Plugin object itself.
 *
 */
void PluginManager::destroyPlugin( Plugin* pluginPtr )
{
    std::cout << "Destroying plugin "
              << pluginPtr->getName() << "...";

    lt_dlhandle lib_handle = pluginPtr->getLibHandle();

    // load destroy symbol
    destroy_t* destroy = (destroy_t*) lt_dlsym(lib_handle, "destroy");
		
    // destroy plugin object
    destroy( pluginPtr );

    // unload library
    lt_dlclose( lib_handle );

    std::cout << " Destroyed." << std::endl;
}
