
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
using System.Windows.Forms;

namespace presage_wcf_service_system_tray
{
    class PresageSystemTray : IDisposable
    {
        private NotifyIcon icon;
        private AboutForm aboutForm;

        public PresageSystemTray()
        {
            icon = new NotifyIcon();

            ContextMenuStrip menu = new ContextMenuStrip();
            ToolStripMenuItem item;

            item = new ToolStripMenuItem();
            item.Text = "About";
            item.Click += new EventHandler(AboutClick);
            menu.Items.Add(item);

            item = new ToolStripMenuItem();
            item.Text = "Exit";
            item.Click += new EventHandler(ExitClick);
            menu.Items.Add(item);

            icon.ContextMenuStrip = menu;
        }

        public void Dispose()
        {
            icon.Dispose();
        }

        public void Display()
        {
            icon.Text = "Presage WCF service";
            icon.Icon = presage_wcf_service_system_tray.Properties.Resources.presage_ico;
            icon.Visible = true;
        }

        private void AboutClick(object sender, EventArgs e)
        {
            if (aboutForm == null)
            {
                aboutForm = new AboutForm();
            }
            aboutForm.ShowDialog();
        }

        private void ExitClick(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
