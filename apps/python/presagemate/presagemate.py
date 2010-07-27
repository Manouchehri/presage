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
import atexit
import Xlib
import Xlib.display

def process_event(event):
   global char_index 
   global prediction
   global ignore_keys

   #print "event_string: " + event.event_string

   if (event.type == pyatspi.KEY_PRESS) & (event.is_text == True):
    
     if (len(event.event_string) == 1) & (event.event_string.isalpha() == True):
  
        if ignore_keys == 0:
           char_index += 1
           callback.buffer += event.event_string
           prediction = prsg.predict()
        else:
           ignore_keys -= 1
     
     elif event.event_string.lower() == "space":
        char_index = 0
        if ignore_keys == 0:
           callback.buffer += ' '
           prediction = prsg.predict()
        else:
           ignore_keys -= 1
     
     elif event.event_string.lower() == "backspace":
        char_index = 0
        if ignore_keys == 0:
           callback.buffer = callback.buffer[:-1]  # cut out the last character
           prediction = prsg.predict()
        else:
           ignore_keys -= 1
     
     elif event.event_string.lower() == "return":
        char_index = 0
        if ignore_keys == 0:
           callback.buffer = '\n'
           prediction = prsg.predict()
        else:
           ignore_keys -= 1
     
     elif event.event_string[0] == "F":
        # -25 is the function key offset, I use F25-34 remapped to the F1-10 keys
        f_key = int(event.event_string[1:]) - 25
       
        if (f_key >= 0) & (len(prediction) > f_key):
        
           predicted_word = prediction[f_key]

           if predicted_word == "i":
              predicted_word = predicted_word.upper()
           
           completion = prsg.completion(predicted_word)
           
           print 'Prediction: ' + predicted_word
           print 'Completion: ' + completion
           
           for ch in completion:
              keyval = gtk.gdk.unicode_to_keyval(ord(ch))
              reg.generateKeyboardEvent(keyval, None, pyatspi.KEY_SYM)
           
           callback.buffer += completion
           
           prediction = prsg.predict()
           
           # theres no way to consume keypresses so we ignore the keypress we have added
           ignore_keys = len(completion)
           char_index = 0
        
     if ignore_keys == 0:
        update_gui(prediction)
 
 
def update_gui(prediction):
   prediction_string = str()
   for index, word in enumerate(prediction):
      if word == "i":
         word = word.upper()
      pred = 'F' + str(index + 1) + '  ',  word + '\n'
      prediction_string += ''.join(pred)
   
   "Set label text omitting the last \n"
   label.set_text(prediction_string[0:(len(prediction_string) - 1)])

def quit_app(widget):
   remap_keys(False)
  
   position = window.get_position()
   set_position_config(position[0], position[1])
   gtk.main_quit()
   exit()
   return gtk.FALSE

def delete_event(widget, event, data=None):
   quit_app(widget)

def frame_event(window, event, data=None):
   state = event.new_window_state
   if state & gtk.gdk.WINDOW_STATE_ICONIFIED:
      # re-map F1-10 to F1-10 when the window is iconified
      remap_keys(False)
   else:
      # re-map F1-10 to F25-34 when the window is uniconified
      remap_keys(True)

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

def remap_keys(remap):
   """ Remap keycode -> keysym for function keys
   
   The keycode -> keysym mapping is so that the keycode generated
   when the F1 key is pressed sends the F25 keysym. But to do that you
   need to know the keycodes for F1-10, for Linux on x86 the keycode for
   F1 is 67 but for other platforms it could be different. So the code
   looks up the keycode mapped to F1 keysym if that is non zero (i.e. F1
   is mapped) I map that keycode to F1 or F25 as required, if F1 is not
   mapped I lookup the keycode for F25 and if that is non zero I map
   that keycode to F1 or F25 as required. If F1 and F25 are not mapped
   to any keycode I use 67 as the keycode for the F1 key. The code
   assumes that the keycodes for F1-10 are sequential which may not be
   true for all platforms. A more robust solution may be found by
   accessing the keyboard layout somehow but I haven't had time to look
   yet.
   """
   
   display = Xlib.display.Display()
  
   # only keycodes 8 to 255 are defined
   first_keycode = 8
   keymaps_orig = display.get_keyboard_mapping(first_keycode, 255 - first_keycode)
   keymaps = list(keymaps_orig)
   
   if remap == True: 
      keysym = 65494
   else:
      keysym = 65470
   
   keycode_F1 = display.keysym_to_keycode(65470)
   keycode_F25 = display.keysym_to_keycode(65494)
  
   # set first_fkeycode to a sensible default incase F1 and F25 are not mapped
   first_fkeycode = 67
  
   if keycode_F1 > 0:
      first_fkeycode = keycode_F1
   elif keycode_F25 > 0:
      first_fkeycode = keycode_F25
   
   for i in range(0, int(number_of_suggestions)):

      keymaps[(i + first_fkeycode - first_keycode)][0] = keysym + i
      keymaps[(i + first_fkeycode - first_keycode)][2] = keysym + i
      keymaps[(i + first_fkeycode - first_keycode)][4] = keysym + i
   
   display.change_keyboard_mapping(first_keycode, keymaps, onerror = None)
   display.sync()
   display.close()

def popup_menu(widget, event):
   if event.button == 3:
      menu.popup(None, None, None, event.button, event.time) 
      return True
   return False

def apply_preferences(widget):
   print "apply"
  
def close_preferences(widget):
   print "close"

def update_no_selections(widget):
   global number_of_suggestions
  
   number_of_suggestions = int(widget.get_value())
   prsg.config("Presage.Selector.SUGGESTIONS", str(number_of_suggestions))
   remap_keys(True)

def preferences(widget):
   preferences = gtk.Window(gtk.WINDOW_TOPLEVEL)
   preferences.set_title("pypresagemate preferences")
   preferences.set_type_hint(gtk.gdk.WINDOW_TYPE_HINT_DIALOG)
  
   preferences_box = gtk.VBox()
   
   notebook = gtk.Notebook()
   notebook.set_tab_pos(gtk.POS_TOP)
   
   appearance_frame = gtk.Frame()
   appearance_tab = gtk.Label("Appearance")
   notebook.append_page(appearance_frame, appearance_tab)
   
   appearance_placeholder = gtk.Label("\n\nconfiguration\nplaceholder\n\n")
   appearance_frame.add(appearance_placeholder)
   
   presage_frame = gtk.Frame()
   presage_tab = gtk.Label("Presage")
   notebook.append_page(presage_frame, presage_tab)
   
   presage_placeholder = gtk.Label("\n\nPresage library configuration\n\n")
   adj = gtk.Adjustment(float(number_of_suggestions), 3, 10, 1, 0, 0)
   no_of_selections = gtk.SpinButton(adj, 0, 0)
   no_of_selections.connect("output", update_no_selections)
   presage_frame.add(no_of_selections)
   
   preferences_box.add(notebook)
   
   preferences_buttons = gtk.HButtonBox()
   
   apply_button = gtk.Button("Apply")
   apply_button.connect("clicked", apply_preferences)
   
   close_button = gtk.Button("Close")
   close_button.connect("clicked", close_preferences)
   
   preferences_buttons.add(apply_button) 
   preferences_buttons.add(close_button)
   preferences_box.add(preferences_buttons)
   
   preferences.add(preferences_box)
   preferences.show_all()

def about(widget):
   comments = '''pypresagemate is a universal predictive text companion.
pypresagemate is powered by presage, an intelligent predictive text entry system.
'''
   license = '''This program is free software; you can redistribute it and/or modify
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
'''
   
   about = gtk.AboutDialog()
   about.set_program_name('pypresagemate')
   about.set_version("0.1")
   about.set_copyright("(c) Matteo Vescovi\n(c) John Hills")
   about.set_website("http://presage.sourceforge.net/")
   about.set_comments(comments)
   if os.path.isfile("/usr/local/share/presage/presage.png"):
      about.set_logo(gtk.gdk.pixbuf_new_from_file("/usr/local/share/presage/presage.png"))
   about.run()
   about.destroy()




config = get_config()
pangofont = config.get('Config', 'pangofont')
window_position_x = config.getint('Config', 'window_position_x')
window_position_y = config.getint('Config', 'window_position_y')

ignore_keys = 0     
char_index = 0
   
reg = pyatspi.Registry

# Define and create PresageCallback object
class SimpleCallback(presage.PresageCallback):
   def __init__(self):
      presage.PresageCallback.__init__(self)
      self.buffer = ''
   
   def get_past_stream(self):
      return self.buffer
   
   def get_future_stream(self):
      return ''

# Presage owns callback, so we create it and disown it
callback = SimpleCallback().__disown__()

prsg = presage.Presage(callback)

number_of_suggestions = prsg.config("Presage.Selector.SUGGESTIONS")
"limit suggestions to 10 as that should be enough for anybody :-)"
if int(number_of_suggestions) > int(10):
  number_of_suggestions = 10
  prsg.config("Presage.Selector.SUGGESTIONS", "10")

prediction = prsg.predict()

reg.registerKeystrokeListener(process_event, mask=pyatspi.allModifiers())

window = gtk.Window(gtk.WINDOW_TOPLEVEL)
window.connect("delete-event", delete_event)
window.connect("window-state-event", frame_event)

window.set_keep_above(True)
#window.set_decorated(False)

window.move(window_position_x, window_position_y)

window.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("#c0c0ff"))

label = gtk.Label("pypresagemate")
label.set_justify(gtk.JUSTIFY_LEFT)
label.set_width_chars(20)

font_desc = pango.FontDescription(pangofont)
label.modify_font(font_desc)

window.connect("button-press-event", popup_menu)
window.add_events(gtk.gdk.BUTTON_PRESS_MASK)

menu = gtk.Menu()
menu_item1 = gtk.MenuItem('About')
menu_item2 = gtk.MenuItem('Preferences')
menu_item3 = gtk.MenuItem('Quit')
menu.append(menu_item1)
menu.append(menu_item2)
menu.append(menu_item3)

menu_item1.connect("activate", about)
menu_item2.connect("activate", preferences)
menu_item3.connect("activate", quit_app)

menu.show_all()

update_gui(prediction)
window.add(label)
window.show_all()

# remap keys if program closes other than by closing gtk
atexit.register(remap_keys, False)

gtk.main()
reg.start()
