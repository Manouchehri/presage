using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using presage;

namespace presage_wcf_service
{
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

            System.Console.WriteLine("New instance created");
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
