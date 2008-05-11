#!/usr/bin/env python

import wx
import wx.stc
import soothsayer

class Prompter(wx.App):
   def OnInit(self):
      self.frame = PrompterFrame(parent=None, id=-1, title='Prompter')
      self.SetTopWindow(self.frame)
      return True

class PrompterFrame(wx.Frame):
   wildcard = "Text files (*.txt)|*.txt|"     \
       "All files (*.*)|*.*"

   def __init__(self, parent, id, title):
      wx.Frame.__init__(self, parent, id, title)

      message = """
Prompter, the intelligent mind reader

Copyright (C) Matteo Vescovi

This program is intended as a demonstration of Soothsayer ONLY.

The Soothsayer project aims to provide an intelligent predictive text entry platform. Its intent is NOT to provide a predictive text entry user interface.

Think of Soothsayer as the predictive backend that sits behind a shiny user interface and does all the predictive heavy lifting.
"""
      dialog = wx.MessageDialog(self, message, "About Prompter demo", wx.OK)
      dialog.ShowModal()
      dialog.Destroy()

      self.MakeMenuBar()

      self.editor = PrompterEditor(self)

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

      # help menu
      self.helpMenu = wx.Menu()
      BindMenu(self.helpMenu.Append(wx.ID_HELP, "&Contents\tF1"), self.OnHelpMenuContents)
      self.helpMenu.AppendSeparator()
      BindMenu(self.helpMenu.Append(wx.ID_ABOUT, "&About\tCTRL+A"), self.OnHelpMenuAbout)

      # menu bar
      self.menuBar = wx.MenuBar()
      self.menuBar.Append(self.fileMenu, "&File")
      self.menuBar.Append(self.helpMenu, "&Help")
      self.SetMenuBar(self.menuBar)

      # grey out menu items
      self.fileMenu.Enable(wx.ID_SAVE, False)
      self.fileMenu.Enable(wx.ID_SAVEAS, False)

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
   def OnHelpMenuContents(self, event):
      print "This will eventually open the online help"
   def OnHelpMenuAbout(self, event):
      message = """Prompter, the intelligent mind reader

Copyright (C) Matteo Vescovi

This is free software; see the source for copying conditions. \
There is NO warranty; not even for MERCHANTABILITY or FITNESS \
FOR A PARTICULAR PURPOSE, to the extent permitted by law."""
      dialog = wx.MessageDialog(self, message, "About Prompter", wx.OK)
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


class PrompterEditor(wx.stc.StyledTextCtrl):
   def __init__(self, parent):
      wx.stc.StyledTextCtrl.__init__(self, parent)

      self.parent = parent    # remember parent access frame menus
      self.file = None        # remember what file to save to
      self.append_whitespace_on_completion = True
      self.autopunctuation = True
      self.autopunctuation_whitespace = ' '
      self.autopunctuation_chars = ".,;:'?!$%&"

      self.Bind(wx.EVT_CHAR, self.OnChar)
      self.Bind(wx.stc.EVT_STC_USERLISTSELECTION, self.OnUserListSelection)
      self.Bind(wx.stc.EVT_STC_MODIFIED, self.OnModified)

      self.soothie = soothsayer.Soothsayer()

      #self.AutoCompSetAutoHide(False)
      #self.AutoCompSetIgnoreCase(1)

      # delaying the __ShowPrediction until after the parent frame and
      # the STC are shown.
      wx.CallAfter(self.__ShowPrediction)
      wx.CallAfter(self.SetSTCFocus, 1)

   def OnChar(self, event):
      print "------------ OnChar() handler"
      key = unichr(event.GetKeyCode())

      self.parent.fileMenu.Enable(wx.ID_SAVE, True)
      self.parent.fileMenu.Enable(wx.ID_SAVEAS, True)

      if self.__AutoPunctuation(key):
         # autopunctuation takes care of adding text and updating
         # soothie, nothing to do here.
         pass
      else:
         self.AddText(key)
         self.soothie.update(key.encode('utf-8'))

      self.__ShowPrediction()

   def __ShowPrediction(self, string = ''):
      print "------------ __ShowPrediction()"
      prediction = self.soothie.predict(string)
      suggestions = " ".join(prediction);
      prefix = self.soothie.prefix()

      print "String:     " + string
      print "Prefix:     " + prefix
      print "Prefix len: " + str(len(prefix))
      print "Context:    " + self.soothie.context()
      print "Prediction: " + suggestions

      if self.AutoCompActive():
         self.AutoCompCancel()

      # AutoCompShow() does not generate an event when autocompletion is
      # successful, hence it is not possible to notify soothsayer that
      # the current token was completed.
      #self.AutoCompShow(len(prefix), suggestions)

      # UserListShow() throws an EVT_STC_USERLISTSELECTION event that we
      # can handle to notify soothsayer that the token was automatically
      # completed.
      self.UserListShow(1, suggestions)

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

                  # update soothie
                  self.soothie.update('\b' + char.encode('utf-8') + ' ')

                  return True

      return False

   def OnUserListSelection(self, event):
      completion = unicode(event.GetText())
      prefix_length = len(unicode(self.soothie.prefix()))
      
      print "----------- OnUserListSelection() handler"
      print "Completion:    " + completion
      print "Prefix length: " + str(prefix_length)
      print "To be added:   " + completion[prefix_length:]

      self.soothie.complete(completion.encode('utf-8'))
      self.AddText(completion[prefix_length:])

      if self.append_whitespace_on_completion:
         self.AddText(' ')
         self.soothie.update(' ')

      # schedule showing of prediction after current and pending events
      # are dealt with (thanks to Robin Dunn for pointing this out)
      wx.CallAfter(self.__ShowPrediction)

   def OnModified(self, event):
      self.parent.fileMenu.Enable(wx.ID_SAVE, True)
      self.parent.fileMenu.Enable(wx.ID_SAVEAS, True)


if __name__ == "__main__":
   app = Prompter()
   app.MainLoop()
