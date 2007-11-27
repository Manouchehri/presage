
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>           *
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


#ifndef SOOTHSAYER_LOGGER
#define SOOTHSAYER_LOGGER

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <iostream>


// manipulators
struct _SetLevel { std::string _level; };

/**
 *  @brief  Manipulator for @c level.
 *  @param  @__l  The new logger level.
 *
 *  Sent to a logger object, this manipulator calls @c setLevel(c) for that
 *  object.
 */
inline _SetLevel
setlevel(std::string __l)
{ 
    _SetLevel __x;
    __x._level = __l;
    return __x; 
}



template <class _charT, class _Traits=std::char_traits<_charT> >
class Logger
{
public:
    // type definitions
    enum Level
    {
        EMERG  = 0, 
        FATAL  = 0,
        ALERT  = 100,
        CRIT   = 200,
        ERROR  = 300, 
        WARN   = 400,
        NOTICE = 500,
        INFO   = 600,
        DEBUG  = 700,
        ALL    = 800
    };

    // constructors
    Logger (std::string logger_name,
	    std::basic_ostream<_charT,_Traits>& ostr)
	: outstream(ostr)
	{
	    set_logger_name(logger_name);
	    loggerLevel = ALL;
	    currentLevel = ALL;
	    line_beginning = true;
	}
  
    Logger (std::string logger_name,
	    std::basic_ostream<_charT,_Traits>& ostr,
	    const std::string& lvl)
	: outstream(ostr)
	{
	    set_logger_name(logger_name);
	    set (loggerLevel, lvl);
	    set (currentLevel, lvl);
	    line_beginning = true;
	}

    // destructor
    ~Logger ()
	{
	    outstream.flush();
	}


    // level getters/setters
    void
    setLevel(const std::string& lvl)
	{
	    set(loggerLevel, lvl);
	}

    void
    setLevel (Level lvl)
	{
	    loggerLevel = lvl;
	}

    Level
    getLevel () const
	{
	    return loggerLevel;
	}

    void
    setCurrentLevel (Level lvl)
	{
	    currentLevel = lvl;
	}

    Level
    getCurrentLevel() const
	{
	    return currentLevel;
	}


    // logging methods

    template<typename T>
    friend Logger&
    operator<< (Logger& lgr, const T& msg)
	{
	    if (lgr.loggerLevel >= lgr.currentLevel)
	    {
		if (lgr.line_beginning) {
		    lgr.outstream << lgr.name;
		    lgr.line_beginning = false;
		}
		lgr.outstream << msg;
	    }
	    return lgr;
	}


//   // this overloaded operator<< is not enough, it results in a
//   // segmentation fault
//   template<typename T>
//   Logger&
//   operator<< (const T& msg)
//   {
//       Level lev = FATAL;
//       if (level >= lev)
//       {
// 	  outstream << msg;
//       }
//   }
    
    inline Logger& 
    operator<< (_SetLevel __l)
	{ 
	    setLevel(__l._level); 
	    return *this; 
	}


    // this method is needed by the functions defined by macro
    // define_logger_level_manipulator(LEVEL)
    //
    inline Logger&
    operator<< (Logger& (*fp)(Logger&))
	{
	    (*fp)(*this);
	    return *this;
	}

    inline void endl()
	{
	    if (loggerLevel >= currentLevel)
	    {
		outstream << std::endl;
		line_beginning = true;
	    }
	}
    
private:
    void set(Level& level, const std::string& lvl)
	{
    	    if (lvl == "EMERG") {
		level = EMERG;
	    } else if (lvl == "FATAL") {
		level = FATAL;
	    } else if (lvl == "ALERT") {
		level = ALERT;
	    } else if (lvl == "CRIT") {
		level = CRIT;
	    } else if (lvl == "ERROR") {
		level = ERROR;
	    } else if (lvl == "WARN") {
		level = WARN;
	    } else if (lvl == "NOTICE") {
		level = NOTICE;
	    } else if (lvl == "INFO") {
		level = INFO;
	    } else if (lvl == "DEBUG") {
		level = DEBUG;
	    } else if (lvl == "ALL") {
		level = ALL;
	    } else {
		level = ERROR;
	    }
	}

    void set_logger_name(const std::string& logger_name)
	{
	    name = "[" + logger_name + "] ";
	}

    std::string name;
    std::basic_ostream <_charT, _Traits>& outstream;

    bool line_beginning;

    Level loggerLevel;
    Level currentLevel;
};



#define define_logger_level_manipulator(LEVEL)    \
template <typename _charT, typename _Traits>      \
inline Logger<_charT, _Traits>&                   \
LEVEL (Logger<_charT, _Traits>& lgr)              \
{                                                 \
    lgr.setCurrentLevel(Logger<_charT, _Traits>::LEVEL); \
    return lgr;                                   \
}                                                 

define_logger_level_manipulator(EMERG );
define_logger_level_manipulator(FATAL );
define_logger_level_manipulator(ALERT );
define_logger_level_manipulator(CRIT  );
define_logger_level_manipulator(ERROR );
define_logger_level_manipulator(WARN  );
define_logger_level_manipulator(NOTICE);
define_logger_level_manipulator(INFO  );
define_logger_level_manipulator(DEBUG );
define_logger_level_manipulator(ALL   );


template <typename _charT, typename _Traits>
inline Logger<_charT, _Traits>&
endl (Logger<_charT, _Traits>& lgr)
{
    lgr.endl();
    return lgr;
}                                                 


#endif // SOOTHSAYER_LOGGER
