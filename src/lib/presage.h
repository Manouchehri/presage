
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


#ifndef PRESAGE
#define PRESAGE

#include "presageException.h"
#include "presageCallback.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *    Presage C API starts here
 */

    typedef struct _presage* presage_t;

    presage_error_code_t presage_new                 (_presage_callback_get_past_stream past_stream_cb,
						      void* past_stream_cb_arg,
						      _presage_callback_get_future_stream future_stream_cb,
						      void* future_stream_cb_arg,
						      presage_t* result);
    
    presage_error_code_t presage_new_with_config     (_presage_callback_get_past_stream past,
						      void* past_stream_cb_arg,
						      _presage_callback_get_future_stream future_stream_cb,
						      void* future_stream_cb_arg,
						      const char* config,
						      presage_t* result);
    
    void                 presage_free                (presage_t prsg);
    
    void                 presage_free_string         (char* str);
    void                 presage_free_string_array   (char** str);

    presage_error_code_t presage_predict             (presage_t prsg,
						      char*** result);
    
    presage_error_code_t presage_completion          (presage_t prsg,
						      const char* token,
						      char** result);

    presage_error_code_t presage_context             (presage_t prsg, 
						      char** result);

    presage_error_code_t presage_context_change      (presage_t prsg,
						      int* result);

    presage_error_code_t presage_prefix              (presage_t prsg,
						      char** result);

    presage_error_code_t presage_config              (presage_t prsg,
						      const char* variable,
						      char** result);

    presage_error_code_t presage_config_set          (presage_t prsg,
						      const char* variable,
						      const char* value);
    
    presage_error_code_t presage_save_config         (presage_t prsg);


/*
 *    Presage C API ends here
 */
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#ifndef _MSC_VER
/*
 *    Presage C++ API starts here
 */


#include <string>
#include <vector>
#include <map>

/* Forward declarations, not part of presage C++ API */
class Configuration;
class ProfileManager;
class Profile;
class ContextTracker;
class PredictorRegistry;
class PredictorActivator;
class Selector;

/** \brief Presage, the intelligent predictive text entry platform.
 */
class Presage {
public:
    /** Creates and initializes presage.
     *
     * \param callback is a user-supplied implementation of PresageCallback interface
     * 
     * Presage does not take ownership of the callback object.
     */
    Presage(PresageCallback* callback) throw (PresageException);


    /** Creates and initializes presage with supplied configuration.
     *
     * \param callback is a user-supplied implementation of PresageCallback interface
     * \param config path to configuration file
     *
     * Presage does not take ownership of the callback object.
     */
    Presage(PresageCallback* callback, const std::string config) throw (PresageException);


    /** Destroys presage.
     */
    ~Presage();

    /** \brief Obtain a prediction.
     *
     * This method requests that presage generates a prediction based
     * on the current context.
     *
     * \return prediction (vector of strings) based on the current
     * context.
     *
     */
    std::vector<std::string> predict() throw (PresageException);

    /** \brief Obtain a prediction that matches the supplied token
     *         filter.
     *
     * \param filter a vector of strings to use to filter the
     * prediction for desired tokens.  I.e. If the current prefix is
     * "gr" and the filter is ["ea", "an"], then only words starting
     * with "grea" or "gran" such as "great" or "grand" will be
     * returned in the prediction.
     *
     * \return prediction containing only tokens that begin with one
     * of the filter tokens.
     *
     */
    std::multimap<double, std::string> predict(std::vector<std::string> filter) throw (PresageException);

    /** \brief Callback getter/setter.
     *
     * \param callback to be used by presage (pass a null pointer to
     * obtain callback to current callback without modifying it)
     *
     * \return pointer to previously used callback
     */
    PresageCallback* callback(PresageCallback* callback) throw (PresageException);

    /** \brief Request presage to return the completion string for the given predicted token.
     *
     * Requests presage to return the completion string. The
     * completion string is defined as the string which, when appended
     * to the current prefix, forms the token passed as the str
     * argument.
     *
     * \param str successful prediction, for which a completion string
     * is requested
     *
     * \return completion string
     */
    std::string completion(std::string str) throw (PresageException);

    /** \brief Returns the text entered so far.
     *
     * \return context, text entered so far.
     */
    std::string context() const throw (PresageException);

    /** \brief Returns true if a context change occured.
     *
     * \return true if a context change occured after the last update
     * or predict calls, or false otherwise.
     */
    bool context_change() const throw (PresageException);

    /** \brief Returns the current prefix.
     *
     * \return prefix
     */
    std::string prefix() const throw (PresageException);

    /** \brief Gets the value of specified configuration variable.
     *
     * Programmatically get the value currently assigned to the
     * specified configuration \param variable
     *
     * \return value assigned to configuration variable.
     */
    std::string config(const std::string variable) const throw (PresageException);

    /** \brief Sets the value of specified configuration variable.
     *
     * Programmatically set the specified configuration \param
     * variable to \param value . This will override the setting read
     * from the configuration file in use.
     *
     */
    void config(const std::string variable, const std::string value) const throw (PresageException);

    /** \brief Save current configuration to file.
     *
     * Call this method to persist current presage configuration to
     * file. The configuration data will be saved to the currently
     * active XML profile.
     *
     */
    void save_config() const throw (PresageException);

    /*
     * Presage public API ends here
     */

private:
    ProfileManager*     profileManager;
    Configuration*      configuration;
    PredictorRegistry*     predictorRegistry;
    ContextTracker*     contextTracker;
    PredictorActivator* predictorActivator;
    Selector*           selector;

};

/*
 *    Presage C++ API ends here
 */
#endif /* _MSC_VER */
#endif /* __cplusplus */

/** \mainpage

    \section intro_section Introduction

    Presage is an intelligent predictive text entry
    platform. Presage exploits <a
    href="http://en.wikipedia.org/wiki/Redundancy_%28information_theory%29">redundant
    information</a> embedded in natural languages to generate
    predictions. Presage's modular and pluggable <a
    href="?q=node/14">architecture</a> allows its <a
    href="http://en.wikipedia.org/wiki/Language_modeling">language
    model</a> to be extended and customized to utilize statistical,
    syntactic, and semantic information sources.

    A predictive text entry system attempts to improve ease and speed of
    textual input. Word prediction consists in computing which word tokens
    or word completions are most likely to be entered next. The system
    analyses the text already entered and combines the information thus
    extracted with other information sources to calculate a set of most
    probable tokens.

    A typical presage-based application would display the set of most
    probable tokens (i.e. a list of suggestions) to the user and
    automatically enter the desired token after the user selects it. If
    the list of suggestions does not contain the desired word, the user
    continues entering text until the correct suggestion is offered or
    until the user is done entering text.

    Presage is fundamentally different from predictive <a
    href="http://en.wikipedia.org/wiki/T9_%28predictive_text%29">input
    technologies commonly found on mobile phones</a>, which might more
    accurately be described as 'disambiguating text entry' rather than
    'predictive text entry' systems.  Such systems do not try to guess
    what the user intends to write in the future, only to determine what
    they most-likely intend to write in the present, given their past
    input.  Presage, on the other hand, actively predicts the what the
    user intends to write, and only reverts to <a
    href="http://en.wikipedia.org/wiki/Word_completion">word
    completion</a> mode if the prediction did not contain the desired
    token.

    Presage is <a
    href="http://www.gnu.org/philosophy/free-sw.html">free
    software</a>. It is distributed under the term of the <a
    href="http://www.gnu.org/copyleft/gpl.html">General Public
    License</a>.

    \author Matteo Vescovi

    \section getting_started_section Getting started
    \include getting_started.txt

*/

#endif /* PRESAGE */
