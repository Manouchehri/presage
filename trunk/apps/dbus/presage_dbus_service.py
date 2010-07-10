#!/usr/bin/python

##########
#  Presage, an extensible predictive text entry system
#  ------------------------------------------------------
#  Copyright (C) 2010  David Pellicer <dpellicer@warp.es>
#  Copyright (C) 2010  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


import gobject
from dbus.service import BusName, Object, method
from dbus import SessionBus, UTF8String
from dbus.mainloop.glib import DBusGMainLoop
import presage
import os

service_name = 'org.gnome.presage.beta'
service_path = '/org/gnome/presage/beta'
default_config_file = 'presage.xml'
pid = None

class PredictionCallback(presage.PresageCallback):
    def __init__(self):
        presage.PresageCallback.__init__(self)
        self.buffer = ''
        self.cursor_position = 0

    def set_buffer(self, string):
        self.buffer = string
        self.set_cursor_position(len(string))

    def set_cursor_position(self, cursor_position):
        self.cursor_position = cursor_position

    def get_past_stream(self):
        return self.buffer
    
    def get_future_stream(self):
        return self.buffer[self.cursor_position:]

class Predictions:
    def __init__(self, config):
        self.callback = PredictionCallback().__disown__()
        self.prsg = presage.Presage(self.callback, config)
        
    def get_prediction(self):
        return self.prsg.predict()

    def reset_buffer(self):
        self.callback.set_buffer('')
        self.callback.set_cursor_position(0)

    def set_buffer(self, string):
        self.callback.set_buffer(string)


dbus_loop = DBusGMainLoop()
session = SessionBus(mainloop=dbus_loop)

class PresageObject(Object):
    instances = 0
    def __init__(self, bus, loop, config):
        PresageObject.instances += 1
        name = BusName(service_name, bus)
        self.path = service_path + '/' + str(PresageObject.instances)
        self.loop = loop
        self.predictions = Predictions(config)
        Object.__init__(self, name, self.path)
        print 'Service %s created' % self.path

    @method(dbus_interface = service_name, out_signature = 'as')
    def get_predictions(self):
        return self.predictions.get_prediction()

    @method(dbus_interface = service_name, utf8_strings=True)
    def set_buffer(self, string):
        self.predictions.set_buffer(str(string))

    @method(dbus_interface = service_name)
    def close(self):
        self.loop.quit()
        print 'Service %s closed' % self.path


class PresageService(Object):
    def __init__(self, bus, loop):
        name = BusName(service_name, bus)
        self.bus = bus
        path = service_path
        self.loop = loop
        Object.__init__(self, name, path)
        print 'Service %s created' % service_name

    @method(dbus_interface = service_name, in_signature = 's', out_signature = 's')
    def get_next_object_path(self, config = default_config_file):
        obj = PresageObject(self.bus, self.loop, config)
        return obj.path

        
def main():
    loop = gobject.MainLoop()
    my_service = PresageService(session, loop)
    pid = os.getpid()
    print pid
    loop.run()
