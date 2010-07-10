
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


#ifndef PRESAGE_LOGGER
#define PRESAGE_LOGGER

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
    inline
    Logger (std::string logger_name,
	    std::basic_ostream<_charT,_Traits>& ostr)
	: outstream(ostr)
	{
	    init(logger_name, "ERROR");
	}

    inline
    Logger (std::string logger_name,
	    std::basic_ostream<_charT,_Traits>& ostr,
	    const std::string& lvl)
	: outstream(ostr)
	{
	    init(logger_name, lvl);
	}

    inline
    void init(const std::string& name, const std::string& lvl)
	{
	    set_name(name);
	    state = new LoggerState();
	    set (state->loggerLevel, lvl);
	    set (state->currentLevel, lvl);
	    state->line_beginning = true;
	}

    // destructor
    inline
    ~Logger ()
	{
	    outstream.flush();
	    delete state;
	}


    // level getters/setters
    inline
    void
    setLevel(const std::string& lvl) const
	{
	    set(state->loggerLevel, lvl);
	}

    inline
    void
    setLevel (Level lvl) const
	{
	    state->loggerLevel = lvl;
	}

    inline
    Level
    getLevel () const
	{
	    return state->loggerLevel;
	}

    inline
    void
    setCurrentLevel (Level lvl) const
	{
	    state->currentLevel = lvl;
	}

    inline
    Level
    getCurrentLevel() const
	{
	    return state->currentLevel;
	}

    inline
    bool
    shouldLog() const
	{
	    return (state->loggerLevel >= state->currentLevel);
	}

    // logging method
    template<typename T>
    friend inline
    const Logger&
    operator<< (const Logger& lgr, const T& msg)
	{
	    //if (lgr.state->loggerLevel >= lgr.state->currentLevel)
	    if (lgr.shouldLog())
	    {
		if (lgr.state->line_beginning) {
		    lgr.outstream << lgr.name;
		    lgr.state->line_beginning = false;
		}
		lgr.outstream << msg;
	    }
	    return lgr;
	}

    // this method is needed by the functions defined by macro
    // define_logger_level_manipulator(LEVEL)
    //
    friend inline
    const Logger&
    operator<< (const Logger& lgr, const Logger& (*fp)(const Logger&))
	{
	    (*fp)(lgr);
	    return lgr;
	}

    inline
    const Logger&
    operator<< (_SetLevel __l) const
	{
	    setLevel(__l._level);
	    return *this;
	}

    inline
    void endl() const
	{
	    //if (state->loggerLevel >= state->currentLevel)
	    if (shouldLog())
	    {
		outstream << std::endl;
		state->line_beginning = true;
	    }
	}

private:
    inline
    void
    set(Level& level, const std::string& lvl) const
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

    inline
    void
    set_name(const std::string& logger_name)
	{
	    name = "[" + logger_name + "] ";
	}

    std::string name;
    std::basic_ostream <_charT, _Traits>& outstream;

    class LoggerState {
    public:
	bool line_beginning;
	Level loggerLevel;
	Level currentLevel;

    };

    LoggerState* state;
};



#define define_logger_level_manipulator(LEVEL)			\
    template <typename _charT, typename _Traits>		\
    inline const Logger<_charT, _Traits>&			\
    LEVEL (const Logger<_charT, _Traits>& lgr)			\
    {								\
	lgr.setCurrentLevel(Logger<_charT, _Traits>::LEVEL);	\
	return lgr;						\
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
inline const Logger<_charT, _Traits>&
endl (const Logger<_charT, _Traits>& lgr)
{
    lgr.endl();
    return lgr;
}


#endif // PRESAGE_LOGGER
