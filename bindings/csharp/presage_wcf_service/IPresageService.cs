using System.ServiceModel;

namespace presage_wcf_service
{
    [ServiceContract]
    public interface IPresageService
    {
        [OperationContractAttribute]
        string[] predict(string previous_words, string current_word);

        [OperationContractAttribute]
        string context();

        [OperationContractAttribute]
        bool context_change();

        [OperationContractAttribute]
        string prefix();

        [OperationContractAttribute]
        void learn(string text);

        [OperationContractAttribute]
        string completion(string token);

        [OperationContractAttribute]
        string get_config(string variable);

        [OperationContractAttribute]
        void set_config(string variable, string value);

        [OperationContractAttribute]
        void save_config();
    }
}
