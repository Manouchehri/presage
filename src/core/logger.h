
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
  Logger (std::basic_ostream<_charT,_Traits>& ostr)
  : outstream(ostr)
  {
      setLevel(ERROR);
  }
  
  Logger (std::basic_ostream<_charT,_Traits>& ostr,
	  const std::string& lvl)
  : outstream(ostr)
  {
      setLevel(ERROR);
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

  void
  setLevel (Level l)
  {
      level = l;
  }

  Level
  getLevel () const
  {
      return level;
  }



  // logging methods

  template<typename T>
  friend Logger&
  operator<< (Logger& lgr, const T& msg)
  {
      Level lev = FATAL;
      if (lgr.level >= lev)
      {
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
    

  private:
  std::basic_ostream <_charT, _Traits>& outstream;
  Level level;
};


#endif // SOOTHSAYER_LOGGER
