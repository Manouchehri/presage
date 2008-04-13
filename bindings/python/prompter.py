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
      print "This will eventually open a file"
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
      self.Bind(wx.stc.EVT_STC_MODIFIED, self.OnModified)

   def OnChar(self, event):
      key = chr(event.GetKeyCode())
      prediction = self.soothie.predict(key)
      suggestions = " ".join(prediction);
      prefix = self.soothie.prefix()

      print "------------"
      print "Key:        " + key
      print "Prefix:     " + prefix
      print "Prefix len: " + str(len(prefix))
      print "Context:    " + self.soothie.context()
      print "Prediction: " + suggestions

      if self.AutoCompActive():
         self.AutoCompCancel()

      self.AddText(key)

      self.AutoCompShow(len(prefix), suggestions)

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
