
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


/*

LIB /MACHINE:x86 /DEF:presage_c.def
CL /EHsc /Zi presage_c_demo.c presage_c.lib

 */

#include "presage_c.h"

#include <stdio.h>

static const char* STREAM = "did you not sa";
static const char* EMPTY = "";

static const char* get_past_stream()
{
    return STREAM;
}

static const char* get_future_stream()
{
    return EMPTY;
}


int main()
{
    presage        prsg;
    char*          completion;
    char**         prediction;
    char*          context;
    char*          prefix;
    char*          value;
    size_t         i;

    prsg = presage_new (get_past_stream, get_future_stream);

    prediction = presage_predict (prsg);
    for (i = 0; prediction[i] != 0; i++)
	printf ("prediction[%d]: %s\n", i, prediction[i]);
    presage_free_string_array (prediction);

    completion = presage_completion (prsg, "savages");
    printf ("completion: %s\n", completion);
    presage_free_string (completion);

    context = presage_context (prsg);
    printf ("context: %s\n", context);
    presage_free_string (context);

    printf ("context_change: %d\n", presage_context_change (prsg));

    prefix = presage_prefix (prsg);
    printf ("prefix: %s\n", prefix);
    presage_free_string (prefix);

    value = presage_config (prsg, "Presage.Selector.SUGGESTIONS");
    printf ("SUGGESTIONS: %s\n", value);
    presage_free_string (value);

    presage_config_set (prsg, "Presage.Selector.SUGGESTIONS", "10");
    prediction = presage_predict (prsg);
    for (i = 0; prediction[i] != 0; i++)
	printf ("prediction[%d]: %s\n", i, prediction[i]);
    presage_free_string_array (prediction);    

    presage_save_config (prsg);

    presage_delete (prsg);

    return 0;
}
