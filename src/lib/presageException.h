
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


#ifndef PRESAGE_EXCEPTION
#define PRESAGE_EXCEPTION

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
	PRESAGE_OK = 0,    /* successful result */
	PRESAGE_ERROR,     /* generic/unknown presage error */
	PRESAGE_TOKEN_PREFIX_MISMATCH_ERROR,
	PRESAGE_SMOOTHED_NGRAM_PREDICTOR_LEARN_ERROR,
	PRESAGE_CONFIG_VARIABLE_ERROR,
	PRESAGE_INVALID_CALLBACK_ERROR,
	PRESAGE_INVALID_SUGGESTION_ERROR,
	PRESAGE_INIT_PREDICTOR_ERROR,
	PRESAGE_SQLITE_OPEN_DATABASE_ERROR,
	PRESAGE_SQLITE_EXECUTE_SQL_ERROR
    } presage_error_code_t;

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#ifndef _MSC_VER

#include <exception>
#include <string>

/** When thrown, provides information about an error that has occurred within presage.
 *
 * If an error occurs within presage, an exception is thrown, and
 * this is the object that encapsulates the details of the problem.
 * 
 * Application using presage should always ensure that exceptions
 * are caught by enclosing all presage methods within a try{}
 * catch(PresageException& e) {} block.
 *
 */
class PresageException : public std::exception {
public:
    PresageException(presage_error_code_t code, const std::string& msg) throw()
	: m_details (msg),
	  m_code    (code)
    {
	// intentionally empty
    }

    virtual ~PresageException() throw()
    {
	// intentionally empty
    }

    virtual const char* what() const throw()
    {
	return m_details.c_str();
    }

    virtual const presage_error_code_t code() const throw()
    {
	return m_code;
    }
    
private:
    std::string          m_details;
    presage_error_code_t m_code;

};

#endif /* _MSC_VER */
#endif /* __cplusplus */

#endif /* PRESAGE_EXCEPTION */
