
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

#include <stdio.h>

#ifdef _WIN32
#  define PRI_SIZE_T_SPECIFIER "%Iu"
#else
#  define PRI_SIZE_T_SPECIFIER "%zu"
#endif

static const char* PAST   = "Prediction is very difficult, especially about the f";
static const char* FUTURE = "";

static const char* get_past_stream(void* arg)
{
    return (char*) arg;
}

static const char* get_future_stream(void* arg)
{
    return (char*) arg;
}


int main()
{
    presage_t      prsg;
    char*          completion;
    char**         prediction;
    char*          context;
    int            context_change;
    char*          prefix;
    char*          value;
    size_t         i;

    if (PRESAGE_OK != presage_new_with_config (get_past_stream,
					       (void *) PAST,
					       get_future_stream,
					       (void *) FUTURE,
					       "presage_c_demo.xml",
					       &prsg))
    {
	return PRESAGE_ERROR;
    }
    

    if (PRESAGE_OK == presage_predict (prsg, &prediction))
    {
	for (i = 0; prediction[i] != 0; i++)
	    printf ("prediction[" PRI_SIZE_T_SPECIFIER "]: %s\n", i, prediction[i]);
	presage_free_string_array (prediction);
    }

    if (PRESAGE_OK == presage_completion (prsg, "future", &completion))
    {
	printf ("completion: %s\n", completion);
	presage_free_string (completion);
    }

    if (PRESAGE_OK == presage_context (prsg, &context))
    {
	printf ("context: %s\n", context);
	presage_free_string (context);
    }

    if (PRESAGE_OK == presage_context_change (prsg, &context_change))
    {
	printf ("context_change: %d\n", context_change);
    }

    if (PRESAGE_OK == presage_prefix (prsg, &prefix))
    {
	printf ("prefix: %s\n", prefix);
	presage_free_string (prefix);
    }

    if (PRESAGE_OK == presage_config (prsg, "Presage.Selector.SUGGESTIONS", &value))
    {
	printf ("SUGGESTIONS: %s\n", value);
	presage_free_string (value);
    }

    presage_config_set (prsg, "Presage.Selector.SUGGESTIONS", "10");

    presage_learn (prsg, "Prediction is very difficult, especially about the future.");

    if (PRESAGE_OK == presage_predict (prsg, &prediction))
    {
	for (i = 0; prediction[i] != 0; i++)
	    printf ("prediction[" PRI_SIZE_T_SPECIFIER "]: %s\n", i, prediction[i]);
	presage_free_string_array (prediction);    
    }

    presage_save_config (prsg);

    presage_free (prsg);

    return 0;
}
