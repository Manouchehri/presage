
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2015  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

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


using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

using Microsoft.Win32;

namespace presage
{
    [Serializable()]
    public class PresageException : System.Exception
    {
        public PresageException() : base() { }
        public PresageException(string message) : base(message) { }
        public PresageException(string message, System.Exception inner) : base(message, inner) { }

        // A constructor is needed for serialization when an 
        // exception propagates from a remoting server to the client.  
        protected PresageException(System.Runtime.Serialization.SerializationInfo info,
            System.Runtime.Serialization.StreamingContext context) { }
    }

    public class Presage : IPresage
    {
        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_new(
            callback_get_past_stream_t cb_get_past_stream,
            IntPtr cb_get_past_stream_arg,
            callback_get_future_stream_t cb_get_future_stream,
            IntPtr cb_get_future_stream_arg,
            out IntPtr result
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_new_with_config(
            callback_get_past_stream_t cb_get_past_stream,
            IntPtr cb_get_past_stream_arg,
            callback_get_future_stream_t cb_get_future_stream,
            IntPtr cb_get_future_stream_arg,
            string config,
            out IntPtr result
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void presage_free(
            IntPtr presage
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private unsafe static extern void presage_free_string(
            IntPtr str
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private unsafe static extern void presage_free_string_array(
            IntPtr str_array
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_predict(
            IntPtr presage,
            out IntPtr result
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_learn(
            IntPtr presage,
            string token
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_completion(
            IntPtr presage,
            string token,
            out IntPtr result
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_context(
            IntPtr presage,
            out IntPtr result
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_context_change(
            IntPtr presage,
            out IntPtr context_change
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_prefix(
            IntPtr presage,
            out IntPtr result
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_config(
            IntPtr presage,
            string variable,
            out IntPtr result
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_config_set(
            IntPtr presage,
            string variable,
            string value
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_save_config(
            IntPtr presage
        );

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool SetDllDirectory(string lpPathName);
        

        public delegate string callback_get_past_stream_t();
        public delegate string callback_get_future_stream_t();

        private callback_get_past_stream_t get_past_stream_cb;
        private callback_get_future_stream_t get_future_stream_cb;

        private IntPtr prsg;

        public Presage(
            callback_get_past_stream_t get_past_stream_cb,
            callback_get_future_stream_t get_future_stream_cb
            )
        {
            // ensure that delegate lifetime persists as long as presage object
            this.get_past_stream_cb = get_past_stream_cb;
            this.get_future_stream_cb = get_future_stream_cb;

            set_presage_dll_directory();

            int rc = presage_new(
                get_past_stream_cb, System.IntPtr.Zero,
                get_future_stream_cb, System.IntPtr.Zero,
                out prsg);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_new() error code: {0}", rc));
            }
        }

        public Presage(
            callback_get_past_stream_t get_past_stream_cb,
            callback_get_future_stream_t get_future_stream_cb,
            string config
            )
        {
            // ensure that delegate lifetime persists as long as presage object
            this.get_past_stream_cb = get_past_stream_cb;
            this.get_future_stream_cb = get_future_stream_cb;

            set_presage_dll_directory();

            // call presage_new_with_config
            int rc = presage_new_with_config(
                get_past_stream_cb, System.IntPtr.Zero,
                get_future_stream_cb, System.IntPtr.Zero,
                config, out prsg);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_new_with_config() error code: {0}", rc));
            }
        }

        ~Presage()
        {
            presage_free(prsg);
        }

        public unsafe string[] predict()
        {
            List<string> result = new List<string>();

            IntPtr prediction;

            // call presage_predict
            int rc = presage_predict(prsg, out prediction);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_predict() error code: {0}", rc));
            }

            if (prediction != null)
            {
                char** ptr_to_ptr_to_char = (char**) prediction.ToPointer();

                for (int i = 0; ptr_to_ptr_to_char[i] != null; ++i)
                {
                    char* ptr_to_char = (char*) ptr_to_ptr_to_char[i];
                    IntPtr str_ptr = new IntPtr(ptr_to_char);
                    string str = Marshal.PtrToStringAnsi(str_ptr);
                    result.Add(str);
                }
 
            }

            // free prediction
            presage_free_string_array(prediction);

            return result.ToArray();
        }

        public string context()
        {
            string result;

            IntPtr str_ptr;

            int rc = presage_context(prsg, out str_ptr);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_context() error code: {0}", rc));
            }

            result = Marshal.PtrToStringAnsi(str_ptr);

            return result;
        }

        public bool context_change()
        {
            bool result;

            IntPtr int_ptr;
            
            int rc = presage_context_change(prsg, out int_ptr);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_context_change() error code: {0}", rc));
            }

            result = ! int_ptr.Equals(0);

            return result;
        }

        public string prefix()
        {
            string result;

            IntPtr str_ptr;

            int rc = presage_prefix(prsg, out str_ptr);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_prefix() error code: {0}", rc));
            }

            result = Marshal.PtrToStringAnsi(str_ptr);

            return result;
        }

        public void learn(string text)
        {
            int rc = presage_learn(prsg, text);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_learn() error code: {0}", rc));
            }
        }

        public string completion(string token)
        {
            IntPtr str_ptr;

            int rc = presage_completion(prsg, token, out str_ptr);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_completion() error code: {0}", rc));
            }

            string result = Marshal.PtrToStringAnsi(str_ptr);

            return result;
        }

        public string get_config(string variable)
        {
            string result;

            IntPtr str_ptr;

            int rc = presage_config(prsg, variable, out str_ptr);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_config() error code: {0}", rc));
            }

            result = Marshal.PtrToStringAnsi(str_ptr);

            return result;
        }

        public void set_config(string variable, string value)
        {
            int rc = presage_config_set(prsg, variable, value);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_config_set() error code: {0}", rc));
            }
        }

        public void save_config()
        {
            int rc = presage_save_config(prsg);
            if (rc != 0)
            {
                throw new PresageException(String.Format("presage_save_config() error code: {0}", rc));
            }

        }

        private string read_presage_registy_key()
        {
            string result = null;

            try
            {
                result = Registry.GetValue("HKEY_CURRENT_USER\\Software\\Presage", "", string.Empty).ToString();
            }
            catch (System.Security.SecurityException ex)
            {
                System.Console.WriteLine("Caught exception: {0}", ex.ToString());
                result = string.Empty;
            }
            catch (System.IO.IOException ex)
            {
                System.Console.WriteLine("Caught exception: {0}", ex.ToString()); 
                result = string.Empty;
            }

            return result;
        }

        private void set_presage_dll_directory()
        {
            string presage_root = read_presage_registy_key();
            if (presage_root != string.Empty)
            {
                string presage_bin_path = presage_root + "\\bin";
                SetDllDirectory(presage_bin_path);
            }
        }
    }
}
