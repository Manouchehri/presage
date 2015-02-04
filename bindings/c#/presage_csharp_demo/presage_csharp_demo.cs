using System;
using presage;

namespace presage_csharp_demo
{
    class presage_csharp_demo
    {
        private string buffer = "some text to get st";
        
        private string callback_get_past_stream()
        {
            return buffer;
        }

        private string callback_get_future_stream()
        {
            return "";
        }

        public static void Main()
        {
            presage_csharp_demo demo = new presage_csharp_demo();

            try
            {
                Presage prsg = new Presage
                   (
                        demo.callback_get_past_stream,
                        demo.callback_get_future_stream,
                        "presage_csharp_demo.xml"
                    );

                System.Console.WriteLine("predict: ");
                foreach (string str in prsg.predict())
                {
                    System.Console.WriteLine("    " + str);
                }

                System.Console.WriteLine("context: " + prsg.context());

                System.Console.WriteLine("prefix: " + prsg.prefix());

                System.Console.WriteLine("context_change: " + prsg.context_change());

                System.Console.WriteLine("completion: " + prsg.completion("started"));

                System.Console.WriteLine("suggestions: " + prsg.config("Presage.Selector.SUGGESTIONS"));

                prsg.config("Presage.Selector.SUGGESTIONS", "10");

                System.Console.WriteLine("suggestions: " + prsg.config("Presage.Selector.SUGGESTIONS"));

                prsg.save_config();
            }
            catch (PresageException ex)
            {
                System.Console.WriteLine("Caught presage exception: {0}", ex.ToString());
            }
        }
    }
}
