using System;
using presage;

namespace presage_csharp_demo
{
    class presage_csharp_demo
    {
        public static string callback_get_past_stream()
        {
            return "some text to get st";
        }

        public static string callback_get_future_stream()
        {
            return "";
        }

        public static void Main()
        {
            Presage prsg = new Presage
            (
                callback_get_past_stream,
                callback_get_future_stream,
                "presage_csharp_demo.xml"
            );

            foreach (string str in prsg.predict())
            {
                System.Console.WriteLine(str);
            }

            System.Console.WriteLine(prsg.context());

            System.Console.WriteLine(prsg.prefix());

            System.Console.WriteLine(prsg.context_change());

            System.Console.WriteLine(prsg.completion("started"));

            System.Console.WriteLine(prsg.config("Presage.Selector.SUGGESTIONS"));

            prsg.config("Presage.Selector.SUGGESTIONS", "10");

            System.Console.WriteLine(prsg.config("Presage.Selector.SUGGESTIONS"));

            prsg.save_config();
        }
    }
}
