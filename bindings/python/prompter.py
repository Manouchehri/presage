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
   def __init__(self, parent, id, title):
      wx.Frame.__init__(self, parent, id, title)

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
      BindMenu(self.fileMenu.Append(wx.ID_ANY, "&Open\tCTRL+O"), self.OnFileMenuOpen)
      BindMenu(self.fileMenu.Append(wx.ID_SAVE, "&Save\tCTRL+S"), self.OnFileMenuSave)
      BindMenu(self.fileMenu.Append(wx.ID_SAVEAS, "Save &As\tSHIFT+CTRL+S"), self.OnFileMenuSaveAs)
      self.fileMenu.AppendSeparator()
      BindMenu(self.fileMenu.Append(wx.ID_ANY, "&Close\tCTRL+W"), self.OnFileMenuClose)

      # help menu
      self.helpMenu = wx.Menu()
      BindMenu(self.helpMenu.Append(wx.ID_ANY, "&Contents\tF1"), self.OnHelpMenuContents)
      self.helpMenu.AppendSeparator()
      BindMenu(self.helpMenu.Append(wx.ID_ANY, "&About\tCTRL+A"), self.OnHelpMenuAbout)

      # menu bar
      self.menuBar = wx.MenuBar()
      self.menuBar.Append(self.fileMenu, "&File")
      self.menuBar.Append(self.helpMenu, "&Help")
      self.SetMenuBar(self.menuBar)

   # menu handlers
   def OnFileMenuOpen(self, event):
      print "Opening a file.."

      wildcard = "Text files (*.txt)|*.txt|"     \
                 "All files (*.*)|*.*"

      # Create the dialog. In this case the current directory is forced as the starting
      # directory for the dialog, and no default file name is forced. This can easilly
      # be changed in your program. This is an 'open' dialog, and allows multitple
      # file selections as well.
      #
      # Finally, if the directory is changed in the process of getting files, this
      # dialog is set up to change the current working directory to the path chosen.
      dlg = wx.FileDialog(
          self, message="Choose a file", defaultDir="", 
          defaultFile="", wildcard=wildcard, style=wx.OPEN | wx.CHANGE_DIR
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
              self.editor.SetText(contents)
              fsock.close()
          except IOError:
              dialog = wx.MessageDialog(self, "Error opening file %s" % path, "Error opening file", wx.OK)
              dialog.ShowModal()
              dialog.Destroy()
              
      # Destroy the dialog. Don't do this until you are done with it!
      # BAD things can happen otherwise!
      dlg.Destroy()

   def OnFileMenuSave(self, event):
      print "Save file"

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
          defaultFile="", wildcard="", style=wx.SAVE
          )

      # This sets the default filter that the user will initially see. Otherwise,
      # the first filter in the list will be used by default.
      dlg.SetFilterIndex(2)

      # Show the dialog and retrieve the user response. If it is the OK response, 
      # process the data.
      if dlg.ShowModal() == wx.ID_OK:
          path = dlg.GetPath()

          try:
              fp = file(path, 'w') # Create file anew
              fp.write(self.editor.GetText())
              fp.close()
          except IOError:
              dialog = wx.MessageDialog(self, "Error saving file %s" % path, "Error saving file", wx.OK)
              dialog.ShowModal()
              dialog.Destroy()

      # Destroy the dialog. Don't do this until you are done with it!
      # BAD things can happen otherwise!
      dlg.Destroy()

   def OnFileMenuClose(self, event):
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

class PrompterEditor(wx.stc.StyledTextCtrl):
   def __init__(self, parent):
      wx.stc.StyledTextCtrl.__init__(self, parent)

      self.soothie = soothsayer.Soothsayer()

      self.Bind(wx.EVT_CHAR, self.OnChar)
      self.Bind(wx.stc.EVT_STC_USERLISTSELECTION, self.OnUserListSelection)
      #self.Bind(wx.stc.EVT_STC_MODIFIED, self.OnModified)

   def OnChar(self, event):
      key = chr(event.GetKeyCode())
      self.AddText(key)
      prediction = self.soothie.predict(key)
      suggestions = " ".join(prediction);
      prefix = self.soothie.prefix()

      print "------------ OnChar() handler"
      print "Key:        " + key
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

   def OnUserListSelection(self, event):
      completion = str(event.GetText())
      prefix_length = len(self.soothie.prefix())
      
      print "----------- OnUserListSelection() handler"
      print "Completion:    " + completion
      print "Prefix length: " + str(prefix_length)
      print "To be added:   " + completion[prefix_length:]

      self.soothie.complete(completion)
      self.AddText(completion[prefix_length:])

   def OnModified(self, event):
      print """OnModified
        Mod type:     %s
        At position:  %d
        Lines added:  %d
        Text Length:  %d
        Text:         %s\n""" % ( self.transModType(event.GetModificationType()),
                                  event.GetPosition(),
                                  event.GetLinesAdded(),
                                  event.GetLength(),
                                  repr(event.GetText()) )

      if wx.stc.STC_MOD_INSERTTEXT & event.GetModificationType():
         print "STC_MOD_INSERTTEXT : " + str(event.GetText())

         self.soothie.update(str(event.GetText()))

         print "Context:    " + self.soothie.context()

      elif wx.stc.STC_MOD_DELETETEXT & event.GetModificationType():
         print "STC_MOD_DELETETEXT : " + str(event.GetText())
         for i in str(event.GetText()):
            self.soothie.update('\b')

         print "Context:    " + self.soothie.context()

      else:
         print "Unhandled STC_MOD event received"


   def transModType(self, modType):
      st = ""
      table = [(wx.stc.STC_MOD_INSERTTEXT, "InsertText"),
               (wx.stc.STC_MOD_DELETETEXT, "DeleteText"),
               (wx.stc.STC_MOD_CHANGESTYLE, "ChangeStyle"),
               (wx.stc.STC_MOD_CHANGEFOLD, "ChangeFold"),
               (wx.stc.STC_PERFORMED_USER, "UserFlag"),
               (wx.stc.STC_PERFORMED_UNDO, "Undo"),
               (wx.stc.STC_PERFORMED_REDO, "Redo"),
               (wx.stc.STC_LASTSTEPINUNDOREDO, "Last-Undo/Redo"),
               (wx.stc.STC_MOD_CHANGEMARKER, "ChangeMarker"),
               (wx.stc.STC_MOD_BEFOREINSERT, "B4-Insert"),
               (wx.stc.STC_MOD_BEFOREDELETE, "B4-Delete"),
               (wx.stc.STC_MODEVENTMASKALL, "MaskAll")
               ]

      for flag,text in table:
         if flag & modType:
            st = st + text + " "
            
         if not st:
            st = 'UNKNOWN '

      return st


if __name__ == "__main__":
   app = Prompter()
   app.MainLoop()
