
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
using System.ServiceModel;
using presage;

namespace presage_wcf_service
{
    [ServiceBehavior(Namespace = Constants.ServiceNamespace)]
    public class PresageService : IPresageService
    {
        private Presage prsg;

        private string past_buffer;
        private string future_buffer;

        private string get_past_stream()
        {
            return past_buffer;
        }

        private string get_future_stream()
        {
            return future_buffer;
        }

        public PresageService()
        {
            past_buffer = "";
            future_buffer = "";
            prsg = new Presage(get_past_stream, get_future_stream);

            System.Console.WriteLine("New PresageService instance created");
        }

        ~PresageService()
        {
            System.Console.WriteLine("PresageService instance destroyed");
        }

        public string[] predict(string previous_words, string current_word)
        {
            past_buffer = previous_words + " " + current_word;

            return predict();
        }


        public string[] predict()
        {
            return prsg.predict();
        }

        public string context()
        {
            return prsg.context();
        }

        public bool context_change()
        {
            return prsg.context_change();
        }

        public string prefix()
        {
            return prsg.prefix();
        }

        public void learn(string text)
        {
            prsg.learn(text);
        }

        public string completion(string token)
        {
            return prsg.completion(token);
        }

        public string get_config(string variable)
        {
            return prsg.get_config(variable);
        }

        public void set_config(string variable, string value)
        {
            prsg.set_config(variable, value);
        }

        public void save_config()
        {
            prsg.save_config();
        }
    }
}
