
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
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.ServiceModel;
using System.ServiceModel.Description;
using presage_wcf_service;

namespace presage_wcf_service_system_tray
{
    public partial class PresageWCFHostForm : Form, IDisposable
    {
        private ServiceHost host;

        public PresageWCFHostForm()
        {
            InitializeComponent();
            
            Uri baseAddress = new Uri(Constants.ServiceBaseAddress);

            // Create the ServiceHost.
            host = new ServiceHost(typeof(PresageService), baseAddress);

            NetNamedPipeBinding binding = new NetNamedPipeBinding();
            binding.Namespace = presage_wcf_service.Constants.ServiceNamespace;

            // Add presage endpoint.
            host.AddServiceEndpoint(
                typeof(IPresageService),
                binding, 
                Constants.ServicePresageEndpointRelativeAddress);

            // Enable metadata publishing.
            // Check to see if the service host already has a ServiceMetadataBehavior
            ServiceMetadataBehavior smb = host.Description.Behaviors.Find<ServiceMetadataBehavior>();
            // If not, add one
            if (smb == null)
                smb = new ServiceMetadataBehavior();
            smb.MetadataExporter.PolicyVersion = PolicyVersion.Policy15;
            host.Description.Behaviors.Add(smb);

            // Add metadata endpoint.
            host.AddServiceEndpoint(
                typeof(IMetadataExchange), 
                MetadataExchangeBindings.CreateMexNamedPipeBinding(),
                Constants.ServiceMexEndpointRelativeAddress);

            // Open the ServiceHost to start listening for messages. Since
            // no endpoints are explicitly configured, the runtime will create
            // one endpoint per base address for each service contract implemented
            // by the service.
            try
            {
                host.Open();
            }
            catch (Exception e)
            {
                MessageBox.Show(
                    "Error occurred while attempting to start Presage WCF Service:\n\n" + e.Message,
                    "Presage WCF Service",
                    MessageBoxButtons.OK);
                // Exit with 
                // ERROR_PIPE_BUSY
                // 231 (0xE7)
                // All pipe instances are busy.
                // 
                System.Environment.Exit(231);
            }
        }

        new public void Dispose()
        {
            host.Close();
            base.Dispose();
        }
    }
}
