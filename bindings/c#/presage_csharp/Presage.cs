using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace presage_csharp
{
    public class Presage
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
        private static extern int presage_free(
            IntPtr presage
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private unsafe static extern int presage_free_string(
            IntPtr str
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private unsafe static extern int presage_free_string_array(
            IntPtr str_array
        );

        [DllImport("libpresage-1.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int presage_predict(
            IntPtr presage,
            out IntPtr result
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

        
        private IntPtr prsg;
        private callback_get_past_stream_t past_cb;
        private callback_get_future_stream_t future_cb;


        public delegate string callback_get_past_stream_t();
        public delegate string callback_get_future_stream_t();

        public Presage(
            callback_get_past_stream_t get_past_stream_cb,
            callback_get_future_stream_t get_future_stream_cb
            )
        {
            past_cb = get_past_stream_cb;
            future_cb = get_future_stream_cb;

            presage_new(past_cb, System.IntPtr.Zero, future_cb, System.IntPtr.Zero, out prsg);
        }

        public Presage(
            callback_get_past_stream_t get_past_stream_cb,
            callback_get_future_stream_t get_future_stream_cb,
            string config
            )
        {
            past_cb = get_past_stream_cb;
            future_cb = get_future_stream_cb;

            // call presage_new_with_config
            presage_new_with_config(past_cb, System.IntPtr.Zero, future_cb, System.IntPtr.Zero, config, out prsg);
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
            presage_predict(prsg, out prediction);

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

            presage_context(prsg, out str_ptr);

            result = Marshal.PtrToStringAnsi(str_ptr);

            return result;
        }

        public bool context_change()
        {
            bool result;

            IntPtr int_ptr;
            
            presage_context_change(prsg, out int_ptr);

            result = ! int_ptr.Equals(0);

            return result;
        }

        public string prefix()
        {
            string result;

            IntPtr str_ptr;

            presage_prefix(prsg, out str_ptr);

            result = Marshal.PtrToStringAnsi(str_ptr);

            return result;
        }

        public string completion(string token)
        {
            IntPtr str_ptr;

            presage_completion(prsg, token, out str_ptr);

            string result = Marshal.PtrToStringAnsi(str_ptr);

            return result;
        }

        public string config(string variable)
        {
            string result;

            IntPtr str_ptr;

            presage_config(prsg, variable, out str_ptr);

            result = Marshal.PtrToStringAnsi(str_ptr);

            return result;
        }

        public void config(string variable, string value)
        {
            presage_config_set(prsg, variable, value);
        }

        public void save_config()
        {
            presage_save_config(prsg);
        }
    }
}
