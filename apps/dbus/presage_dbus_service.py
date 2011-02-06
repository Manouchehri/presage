#!/usr/bin/env python

##########
#  Presage, an extensible predictive text entry system
#  ------------------------------------------------------
#
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

import dbus
import dbus.service
import dbus.mainloop.glib

import os
import sys

import presage

presage_service_name      = 'org.gnome.presage.beta'
presage_service_path      = '/org/gnome/presage/beta'
presage_service_interface = 'org.gnome.presage.beta'

default_config_file = 'presage.xml'


class DbusPresageCallback(presage.PresageCallback):
    def __init__(self):
        presage.PresageCallback.__init__(self)
        self.past_stream = ''
        self.future_stream = ''

    def set_past_stream(self, string):
        self.past_stream = string
        #print 'set_past_stream: ' + self.past_stream

    def set_future_stream(self, string):
        self.future_stream = string
        #print 'set_future_stream: ' + self.future_stream

    def get_past_stream(self):
        #print 'get_past_stream: ' + self.past_stream
        return str(self.past_stream)
    
    def get_future_stream(self):
        #print 'get_future_stream: ' + self.future_stream
        return str(self.future_stream)


class PresageObject(dbus.service.Object):
    instances = 0
    def __init__(self, bus, config):
        PresageObject.instances += 1
        name = dbus.service.BusName(presage_service_name, bus)
        self.path = presage_service_path + '/' + str(PresageObject.instances)
        self.callback = DbusPresageCallback().__disown__()
        self.prsg = presage.Presage(self.callback, config)
        dbus.service.Object.__init__(self, name, self.path)
        print 'Created presage object %s' % self.path

#    def __del__( self ):
#        print 'PresageObject destroyed: ', self 

    @dbus.service.method(dbus_interface = presage_service_interface,
            in_signature = 'ss',
            out_signature = 'as')
    def get_prediction(self, past, future):
        self.callback.set_past_stream(past)
        self.callback.set_future_stream(future)
        return self.prsg.predict()

    @dbus.service.method(dbus_interface = presage_service_interface)
    def destroy(self):
        self.remove_from_connection()
        print 'Removed presage object ' + self.path


class PresageService(dbus.service.Object):
    def __init__(self, bus, loop):
        self.name = dbus.service.BusName(presage_service_name, bus)
        self.bus = bus
        self.path = presage_service_path
        self.loop = loop
        dbus.service.Object.__init__(self, self.name, self.path)
        print 'Service %s created, pid %d' % (presage_service_name, os.getpid())

    @dbus.service.method(dbus_interface = presage_service_interface,
                         in_signature = 's',
                         out_signature = 's')
    def get_presage_object_path(self, config = default_config_file):
        obj = PresageObject(self.bus, config)
        return obj.path

    @dbus.service.method(dbus_interface = presage_service_interface,
                         in_signature = '',
                         out_signature = '')
    def shutdown(self):
        self.loop.quit()
        print 'Service %s shutdown' % presage_service_name


def start():
    print 'Starting ' + presage_service_name + '...'

    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
    loop = gobject.MainLoop()
    presage_service = PresageService(dbus.SessionBus(), loop)
    loop.run()

def stop():
    print 'Stopping ' + presage_service_name + '...'
    try:
        dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
        bus = dbus.SessionBus()
        presage_service_object = bus.get_object(presage_service_name, presage_service_path)
        presage_service_object.shutdown()

    except Exception, e:
        print 'Caught exception while attempting to stop ' + presage_service_name
        print e
        
if __name__ == '__main__':
    start()
