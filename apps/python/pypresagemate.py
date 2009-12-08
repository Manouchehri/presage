#!/usr/bin/python

##########
#  Presage, an extensible predictive text entry system
#  ------------------------------------------------------
#  Copyright (C) 2009  John Hills 
#  Copyright (C) 2009  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
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

import string 

try:
   import presage
except ImportError, ex:
   print '''
Error: failed to import module presage.

Check that presage python binding is properly installed (if
installed in a non-standard location, please set PYTHONPATH
accordingly).

Check that presage library is properly installed (if installed in a
non-standard location, please set LD_LIBRARY_PATH (PATH, LIBPATH)
accordingly).
'''
try:
    import pyatspi
except ImportError, ex:
   print '''
Error: failed to import module pyatspi.

Check that pyatspi python binding is properly installed (if
installed in a non-standard location, please set PYTHONPATH
accordingly).

Check that pyatspi library is properly installed (if installed in a
non-standard location, please set LD_LIBRARY_PATH (PATH, LIBPATH)
accordingly).
'''
   
try:
    import gtk
except ImportError, ex:
   print '''
Error: failed to import module gtk.

Check that gtk python binding is properly installed (if
installed in a non-standard location, please set PYTHONPATH
accordingly).

Check that gtk library is properly installed (if installed in a
non-standard location, please set LD_LIBRARY_PATH (PATH, LIBPATH)
accordingly).
'''


import ConfigParser
import os
import pango


def process_event(event):
  global char_index 
  global reg
  global prsg
  global prediction
  global ignore_keys
  
  if (event.type == pyatspi.KEY_PRESS) & (event.is_text == True):
    #print event
    
    if (len(event.event_string) == 1) & (event.event_string.isalpha() == True):
  
      if ignore_keys == 0:
        char_index += 1
        prediction = prsg.predict(event.event_string)
      else:
        ignore_keys -= 1
	
    elif event.event_string.lower() == "space":
      char_index = 0
      if ignore_keys == 0:
        prediction = prsg.predict(' ')
      else:
        ignore_keys -= 1
	
    elif event.event_string.lower() == "return":
      char_index = 0
      if ignore_keys == 0:
        prediction = prsg.predict('\n')
      else:
        ignore_keys -= 1
  
    elif event.event_string[0] == "F":
      # -13 is the function key offset I use F13-22 (remapped to the F1-10 keys)
      f_key = int(event.event_string[1:]) - 13
         
      if len(prediction) > f_key:
        
        predicted_word = prediction[f_key]
                
        for ch in predicted_word[char_index:] + ' ':
          keyval = gtk.gdk.unicode_to_keyval(ord(ch))
          reg.generateKeyboardEvent(keyval, None, pyatspi.KEY_SYM)

        prsg.complete(prediction[f_key])
       
        prediction = prsg.predict(' ')
        
        # theres no way to consume keypresses so we ignore the keypress we have added
        ignore_keys = len(predicted_word[char_index:] + ' ')
        char_index = 0
        
    if ignore_keys == 0:
      update_gui(prediction)
 
 
def update_gui(prediction):
  global label
  prediction_string = str()
  for index, word in enumerate(prediction):
    pred = 'F' + str(index + 1) + '  ',  word + '\n'
    prediction_string += ''.join(pred)
      
  label.set_text(prediction_string)


def delete_event(widget, event, data=None):
  position = window.get_position()
  set_position_config(position[0], position[1])
  gtk.main_quit()
  exit()
  return gtk.FALSE

def get_config():

  writeconfig_flag = False
  config = ConfigParser.SafeConfigParser()
  config.read(os.path.expanduser('~/.pypresagematerc'))

  if config.has_section('Config') == False:
    config.add_section('Config')
  
  try:
    pangofont = config.get('Config', 'pangofont')
  except:
    config.set('Config', 'pangofont', 'monospace 14')
    writeconfig_flag = True

  try:
    window_position_x = config.getint('Config', 'window_position_x')
  except:
    config.set('Config', 'window_position_x', '0')
    writeconfig_flag = True

  try:    
    window_position_y = config.getint('Config', 'window_position_y')
  except:
    config.set('Config', 'window_position_y', '0')
    writeconfig_flag = True

  if writeconfig_flag == True:
    configfile = open(os.path.expanduser('~/.pypresagematerc'), 'wb')
    try:
      config.write(configfile)
    finally:
      configfile.close()

    #with open(os.path.expanduser('~/.pypresagematerc'), 'wb') as configfile:
    #  config.write(configfile)

  return config
  
def set_position_config(x, y):
  config = ConfigParser.SafeConfigParser()
  config.read(os.path.expanduser('~/.pypresagematerc'))
  config.set('Config', 'window_position_x', str(x))
  config.set('Config', 'window_position_y', str(y))
  configfile = open(os.path.expanduser('~/.pypresagematerc'), 'wb')
  try:
    config.write(configfile)
  finally:
    configfile.close()

  #with open(os.path.expanduser('~/.pypresagematerc'), 'wb') as configfile:
  #  config.write(configfile)

  
config = get_config()
pangofont = config.get('Config', 'pangofont')
window_position_x = config.getint('Config', 'window_position_x')
window_position_y = config.getint('Config', 'window_position_y')

ignore_keys = 0     
char_index = 0
   
reg = pyatspi.Registry

prsg = presage.Presage()
prediction = prsg.predict('')

reg.registerKeystrokeListener(process_event, mask=pyatspi.allModifiers())

window = gtk.Window(gtk.WINDOW_TOPLEVEL)
window.connect("delete_event", delete_event)

window.set_keep_above(True)
#window.set_decorated(False)

window.move(window_position_x, window_position_y)

window.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("#c0c0ff"))

label = gtk.Label("pypresagemate")
label.set_justify(gtk.JUSTIFY_LEFT)
label.set_width_chars(20)

font_desc = pango.FontDescription(pangofont)
label.modify_font(font_desc)

update_gui(prediction)
window.add(label)
window.show_all()
gtk.main()
reg.start()
