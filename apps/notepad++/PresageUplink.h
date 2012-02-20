
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2011  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

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


#ifndef PRESAGE_UPLINK_H
#define PRESAGE_UPLINK_H

/* must match the presage DLL filename */
//const TCHAR PRESAGE_DLL_FILENAME[] = _T("libpresage-1.dll");

/* must match declarations in presageException.h */
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

/* must match declarations in presageCallback.h */
typedef const char* (*_presage_callback_get_past_stream)   (void*);
typedef const char* (*_presage_callback_get_future_stream) (void*);

/* must match declarations in presage.h */
typedef struct _presage* presage_t;

typedef presage_error_code_t (__cdecl * PFUNC_presage_new)
    (_presage_callback_get_past_stream past_stream_cb,
     void* past_stream_cb_arg,
     _presage_callback_get_future_stream future_stream_cb,
     void* future_stream_cb_arg,
     presage_t* result);
    
typedef presage_error_code_t (__cdecl * PFUNC_presage_new_with_config)
    (_presage_callback_get_past_stream past,
     void* past_stream_cb_arg,
     _presage_callback_get_future_stream future_stream_cb,
     void* future_stream_cb_arg,
     const char* config,
     presage_t* result);
    
typedef void                 (__cdecl * PFUNC_presage_free)
(presage_t prsg);
    
typedef void                 (__cdecl * PFUNC_presage_free_string)
(char* str);

typedef void                 (__cdecl * PFUNC_presage_free_string_array)
(char** str);

typedef presage_error_code_t (__cdecl * PFUNC_presage_predict)
    (presage_t prsg,
     char*** result);
    
typedef presage_error_code_t (__cdecl * PFUNC_presage_completion)
    (presage_t prsg,
     const char* token,
     char** result);

typedef presage_error_code_t (__cdecl * PFUNC_presage_context)
    (presage_t prsg, 
     char** result);

typedef presage_error_code_t (__cdecl * PFUNC_presage_context_change)
    (presage_t prsg,
     int* result);

typedef presage_error_code_t (__cdecl * PFUNC_presage_prefix)
    (presage_t prsg,
     char** result);

typedef presage_error_code_t (__cdecl * PFUNC_presage_config)
    (presage_t prsg,
     const char* variable,
     char** result);

typedef presage_error_code_t (__cdecl * PFUNC_presage_config_set)
    (presage_t prsg,
     const char* variable,
     const char* value);
    
typedef presage_error_code_t (__cdecl * PFUNC_presage_save_config)
    (presage_t prsg);


bool LoadPresageDLL(void);
void UnloadPresageDLL(void);

#endif /* PRESAGE_UPLINK_H */
