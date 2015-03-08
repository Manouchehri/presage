
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

namespace presage_wcf_service
{
    [ServiceContract(Name = Constants.ServiceName, Namespace = Constants.ServiceNamespace)]
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
