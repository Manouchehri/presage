
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


#include "presage.h"

#include "core/profileManager.h"
#include "core/predictorRegistry.h"
#include "core/context_tracker/contextTracker.h"
#include "core/selector.h"
#include "core/predictorActivator.h"

Presage::Presage (PresageCallback* callback)
    throw (PresageException)
{
    profileManager = new ProfileManager();
    configuration = profileManager->get_configuration();
    predictorRegistry = new PredictorRegistry(configuration);
    contextTracker = new ContextTracker(configuration, predictorRegistry, callback);
    predictorActivator = new PredictorActivator(configuration, predictorRegistry, contextTracker);
    selector = new Selector(configuration, contextTracker);
}

Presage::Presage (PresageCallback* callback, const std::string config_filename)
    throw (PresageException)
{
    profileManager = new ProfileManager(config_filename);
    configuration = profileManager->get_configuration();
    predictorRegistry = new PredictorRegistry(configuration);
    contextTracker = new ContextTracker(configuration, predictorRegistry, callback);
    predictorActivator = new PredictorActivator(configuration, predictorRegistry, contextTracker);
    selector = new Selector(configuration, contextTracker);
}

Presage::~Presage()
{
    delete selector;
    delete predictorActivator;
    delete contextTracker;
    delete predictorRegistry;
    delete profileManager;
}

std::vector<std::string> Presage::predict ()
    throw (PresageException)
{
    std::vector<std::string> result;

    unsigned int multiplier = 1;
    Prediction prediction = predictorActivator->predict(multiplier++, 0);
    result = selector->select(prediction);

    Prediction previous_prediction = prediction;
    while ((result.size() < (selector->get_suggestions()))
	   && (prediction = predictorActivator->predict(multiplier++, 0)).size() > previous_prediction.size()) {
	// while the number of predicted tokens is lower than desired,
	// search harder (i.e. higher multiplier) for a prediction of
	// sufficient size (i.e. that satisfies selector), as long as
	// the result of current prediction is greater than the
	// previous prediction (i.e. we are finding new tokens).
	result = selector->select(prediction);
	previous_prediction = prediction;
    }

    contextTracker->update();

    return result;
}

std::multimap<double, std::string> Presage::predict (std::vector<std::string> filter)
    throw (PresageException)
{
    std::multimap<double, std::string> result;

    std::vector<std::string> selection;
    const char** internal_filter = 0;
    if(filter.size()>0)
    {
	// convert filter to internal representation - currently a null
	// terminated const char**
	internal_filter = new const char*[filter.size() + 1];
	for (std::vector<std::string>::size_type i = 0; i < filter.size(); i++) {
	    internal_filter[i] = filter[i].c_str();
	}
	internal_filter[filter.size()] = 0;
    }

    unsigned int multiplier = 1;
    Prediction prediction = predictorActivator->predict(multiplier++, internal_filter);
    selection = selector->select(prediction);

    Prediction previous_prediction = prediction;
    while ((selection.size() < (selector->get_suggestions()))
	   && (prediction = predictorActivator->predict(multiplier++, internal_filter)).size() > previous_prediction.size()) {
	// while the number of predicted tokens is lower than desired,
	// search harder (i.e. higher multiplier) for a prediction of
	// sufficient size (i.e. that satisfies selector), as long as
	// the selection of current prediction is greater than the
	// previous prediction (i.e. we are finding new tokens).
	selection = selector->select(prediction);
	previous_prediction = prediction;
    }

    delete[] internal_filter;

    for (std::vector<std::string>::const_iterator it = selection.begin();
	 it != selection.end();
	 it++) {
        std::pair<const double, std::string> p(prediction.getSuggestion(*it).getProbability(),
					       (*it));
	result.insert(p);
    }

    contextTracker->update();

    return result;
}

void Presage::learn(const std::string text) const
    throw (PresageException)
{
    contextTracker->learn(text); // TODO: can pass additional param to
				 // learn to specify offline learning
}

PresageCallback* Presage::callback (PresageCallback* callback)
    throw (PresageException)
{
    return const_cast<PresageCallback*>(contextTracker->callback(callback));
}

std::string Presage::completion (const std::string str)
    throw (PresageException)
{
    // There are two types of completions: normal and erasing.
    // normal_completion  = prefix + remainder
    // erasing_completion = eraser + prefix + remainder
    //
    // or, given that token = prefix + remainder
    // normal_completion  = token
    // erasing_completion = eraser + token
    //
    // where eraser = ^H+ (one or more backspace characters)
    //
    // offset to first non ^H character in completion (^H are inserted
    // by abbreviation expansion predictor to erase abbreviation from
    // stream)
    //
    std::string result;

    std::string::size_type offset = str.find_first_not_of('\b');
    if (offset == 0) {
        // normal completion,
        // ensure that current prefix is a substring of completion
        // token and set result
        //
        if (contextTracker->isCompletionValid(str)) {
            std::string prefix = contextTracker->getPrefix();
	    result = str.substr(prefix.size());
        } else {
            std::string message = "[Presage] Error: token '";
	    message += str;
	    message += "' does not match prefix: ";
	    message += contextTracker->getPrefix();
	    throw PresageException(PRESAGE_TOKEN_PREFIX_MISMATCH_ERROR, message);
        }
    } else {
        // erasing completion,
        // pass it to tracker in its entirety
        //
        result = str;
    }

    // if (append_trailing_space_is_on()) // TODO: make this configurable
    result += ' ';

    return result;
}

std::string Presage::context () const
    throw (PresageException)
{
    return contextTracker->getPastStream();
}

bool Presage::context_change () const
    throw (PresageException)
{
    return contextTracker->contextChange();
}

std::string Presage::prefix () const
    throw (PresageException)
{
    return contextTracker->getPrefix();
}

std::string Presage::config (const std::string variable) const
    throw (PresageException)
{
    return configuration->find (variable)->get_value ();
}

void Presage::config (const std::string variable, const std::string value) const
    throw (PresageException)
{
    configuration->insert (variable, value);
}

void Presage::save_config () const
    throw (PresageException)
{
    profileManager->save_profile ();
}



struct _presage {
  PresageCallback* presage_callback_object;
  Presage*         presage_object;
};

class CPresageCallback : public PresageCallback
{
public:
  CPresageCallback (_presage_callback_get_past_stream   past,
		    void* past_arg,
		    _presage_callback_get_future_stream future,
		    void* future_arg)
    : m_get_past_stream_cb (past),
      m_get_past_stream_cb_arg (past_arg),
      m_get_future_stream_cb (future),
      m_get_future_stream_cb_arg (future_arg)
  { }

  virtual ~CPresageCallback() { }

  std::string get_past_stream() const {
    return (*m_get_past_stream_cb) (m_get_past_stream_cb_arg);
  }

  std::string get_future_stream() const {
    return (*m_get_future_stream_cb) (m_get_future_stream_cb_arg);
  }

private:
  _presage_callback_get_past_stream   m_get_past_stream_cb;
  void*                               m_get_past_stream_cb_arg;
  _presage_callback_get_future_stream m_get_future_stream_cb;
  void*                               m_get_future_stream_cb_arg;
};

#define presage_exception_handler(CODE) \
    try \
    { \
	CODE; \
    } \
    catch (PresageException& ex) \
    { \
	return ex.code (); \
    } \
    return PRESAGE_OK;

#define presage_exception_handler_with_result(CODE) \
    try \
    { \
	CODE; \
    } \
    catch (PresageException& ex) \
    { \
        (*result) = 0; \
	return ex.code (); \
    } \
    return PRESAGE_OK;

static char* alloc_c_str (const std::string& str)
{
    char* result_c_str = (char*) malloc (str.size() + 1);
    if (result_c_str)
	strcpy (result_c_str, str.c_str());
    return result_c_str;    
}

presage_error_code_t presage_new (_presage_callback_get_past_stream past_stream_cb,
				  void* past_stream_cb_arg,
				  _presage_callback_get_future_stream future_stream_cb,
				  void* future_stream_cb_arg,
				  presage_t* result)
{
    presage_exception_handler_with_result
    (
	(*result) = (presage_t) malloc (sizeof (_presage));
        if (*result != NULL)
        {
            (*result)->presage_callback_object = new CPresageCallback (past_stream_cb,
                                                                       past_stream_cb_arg,
                                                                       future_stream_cb,
                                                                       future_stream_cb_arg);
            (*result)->presage_object          = new Presage          ((*result)->presage_callback_object);
        }
	
    );
}

presage_error_code_t presage_new_with_config (_presage_callback_get_past_stream past_stream_cb,
					      void* past_stream_cb_arg,
					      _presage_callback_get_future_stream future_stream_cb,
					      void* future_stream_cb_arg,
					      const char* config,
					      presage_t* result)
{
    presage_exception_handler_with_result
    (
	(*result) = (presage_t) malloc (sizeof (_presage));
        if (*result != NULL)
        {
	
            (*result)->presage_callback_object = new CPresageCallback (past_stream_cb,
                                                                       past_stream_cb_arg,
                                                                       future_stream_cb,
                                                                       future_stream_cb_arg);
            (*result)->presage_object          = new Presage          ((*result)->presage_callback_object, config);
        }
    );
}

void presage_free (presage_t prsg)
{
    if (prsg)
    {
	delete prsg->presage_object;
	delete prsg->presage_callback_object;
	
	free (prsg);
    }
}

void presage_free_string (char* str)
{
    free (str);
}

void presage_free_string_array (char** strs)
{
    if (strs) {
	for (size_t t = 0; strs[t] != 0; t++) {
	    free (strs[t]);
	}
	free (strs);
    }
}

presage_error_code_t presage_predict (presage_t prsg, char*** result)
{
    presage_exception_handler_with_result
    (
	std::vector<std::string> prediction = prsg->presage_object->predict();
	
	size_t prediction_c_str_size = prediction.size() + 1;
	char** prediction_c_str      = (char**) malloc (prediction_c_str_size * sizeof(char*));
        if (prediction_c_str != NULL)
        {
            memset (prediction_c_str, 0, prediction_c_str_size * sizeof(char*));

            size_t i = 0;
            while (i < prediction_c_str_size - 1) {
                prediction_c_str[i] = (char*) malloc (prediction[i].size() + 1);
                if (prediction_c_str[i] != NULL)
                    strcpy (prediction_c_str[i], prediction[i].c_str());
                i++;
            }
            prediction_c_str[i] = 0;
        }
	
	*result = prediction_c_str;
    );
}

presage_error_code_t presage_learn (presage_t prsg, const char* text)
{
    presage_exception_handler
    (
	prsg->presage_object->learn (text);
    );
}

presage_error_code_t presage_completion (presage_t prsg, const char* token, char** result)
{
    presage_exception_handler_with_result
    (
	*result = alloc_c_str (prsg->presage_object->completion (token));
    );
}

presage_error_code_t presage_context (presage_t prsg, char** result)
{
    presage_exception_handler_with_result
    (
	*result = alloc_c_str (prsg->presage_object->context ());
    );
}

presage_error_code_t presage_context_change (presage_t prsg, int* result)
{
    presage_exception_handler_with_result
    (
	*result = prsg->presage_object->context_change ();
    );
}

presage_error_code_t presage_prefix (presage_t prsg, char** result)
{
    presage_exception_handler_with_result
    (
	*result = alloc_c_str (prsg->presage_object->prefix ());
    );
}

presage_error_code_t presage_config (presage_t prsg, const char* variable, char** result)
{
    presage_exception_handler_with_result
    (
	*result = alloc_c_str (prsg->presage_object->config (variable));
    );
}

presage_error_code_t presage_config_set (presage_t prsg, const char* variable, const char* value)
{
    presage_exception_handler 
    (
	prsg->presage_object->config (variable, value)
    );
}
	    
presage_error_code_t presage_save_config (presage_t prsg)
{
    presage_exception_handler 
    (
	prsg->presage_object->save_config ()
    );
}
