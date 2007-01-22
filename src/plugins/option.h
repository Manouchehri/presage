
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


#ifndef SOOTH_OPTION
#define SOOTH_OPTION


#include <string>


/** OptionType encodes the type of the Option object.
 *
 */
enum OptionType { INT=0,    /**< Integer value (i.e. 10)                  */
		  BOOL,     /**< Boolean value (i.e. yes/no)              */
		  DOUBLE,   /**< Double value                             */
		  STRING,   /**< Alphanumeric string (i.e. linux-2.4.26)  */
		  INT_RANGE,/**< Integer interval (i.e. 10-15)            */
		  INT_LIST  /**< Integer list (i.e. 1,2,3,4,5)            */
};

/** Option class embodies options passed to plugin objects.
 *
 */
class Option {
public:
    Option(const std::string,        // default
	   const enum OptionType,    // type
	   const std::string = "",   // name
	   const std::string = "" ); // description

    Option(const std::string,        // value       <=| I
	   const std::string,        // default       | DO
	   const enum OptionType,    // type          | NOT
	   const std::string = "",   // name          | LIKE
	   const std::string = "" ); // description   | THIS!

    ~Option();


    OptionType  getType()        const;  /**< Returns option type     */
    std::string getName()        const;  /**< Returns name            */
    std::string getDescription() const;  /**< Returns description     */
    std::string getDefault()     const;  /**< Returns option default  */
    std::string getValue()       const;  /**< Returns option value    */

    bool setValue(const std::string);    /**< Set option value        */


    /** Draw option graphical user interface element */
    void draw() const; 


private:
    std::string name;         /**< Option name          */
    std::string description;  /**< Option description   */
    std::string defaultValue; /**< Option default value */
    std::string value;        /**< Option value         */

    OptionType type;          /**< Option type          */

    /** Validate option value based on option type      */
    bool validate( std::string );
    bool iterate( std::string, int (*)(int) throw () );

    void setDefault(const std::string);	
};


#endif // SOOTH_OPTION
