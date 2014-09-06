##########
#  Presage, an extensible predictive text entry system
#  ------------------------------------------------------
#
#  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
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

import sys

try:
   import wx
   import wx.stc
except ImportError, ex:
   print '''
Error: failed to import module wxPython.

wxPython is a Python binding for the wxWidgets toolkit.

Check that wxPython is properly installed.
'''
   print ex
   sys.exit(1)

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
   print ex
   sys.exit(1)

##########
# Prompter
#
class Prompter(wx.App):
   def __init__(self, version, config=None, suggestions=None, redirect=False):
      self.version = version # do this first, wx.App.__init__() calls OnInit()
      self.config = config
      self.suggestions = suggestions
      wx.App.__init__(self, redirect)

   def OnInit(self):
      self.frame = PrompterFrame(parent=None, id=-1, title='pyprompter', version=self.version, config=self.config, suggestions=self.suggestions)
      self.SetTopWindow(self.frame)
      return True

###############
# PrompterFrame
#
class PrompterFrame(wx.Frame):
   wildcard = "Text files (*.txt)|*.txt|"     \
       "All files (*.*)|*.*"

   def __init__(self, parent, id, title, version, config, suggestions):
      wx.Frame.__init__(self, parent, id, title)
      self.version = version

      self.editor = PrompterEditor(self, config, suggestions)

      self.MakeMenuBar()
      self.MakeToolBar()
      
      self.sizer = wx.BoxSizer(wx.HORIZONTAL)
      self.sizer.Add(self.editor, 1, wx.EXPAND)
      self.SetSizer(self.sizer)
      self.SetAutoLayout(True)
      self.Show()

   def MakeMenuBar(self):
      def BindMenu(item, handler):
         self.Bind(wx.EVT_MENU, handler, item)

      # file menu
      self.fileMenu = wx.Menu()
      BindMenu(self.fileMenu.Append(wx.ID_NEW, "&New\tCTRL+N"), self.OnFileMenuNew)
      BindMenu(self.fileMenu.Append(wx.ID_OPEN, "&Open\tCTRL+O"), self.OnFileMenuOpen)
      self.fileMenu.AppendSeparator()
      BindMenu(self.fileMenu.Append(wx.ID_SAVE, "&Save\tCTRL+S"), self.OnFileMenuSave)
      BindMenu(self.fileMenu.Append(wx.ID_SAVEAS, "Save &As\tSHIFT+CTRL+S"), self.OnFileMenuSaveAs)
      self.fileMenu.AppendSeparator()
      BindMenu(self.fileMenu.Append(wx.ID_CLOSE, "&Close\tCTRL+W"), self.OnFileMenuClose)
      BindMenu(self.fileMenu.Append(wx.ID_EXIT, "&Quit\tCTRL+Q"), self.OnFileMenuQuit)

      # edit menu
      self.editMenu = wx.Menu()
      BindMenu(self.editMenu.Append(wx.ID_UNDO, "&Undo\tCTRL+Z"), self.OnEditMenuUndo)
      BindMenu(self.editMenu.Append(wx.ID_REDO, "&Redo\tSHIFT+CTRL+Z"), self.OnEditMenuRedo)
      self.editMenu.AppendSeparator()
      BindMenu(self.editMenu.Append(wx.ID_CUT, "Cu&t\tCTRL+X"), self.OnEditMenuCut)
      BindMenu(self.editMenu.Append(wx.ID_COPY, "&Copy\tCTRL+C"), self.OnEditMenuCopy)
      BindMenu(self.editMenu.Append(wx.ID_PASTE, "&Paste\tCTRL+V"), self.OnEditMenuPaste)
      self.editMenu.AppendSeparator()
      BindMenu(self.editMenu.Append(wx.ID_SELECTALL, "Select &All\tCTRL+A"), self.OnEditMenuSelectAll)

      
      # view menu
      self.viewMenu = wx.Menu()
      self.ID_TOGGLE_WORD_WRAP = wx.NewId()
      # need to save wxMenuItem object returned by Append() to test if checked or not
      self.word_wrap = self.viewMenu.Append(self.ID_TOGGLE_WORD_WRAP,
                                            "&Word wrap",
                                            "Toggle word wrap mode",
                                            wx.ITEM_CHECK)
      BindMenu(self.word_wrap, self.OnViewMenuWordWrap)
      # turn text_wrap checked menu item on at start-up
      self.viewMenu.Check(self.ID_TOGGLE_WORD_WRAP, True)
      self.viewMenu.AppendSeparator()
      BindMenu(self.viewMenu.Append(wx.ID_ZOOM_IN, "L&arger text\tCTRL++"), self.OnViewMenuLargerText)
      BindMenu(self.viewMenu.Append(wx.ID_ZOOM_OUT, "S&maller text\tCTRL+-"), self.OnViewMenuSmallerText)
      self.viewMenu.AppendSeparator()      
      self.ID_SHOW_TOOLBAR = wx.NewId()
      self.show_toolbar_view_menu_item = self.viewMenu.Append(self.ID_SHOW_TOOLBAR,
                                                              "Show &Toolbar",
                                                              "Show Toolbar",
                                                              wx.ITEM_CHECK)
      BindMenu(self.show_toolbar_view_menu_item, self.OnViewMenuShowToolbar)
      self.viewMenu.Check(self.ID_SHOW_TOOLBAR, True)

      # presage menu
      self.presageMenu = wx.Menu()
      self.ID_PROMPT_ME = wx.NewId()
      BindMenu(self.presageMenu.Append(self.ID_PROMPT_ME, "&Prompt me\tCTRL+P"), self.OnPresageMenuPromptMe)

      self.ID_TOGGLE_FUNCTION_KEYS_MODE = wx.NewId()
      self.function_mode_presage_menu_item = self.presageMenu.Append(self.ID_TOGGLE_FUNCTION_KEYS_MODE,
                                                                     "&Function keys\tCTRL+F",
                                                                     "Toggle function keys mode",
                                                                     wx.ITEM_CHECK)
      self.presageMenu.Check(self.ID_TOGGLE_FUNCTION_KEYS_MODE, True)
      BindMenu(self.function_mode_presage_menu_item, self.OnPresageMenuToggleFunctionMode)

      self.ID_TOGGLE_AUTOPUNCTUATION_MODE = wx.NewId()
      self.autopunctuation_mode_presage_menu_item = self.presageMenu.Append(self.ID_TOGGLE_AUTOPUNCTUATION_MODE,
                                                                            "&Autopunctuation\tCTRL+A",
                                                                            "Toggle autopunctuation mode",
                                                                            wx.ITEM_CHECK)
      self.presageMenu.Check(self.ID_TOGGLE_AUTOPUNCTUATION_MODE, True)
      BindMenu(self.autopunctuation_mode_presage_menu_item, self.OnPresageMenuToggleAutopunctuationMode)

      # help menu
      self.helpMenu = wx.Menu()
      BindMenu(self.helpMenu.Append(wx.ID_HELP, "&Contents"), self.OnHelpMenuContents)
      self.helpMenu.AppendSeparator()
      BindMenu(self.helpMenu.Append(wx.ID_ABOUT, "&About"), self.OnHelpMenuAbout)

      # menu bar
      self.menuBar = wx.MenuBar()
      self.menuBar.Append(self.fileMenu, "&File")
      self.menuBar.Append(self.editMenu, "&Edit")
      self.menuBar.Append(self.viewMenu, "&View")
      self.menuBar.Append(self.presageMenu, "&Presage")
      self.menuBar.Append(self.helpMenu, "&Help")
      self.SetMenuBar(self.menuBar)

      # grey out menu items
      self.fileMenu.Enable(wx.ID_SAVE, False)
      self.fileMenu.Enable(wx.ID_SAVEAS, False)

      self.editMenu.Enable(wx.ID_UNDO, False)
      self.editMenu.Enable(wx.ID_REDO, False)

   def MakeToolBar(self):
      def BindTool(item, handler):
         self.Bind(wx.EVT_TOOL, handler, item)
         self.Bind(wx.EVT_TOOL_RCLICKED, handler, item)

      self.toolbar = self.CreateToolBar(wx.TB_HORIZONTAL
                                        | wx.NO_BORDER
                                        | wx.TB_FLAT)

      tsize = (16,16)
      new_bmp =  wx.ArtProvider.GetBitmap(wx.ART_NEW, wx.ART_TOOLBAR, tsize)
      open_bmp = wx.ArtProvider.GetBitmap(wx.ART_FILE_OPEN, wx.ART_TOOLBAR, tsize)
      save_bmp = wx.ArtProvider.GetBitmap(wx.ART_FILE_SAVE, wx.ART_TOOLBAR, tsize)

      undo_bmp = wx.ArtProvider.GetBitmap(wx.ART_UNDO, wx.ART_TOOLBAR, tsize)
      redo_bmp = wx.ArtProvider.GetBitmap(wx.ART_REDO, wx.ART_TOOLBAR, tsize)

      cut_bmp = wx.ArtProvider.GetBitmap(wx.ART_CUT, wx.ART_TOOLBAR, tsize)
      copy_bmp = wx.ArtProvider.GetBitmap(wx.ART_COPY, wx.ART_TOOLBAR, tsize)
      paste_bmp= wx.ArtProvider.GetBitmap(wx.ART_PASTE, wx.ART_TOOLBAR, tsize)

      prompt_me_bmp= wx.ArtProvider.GetBitmap(wx.ART_TIP, wx.ART_TOOLBAR, tsize)

      self.toolbar.SetToolBitmapSize(tsize)
      
      BindTool(self.toolbar.AddLabelTool(wx.ID_NEW, "New", new_bmp, shortHelp="New", longHelp="New file"),
               self.OnFileMenuNew)
      BindTool(self.toolbar.AddLabelTool(wx.ID_OPEN, "Open", open_bmp, shortHelp="Open", longHelp="Open file"),
               self.OnFileMenuOpen)
      BindTool(self.toolbar.AddLabelTool(wx.ID_SAVE, "Save", save_bmp, shortHelp="Save", longHelp="Save file"),
               self.OnFileMenuSave)
      self.toolbar.AddSeparator()
      BindTool(self.toolbar.AddLabelTool(wx.ID_UNDO, "Undo", undo_bmp, shortHelp="Undo", longHelp="Undo last action"),
               self.OnEditMenuUndo)
      BindTool(self.toolbar.AddLabelTool(wx.ID_REDO, "Redo", redo_bmp, shortHelp="Redo", longHelp="Redo last action"),
               self.OnEditMenuRedo)
      self.toolbar.AddSeparator()
      BindTool(self.toolbar.AddLabelTool(wx.ID_CUT, "Cut", cut_bmp, shortHelp="Cut", longHelp="Cut selection"),
               self.OnEditMenuCut)
      BindTool(self.toolbar.AddLabelTool(wx.ID_COPY, "Copy", copy_bmp, shortHelp="Copy", longHelp="Copy selection"),
               self.OnEditMenuCopy)
      BindTool(self.toolbar.AddLabelTool(wx.ID_PASTE, "Paste", paste_bmp, shortHelp="Paste", longHelp="Paste selection"),
               self.OnEditMenuPaste)
      self.toolbar.AddSeparator()
      BindTool(self.toolbar.AddLabelTool(self.ID_PROMPT_ME, "Prompt me", prompt_me_bmp, shortHelp="Prompt me", longHelp="Prompt me with a prediction"),
               self.OnPresageMenuPromptMe)

      self.toolbar.Realize()

   # menu handlers
   def OnFileMenuNew(self, event):
      self.editor.file = None
      self.editor.ClearAll()
      self.fileMenu.Enable(wx.ID_SAVE, False)
      self.fileMenu.Enable(wx.ID_SAVEAS, False)

   def OnFileMenuOpen(self, event):
      print "Opening a file.."

      # Create the dialog. In this case the current directory is forced as the starting
      # directory for the dialog, and no default file name is forced. This can easilly
      # be changed in your program. This is an 'open' dialog, and allows multitple
      # file selections as well.
      #
      # Finally, if the directory is changed in the process of getting files, this
      # dialog is set up to change the current working directory to the path chosen.
      dlg = wx.FileDialog(
          self, message="Choose a file", defaultDir="", 
          defaultFile="", wildcard=self.wildcard, style=wx.OPEN | wx.CHANGE_DIR
          )
      
      # Show the dialog and retrieve the user response. If it is the OK response, 
      # process the data.
      if dlg.ShowModal() == wx.ID_OK:
          # This returns a Python list of files that were selected.
          path = dlg.GetPath()
      
          print ("Opening %s\n" % path)

          try:
              fsock = open(path, 'r')
              contents = fsock.read()
              fsock.close()
              self.editor.SetText(contents)
              self.editor.file = path       # remember file we're editing
              self.fileMenu.Enable(wx.ID_SAVE, False)
              self.fileMenu.Enable(wx.ID_SAVEAS, True)
          except IOError:
              dialog = wx.MessageDialog(self, "Error opening file %s" % path,
                                        "Error opening file", wx.OK)
              dialog.ShowModal()
              dialog.Destroy()
              
      # Destroy the dialog. Don't do this until you are done with it!
      # BAD things can happen otherwise!
      dlg.Destroy()

   def OnFileMenuSave(self, event):
      print "Save file"
      if self.editor.file == None:
         self.OnFileMenuSaveAs(event)
      else:
         self.__SaveFile(self.editor.file)
         self.fileMenu.Enable(wx.ID_SAVE, False)

   def OnFileMenuSaveAs(self, event):
      print "Save file as"

      # Create the dialog. In this case the current directory is forced as the starting
      # directory for the dialog, and no default file name is forced. This can easilly
      # be changed in your program. This is an 'save' dialog.
      #
      # Unlike the 'open dialog' example found elsewhere, this example does NOT
      # force the current working directory to change if the user chooses a different
      # directory than the one initially set.
      dlg = wx.FileDialog(
          self, message="Save file as ...", defaultDir="", 
          defaultFile="", wildcard=self.wildcard, style=wx.SAVE
          )

      # This sets the default filter that the user will initially see. Otherwise,
      # the first filter in the list will be used by default.
      dlg.SetFilterIndex(2)

      # Show the dialog and retrieve the user response. If it is the OK response, 
      # process the data.
      if dlg.ShowModal() == wx.ID_OK:
          path = dlg.GetPath()
          self.editor.file = path  # remember file we're editing has changed
          self.__SaveFile(path)
          self.fileMenu.Enable(wx.ID_SAVE, False)

      # Destroy the dialog. Don't do this until you are done with it!
      # BAD things can happen otherwise!
      dlg.Destroy()

   def OnFileMenuClose(self, event):
      self.OnFileMenuNew(event)        # this will do for now
   
   def OnFileMenuQuit(self, event):
      print "This should first check that changes have been saved..."
      self.Close(True)
   
   def OnEditMenuUndo(self, event):
      if self.editor.CanUndo():
         self.editor.Undo()
         print "Undo last action"

   def OnEditMenuRedo(self, event):
      if self.editor.CanRedo():
         self.editor.Redo()
         print "Redo last action"

   def OnEditMenuCut(self, event):
      self.clip = self.editor.GetSelectedText()
      self.editor.ReplaceSelection('')
      print "Cut selected text: " + self.clip
   
   def OnEditMenuCopy(self, event):
      self.clip = self.editor.GetSelectedText()
      print "Stored selected text into clip: " + self.clip
   
   def OnEditMenuPaste(self, event):
      self.editor.ReplaceSelection(self.clip)
      print "Replace selection with: " + self.clip

   def OnEditMenuSelectAll(self, event):
      self.editor.SelectAll()

   def OnViewMenuWordWrap(self, event):
      self.editor.ToggleWordWrapMode()

   def OnViewMenuLargerText(self, event):
      self.editor.IncreaseTextSize()

   def OnViewMenuSmallerText(self, event):
      self.editor.DecreaseTextSize()

   def OnViewMenuShowToolbar(self, event):
      if event.Checked():
         self.toolbar.Show()
      else:
         self.toolbar.Hide()
      self.SendSizeEvent() # cause Frame to reevaluate childrens' positions

   def OnPresageMenuPromptMe(self, event):
      self.editor.ShowPrediction()

   def OnPresageMenuToggleFunctionMode(self, event):
      self.editor.function_keys_enabled = event.Checked()

   def OnPresageMenuToggleAutopunctuationMode(self, event):
      self.editor.autopunctuation = event.Checked()

   def OnHelpMenuContents(self, event):
      message = "Sorry, help not written yet."
      dialog = wx.MessageDialog(self, message, "pyprompter help", wx.OK)
      dialog.ShowModal()
      dialog.Destroy()

   def OnHelpMenuAbout(self, event):
      self.__ShowAboutDialogBox()

   def __ShowAboutDialogBox(self):
      name = 'pyprompter'
      version = self.version
      copyright = '(C) 2008 Matteo Vescovi'
      description = '''This program is intended as a demonstration of Presage ONLY.
The Presage project aims to provide an intelligent predictive text entry platform. Its intent is NOT to provide a predictive text entry user interface.

Think of Presage as the predictive backend that sits behind a shiny user interface and does all the predictive heavy lifting.
'''
      website = 'http://presage.sourceforge.net/'
      devs = ["Matteo Vescovi"]
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
      # AboutBox and AboutDialogInfo were introduced in wxPython 2.7.1.1
      if wx.VERSION > (2, 7, 1, 1):
         # build about dialog information
         info = wx.AboutDialogInfo()
         info.SetName(name)
         info.SetVersion(version)
         info.SetCopyright(copyright)
         info.SetDescription(description)
         info.SetWebSite(website)
         info.SetDevelopers(devs)
         info.SetDocWriters(devs)
         info.SetArtists(devs)
         #info.SetTranslators(devs)
         info.SetLicense(license)
         #info.SetIcon()
         
         # show about dialog box
         wx.AboutBox(info)

      else:
         message = name + ' ' + version + '\n' \
                   + '\n' \
                   + copyright + '\n' \
                   + '\n' \
                   + description + '\n' \
                   + website
         
         dialog = wx.MessageDialog(self, message, "About pyprompter", wx.OK)
         dialog.ShowModal()
         dialog.Destroy()

   def __SaveFile(self, path):
      try:
         fp = open(path, 'w') # Create file anew
         fp.write(self.editor.GetText())
         fp.close()
      except IOError:
         dialog = wx.MessageDialog(self, "Error saving file %s" % path,
                                   "Error saving file", wx.OK)
         dialog.ShowModal()
         dialog.Destroy()

################
# PrompterEditor
#
class PrompterEditor(wx.stc.StyledTextCtrl):
   def __init__(self, parent, config, suggestions):
      wx.stc.StyledTextCtrl.__init__(self, parent)

      self.parent = parent    # remember parent access frame menus
      self.file = None        # remember what file to save to
      self.append_whitespace_on_completion = True
      self.autopunctuation = True
      self.autopunctuation_whitespace = ' '
      self.autopunctuation_chars = ".,;:'?!$%&"
      self.function_keys_enabled = True


      self.Bind(wx.EVT_CHAR, self.OnChar)
      self.Bind(wx.stc.EVT_STC_USERLISTSELECTION, self.OnUserListSelection)
      self.Bind(wx.stc.EVT_STC_MODIFIED, self.OnModified)

      class PrompterPresageCallback(presage.PresageCallback):
         def __init__(self, edtr):
            presage.PresageCallback.__init__(self)
            self.editor = edtr

         def get_past_stream(self):
            result = self.editor.GetTextRange(0,
                                              self.editor.GetCurrentPos())
            return str(result)

         def get_future_stream(self):
            result = self.editor.GetTextRange(self.editor.GetCurrentPos(),
                                              self.editor.GetTextLength())
            return str(result)

      callback = PrompterPresageCallback(self).__disown__()

      if config:
         self.prsg = presage.Presage(callback, config)
      else:
         self.prsg = presage.Presage(callback)

      if suggestions:
         self.prsg.config('Presage.Selector.SUGGESTIONS', suggestions)

      self.SetCodePage(wx.stc.STC_CP_UTF8) # set unicode codepage
      self.SetWrapMode(wx.stc.STC_WRAP_WORD)
      self.AutoCompSetAutoHide(False)
      self.separator = ','
      self.AutoCompSetSeparator(44)
      #self.AutoCompSetIgnoreCase(1)
      self.AutoCompSetMaxHeight(int(self.prsg.config("Presage.Selector.SUGGESTIONS")))

      self.text_size = 10
      self.StyleSetSize(wx.stc.STC_STYLE_DEFAULT, self.text_size)

      # hide margings
      for i in range(5):
         self.SetMarginWidth(i, 0)

      # delaying the __ShowPrediction until after the parent frame and
      # the STC are shown.
      wx.CallAfter(self.__ShowPrediction)
      wx.CallAfter(self.SetSTCFocus, 1)

   def OnChar(self, event):
      print "------------ OnChar() handler"

      if event.HasModifiers():
         if self.AutoCompActive():
            self.AutoCompCancel()

         # CTRL or ALT pressed
         # let the default menu handlers handle the command event
         event.Skip()

      else:
         key = event.GetUnicodeKey()

         keycode = event.GetKeyCode()
         if self.__FunctionKey(keycode):
            self.__HandleFunctionKey(keycode)

         else:
            key = unichr(key)

            self.parent.fileMenu.Enable(wx.ID_SAVE, True)
            self.parent.fileMenu.Enable(wx.ID_SAVEAS, True)

            if self.__AutoPunctuation(key):
               # autopunctuation takes care of adding text and updating
               # prsg, nothing to do here.
               pass
            else:
               self.AddText(key)

               self.__ShowPrediction()

   def ShowPrediction(self, string = ''):
      self.__ShowPrediction(string)

   def __ShowPrediction(self, string = ''):
      print "------------ __ShowPrediction()"
      try:
         prefix = self.prsg.prefix()
         context = self.prsg.context()
         context_change = self.prsg.context_change()

         self.prediction = self.prsg.predict()

         if self.function_keys_enabled:
            self.prediction = self.__PrependFunctionLabel(self.prediction)
         self.suggestions = self.separator.join(self.prediction);

      except presage.PresageException, ex:
         print 'Caught exception %s' % (ex)
         print '  code: %d' % (ex.code())
         print '  what: %s' % (ex.what())
         

      print "String:         " + string
      print "Prefix:         " + prefix
      print "Prefix len:     " + str(len(prefix))
      print "Context:        " + context
      print "Context change: " + str(context_change)
      print "Prediction:     " + self.suggestions

      if self.AutoCompActive():
         self.AutoCompCancel()

      # AutoCompShow() does not generate an event when autocompletion is
      # successful, hence it is not possible to notify presage that
      # the current token was completed.
      #self.AutoCompShow(len(prefix), suggestions)

      # UserListShow() throws an EVT_STC_USERLISTSELECTION event that we
      # can handle to notify presage that the token was automatically
      # completed.
      self.UserListShow(1, self.suggestions)

   def __AutoPunctuation(self, char):
      if self.autopunctuation:
         # autopunctuation is enabled
         if char in self.autopunctuation_chars:
            # char is an autopunctuation character
            curr_pos = self.GetCurrentPos()
            if curr_pos > 0:
               # previous character exists
               prev_pos = curr_pos - 1
               prev_char = chr(self.GetCharAt(prev_pos))
               if prev_char in self.autopunctuation_whitespace:
                  # previous character is an autopunctuation whitespace

                  # swap whitespace and current char
                  self.SetSelection(prev_pos, curr_pos)
                  self.ReplaceSelection(char + ' ')

                  return True

      return False

   def __FunctionKey(self, keycode):
      result = False
      if self.function_keys_enabled:
         self.function_keys = [wx.WXK_F1, wx.WXK_F2, wx.WXK_F3, wx.WXK_F4,  wx.WXK_F5,  wx.WXK_F6,
                               wx.WXK_F7, wx.WXK_F8, wx.WXK_F9, wx.WXK_F10, wx.WXK_F11, wx.WXK_F12]
         if keycode in self.function_keys:
            result = True
      return result

   def __HandleFunctionKey(self, key):
      print "Got function key " + str(key)

      try:
         idx = self.function_keys.index(key)
         print self.prediction[idx]
         if self.AutoCompActive():
            self.AutoCompCancel()

         self.UserListShow(1, self.suggestions)
         self.AutoCompSelect(self.prediction[idx])
         self.AutoCompComplete()
      except IndexError:
         print 'Key not in prediction completion list'
   
   def __PrependFunctionLabel(self, prediction):
      return ['F' + str(i + 1) + ' ' + prediction[i] for i in range(len(prediction))]

   def __RemoveFunctionLabel(self, completion):
      idx = completion.find(' ')
      if not idx == -1:
         completion = completion[idx+1:]
      return completion

   def OnUserListSelection(self, event):
      completion = unicode(event.GetText())
      if self.function_keys_enabled:
         completion = self.__RemoveFunctionLabel(completion)
      prefix_length = len(unicode(self.prsg.prefix()))
      
      print "----------- OnUserListSelection() handler"
      print "Completion:    " + completion
      print "Prefix length: " + str(prefix_length)
      print "To be added:   " + completion[prefix_length:]

      # no need to call complete, using callbacks
      #self.prsg.complete(completion.encode('utf-8'))
      self.AddText(completion[prefix_length:])

      if self.append_whitespace_on_completion:
         self.AddText(' ')
         # no need to update, using callbacks
         #self.prsg.update(' ')

      # schedule showing of prediction after current and pending events
      # are dealt with (thanks to Robin Dunn for pointing this out)
      wx.CallAfter(self.__ShowPrediction)

   def OnModified(self, event):
      # grey out or enable menu items
      self.parent.fileMenu.Enable(wx.ID_SAVE, True)
      self.parent.fileMenu.Enable(wx.ID_SAVEAS, True)
      
      if self.CanUndo():
         self.parent.editMenu.Enable(wx.ID_UNDO, True)
      else:
         self.parent.editMenu.Enable(wx.ID_UNDO, False)
      
      if self.CanRedo():
         self.parent.editMenu.Enable(wx.ID_REDO, True)
      else:
         self.parent.editMenu.Enable(wx.ID_REDO, False)

   def ToggleWordWrapMode(self):
      if self.parent.word_wrap.IsChecked():
         self.SetWrapMode(wx.stc.STC_WRAP_WORD)
      else:
         self.SetWrapMode(wx.stc.STC_WRAP_NONE)

   def IncreaseTextSize(self):
      self.text_size += 1
      self.StyleSetSize(wx.stc.STC_STYLE_DEFAULT, self.text_size)

   def DecreaseTextSize(self):
      if self.text_size > 1:
         self.text_size -= 1
         self.StyleSetSize(wx.stc.STC_STYLE_DEFAULT, self.text_size)
