namespace presage
{
    public interface IPresage
    {
        string[] predict();

        string context();

        bool context_change();

        string prefix();

        void learn(string text);

        string completion(string token);

        string get_config(string variable);

        void set_config(string variable, string value);

        void save_config();
    }
}
