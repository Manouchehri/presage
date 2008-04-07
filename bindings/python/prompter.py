#!/usr/bin/env python

import wx
import wx.stc
#import wx.lib.editor

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

      # don't use wx.lib.editor, use stc instead
      #self.editor = wx.lib.editor.Editor(self, id, style=wx.SUNKEN_BORDER)
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
      BindMenu(self.fileMenu.Append(wx.ID_ANY, "&Open"), self.OnFileMenuOpen)
      self.fileMenu.AppendSeparator()
      BindMenu(self.fileMenu.Append(wx.ID_ANY, "&Close"), self.OnFileMenuClose)

      # help menu
      self.helpMenu = wx.Menu()
      BindMenu(self.helpMenu.Append(wx.ID_ANY, "&Contents"), self.OnHelpMenuContents)
      self.helpMenu.AppendSeparator()
      BindMenu(self.helpMenu.Append(wx.ID_ANY, "&About"), self.OnHelpMenuAbout)

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

   def OnChar(self, event):
      prediction = self.soothie.predict(event.GetUnicodeKey())

      #self.AutoCompSetMaxHeight(6) # should be set to soothsayer prediction size
      #self.AutoCompSetFillUps("\n\t")

      prediction_str = " ".join(prediction);

      print prediction
      print self.soothie.context()

      self.AutoCompShow(0, prediction_str)
      event.Skip()

if __name__ == "__main__":
   app = Prompter()
   app.MainLoop()
