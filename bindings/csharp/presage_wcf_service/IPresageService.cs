using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace presage_wcf_service
{
    [ServiceContract]
    public interface IPresageService
    {
        [OperationContract]
        string[] predict(string previous_words, string current_word);

        [OperationContract]
        string context();

        [OperationContract]
        bool context_change();

        [OperationContract]
        string prefix();

        [OperationContract]
        void learn(string text);

        [OperationContract]
        string completion(string token);

        [OperationContract]
        string get_config(string variable);

        [OperationContract]
        void set_config(string variable, string value);

        [OperationContract]
        void save_config();
    }
}
