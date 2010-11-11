
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


#include "presage_c.h"

#include "presage.h"

#include <stdlib.h>
#include <string.h>

struct _presage {
  PresageCallback* presage_callback_object;
  Presage*         presage_object;
};

class CPresageCallback : public PresageCallback
{
public:
  CPresageCallback (_presage_callback_get_past_stream   past,
		    _presage_callback_get_future_stream future)
    : m_get_past_stream (past),
      m_get_future_stream (future) 
  { }

  virtual ~CPresageCallback() { }

  std::string get_past_stream() const {
    return (*m_get_past_stream) ();
  }

  std::string get_future_stream() const {
    return (*m_get_future_stream) ();
  }

private:
  _presage_callback_get_past_stream   m_get_past_stream;
  _presage_callback_get_future_stream m_get_future_stream;
};

static char* alloc_c_str (std::string str)
{
    char* result_c_str = (char*) malloc (str.size() + 1);
    if (result_c_str)
	strcpy (result_c_str, str.c_str());
    return result_c_str;    
}

presage_t presage_new (_presage_callback_get_past_stream past,
		       _presage_callback_get_future_stream future)
{
    presage_t prsg = (presage_t) malloc (sizeof (_presage));
    
    prsg->presage_callback_object = new CPresageCallback (past, future);
    prsg->presage_object          = new Presage          (prsg->presage_callback_object);

    return prsg;
}

presage_t presage_new_with_config (_presage_callback_get_past_stream past,
				   _presage_callback_get_future_stream future,
				   const char* config)
{
    presage_t prsg = (presage_t) malloc (sizeof (_presage));
    
    prsg->presage_callback_object = new CPresageCallback (past, future);
    prsg->presage_object          = new Presage          (prsg->presage_callback_object, config);

    return prsg;
}

void presage_delete(presage_t prsg)
{
    delete prsg->presage_object;
    delete prsg->presage_callback_object;
    
    free (prsg);
}

void presage_free_string (char* str)
{
    if (str)
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

char** presage_predict (presage_t prsg)
{
    std::vector<std::string> prediction = prsg->presage_object->predict();
    
    size_t prediction_c_str_size = prediction.size() + 1;
    char** prediction_c_str      = (char**) malloc (prediction_c_str_size * sizeof(char*));
    memset (prediction_c_str, 0, prediction_c_str_size * sizeof(char*));

    size_t i = 0;
    while (i < prediction_c_str_size - 1) {
	prediction_c_str[i] = (char*) malloc (prediction[i].size() + 1);
	strcpy (prediction_c_str[i], prediction[i].c_str());
	i++;
    }
    prediction_c_str[i] = 0;

    return prediction_c_str;
}

char* presage_completion (presage_t prsg, const char* token)
{
    return alloc_c_str (prsg->presage_object->completion (token));
}

char* presage_context (presage_t prsg)
{
    return alloc_c_str (prsg->presage_object->context ());
}

int presage_context_change (presage_t prsg)
{
    return prsg->presage_object->context_change ();
}

char* presage_prefix (presage_t prsg)
{
    return alloc_c_str (prsg->presage_object->prefix ());
}

char* presage_config (presage_t prsg, const char* variable)
{
    return alloc_c_str (prsg->presage_object->config (variable));
}

void presage_config_set (presage_t prsg, const char* variable, const char* value)
{
    prsg->presage_object->config (variable, value);
}

void presage_save_config (presage_t prsg)
{
    prsg->presage_object->save_config ();
}
