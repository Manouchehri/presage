
###########
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

## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = subdir-objects

if BUILD_GTK_PROMPTER

###########
# Scintilla
#
noinst_LTLIBRARIES = libscintilla.la

SCINTILLA_GTK = \
scintilla/gtk/Converter.h \
scintilla/gtk/PlatGTK.cxx \
scintilla/gtk/ScintillaGTK.cxx \
scintilla/gtk/scintilla-marshal.c \
scintilla/gtk/scintilla-marshal.h 

SCINTILLA_SRC = \
scintilla/src/AutoComplete.cxx \
scintilla/src/AutoComplete.h \
scintilla/src/CallTip.cxx \
scintilla/src/CallTip.h \
scintilla/src/Catalogue.cxx \
scintilla/src/Catalogue.h \
scintilla/src/CellBuffer.cxx \
scintilla/src/CellBuffer.h \
scintilla/src/CharClassify.cxx \
scintilla/src/CharClassify.h \
scintilla/src/ContractionState.cxx \
scintilla/src/ContractionState.h \
scintilla/src/Decoration.cxx \
scintilla/src/Decoration.h \
scintilla/src/Document.cxx \
scintilla/src/Document.h \
scintilla/src/Editor.cxx \
scintilla/src/Editor.h \
scintilla/src/ExternalLexer.cxx \
scintilla/src/ExternalLexer.h \
scintilla/src/Indicator.cxx \
scintilla/src/Indicator.h \
scintilla/src/KeyMap.cxx \
scintilla/src/KeyMap.h \
scintilla/src/LineMarker.cxx \
scintilla/src/LineMarker.h \
scintilla/src/Partitioning.h \
scintilla/src/PerLine.cxx \
scintilla/src/PerLine.h \
scintilla/src/PositionCache.cxx \
scintilla/src/PositionCache.h \
scintilla/src/RESearch.cxx \
scintilla/src/RESearch.h \
scintilla/src/RunStyles.cxx \
scintilla/src/RunStyles.h \
scintilla/src/ScintillaBase.cxx \
scintilla/src/ScintillaBase.h \
scintilla/src/SciTE.properties \
scintilla/src/Selection.cxx \
scintilla/src/Selection.h \
scintilla/src/SplitVector.h \
scintilla/src/Style.cxx \
scintilla/src/Style.h \
scintilla/src/UniConversion.cxx \
scintilla/src/UniConversion.h \
scintilla/src/ViewStyle.cxx \
scintilla/src/ViewStyle.h \
scintilla/src/XPM.cxx \
scintilla/src/XPM.h \
scintilla/src/CaseConvert.cxx \
scintilla/src/CaseConvert.h \
scintilla/src/CaseFolder.cxx \
scintilla/src/CaseFolder.h \
scintilla/src/UnicodeFromUTF8.h \
scintilla/src/EditModel.cxx \
scintilla/src/EditModel.h \
scintilla/src/EditView.cxx \
scintilla/src/EditView.h \
scintilla/src/MarginView.cxx \
scintilla/src/MarginView.h

SCINTILLA_INCLUDE = \
scintilla/include/ILexer.h \
scintilla/include/Platform.h \
scintilla/include/SciLexer.h \
scintilla/include/Scintilla.h \
scintilla/include/ScintillaWidget.h

SCINTILLA_LEXLIB = \
scintilla/lexlib/LexerSimple.h \
scintilla/lexlib/LexerNoExceptions.cxx \
scintilla/lexlib/LexerModule.cxx \
scintilla/lexlib/Accessor.h \
scintilla/lexlib/LexerSimple.cxx \
scintilla/lexlib/StyleContext.h \
scintilla/lexlib/PropSetSimple.h \
scintilla/lexlib/WordList.h \
scintilla/lexlib/CharacterSet.h \
scintilla/lexlib/LexerBase.h \
scintilla/lexlib/LexAccessor.h \
scintilla/lexlib/Accessor.cxx \
scintilla/lexlib/LexerNoExceptions.h \
scintilla/lexlib/StyleContext.cxx \
scintilla/lexlib/PropSetSimple.cxx \
scintilla/lexlib/WordList.cxx \
scintilla/lexlib/CharacterSet.cxx \
scintilla/lexlib/OptionSet.h \
scintilla/lexlib/LexerModule.h \
scintilla/lexlib/LexerBase.cxx \
scintilla/lexlib/SparseState.h \
scintilla/lexlib/SubStyles.h \
scintilla/lexlib/CharacterCategory.h \
scintilla/lexlib/CharacterCategory.cxx \
scintilla/lexlib/StringCopy.h

SCINTILLA_LEXERS = \
scintilla/lexers/LexLisp.cxx \
scintilla/lexers/LexErlang.cxx \
scintilla/lexers/LexEScript.cxx \
scintilla/lexers/LexFortran.cxx \
scintilla/lexers/LexAVE.cxx \
scintilla/lexers/LexNsis.cxx \
scintilla/lexers/LexSorcus.cxx \
scintilla/lexers/LexTADS3.cxx \
scintilla/lexers/LexTxt2tags.cxx \
scintilla/lexers/LexBasic.cxx \
scintilla/lexers/LexForth.cxx \
scintilla/lexers/LexMarkdown.cxx \
scintilla/lexers/LexTACL.cxx \
scintilla/lexers/LexMagik.cxx \
scintilla/lexers/LexPLM.cxx \
scintilla/lexers/LexOpal.cxx \
scintilla/lexers/LexFlagship.cxx \
scintilla/lexers/LexScriptol.cxx \
scintilla/lexers/LexMatlab.cxx \
scintilla/lexers/LexBullant.cxx \
scintilla/lexers/LexPB.cxx \
scintilla/lexers/LexRebol.cxx \
scintilla/lexers/LexHTML.cxx \
scintilla/lexers/LexBash.cxx \
scintilla/lexers/LexPowerPro.cxx \
scintilla/lexers/LexTAL.cxx \
scintilla/lexers/LexRuby.cxx \
scintilla/lexers/LexPS.cxx \
scintilla/lexers/LexD.cxx \
scintilla/lexers/LexSpecman.cxx \
scintilla/lexers/LexCSS.cxx \
scintilla/lexers/LexKix.cxx \
scintilla/lexers/LexASY.cxx \
scintilla/lexers/LexVHDL.cxx \
scintilla/lexers/LexGui4Cli.cxx \
scintilla/lexers/LexR.cxx \
scintilla/lexers/LexYAML.cxx \
scintilla/lexers/LexAbaqus.cxx \
scintilla/lexers/LexCaml.cxx \
scintilla/lexers/LexAPDL.cxx \
scintilla/lexers/LexCmake.cxx \
scintilla/lexers/LexCPP.cxx \
scintilla/lexers/LexLout.cxx \
scintilla/lexers/LexCLW.cxx \
scintilla/lexers/LexAU3.cxx \
scintilla/lexers/LexCrontab.cxx \
scintilla/lexers/LexEiffel.cxx \
scintilla/lexers/LexMetapost.cxx \
scintilla/lexers/LexCOBOL.cxx \
scintilla/lexers/LexTeX.cxx \
scintilla/lexers/LexMPT.cxx \
scintilla/lexers/LexBaan.cxx \
scintilla/lexers/LexPerl.cxx \
scintilla/lexers/LexPascal.cxx \
scintilla/lexers/LexOthers.cxx \
scintilla/lexers/LexProgress.cxx \
scintilla/lexers/LexMySQL.cxx \
scintilla/lexers/LexVB.cxx \
scintilla/lexers/LexGAP.cxx \
scintilla/lexers/LexVerilog.cxx \
scintilla/lexers/LexAsm.cxx \
scintilla/lexers/LexLua.cxx \
scintilla/lexers/LexPython.cxx \
scintilla/lexers/LexTCL.cxx \
scintilla/lexers/LexHaskell.cxx \
scintilla/lexers/LexPowerShell.cxx \
scintilla/lexers/LexSmalltalk.cxx \
scintilla/lexers/LexAda.cxx \
scintilla/lexers/LexConf.cxx \
scintilla/lexers/LexMMIXAL.cxx \
scintilla/lexers/LexNimrod.cxx \
scintilla/lexers/LexCsound.cxx \
scintilla/lexers/LexSML.cxx \
scintilla/lexers/LexSQL.cxx \
scintilla/lexers/LexMSSQL.cxx \
scintilla/lexers/LexAsn1.cxx \
scintilla/lexers/LexInno.cxx \
scintilla/lexers/LexSpice.cxx \
scintilla/lexers/LexPOV.cxx \
scintilla/lexers/LexA68k.cxx \
scintilla/lexers/LexModula.cxx \
scintilla/lexers/LexCoffeeScript.cxx \
scintilla/lexers/LexAVS.cxx \
scintilla/lexers/LexTCMD.cxx \
scintilla/lexers/LexECL.cxx \
scintilla/lexers/LexOScript.cxx \
scintilla/lexers/LexVisualProlog.cxx \
scintilla/lexers/LexPO.cxx \
scintilla/lexers/LexLaTeX.cxx \
scintilla/lexers/LexSTTXT.cxx \
scintilla/lexers/LexDMAP.cxx \
scintilla/lexers/LexDMIS.cxx \
scintilla/lexers/LexKVIrc.cxx \
scintilla/lexers/LexRegistry.cxx \
scintilla/lexers/LexRust.cxx



libscintilla_la_SOURCES =	$(SCINTILLA_SRC) $(SCINTILLA_GTK) $(SCINTILLA_INCLUDE) $(SCINTILLA_LEXERS) $(SCINTILLA_LEXLIB)
libscintilla_la_CXXFLAGS =	-DNDEBUG -DGTK -DSCI_LEXER -DG_THREADS_IMPL_NONE
libscintilla_la_CPPFLAGS =	$(GNOME_CFLAGS) $(GTHREAD_CFLAGS) \
				-I$(srcdir)/scintilla/include \
				-I$(srcdir)/scintilla/src \
				-I$(srcdir)/scintilla/lexlib 
libscintilla_la_LIBADD = 	$(GNOME_LIBS) $(GTHREAD_LIBS)

# Distribute files from here.
EXTRA_DIST = scintilla/License.txt scintilla/version.txt
#
# End of rules to build scintilla
#################################


bin_PROGRAMS =		gprompter

gprompter_SOURCES = 	gprompter.c
gprompter_CFLAGS =	$(GNOME_CFLAGS) $(GTHREAD_CFLAGS) $(GMODULE_CFLAGS) \
			-I$(top_srcdir)/src/lib \
			-I$(srcdir)/scintilla/include 
gprompter_LDADD = 	$(GNOME_LIBS) $(GTHREAD_LIBS) $(GMODULE_LIBS) \
			-lm \
			../../../src/lib/libpresage.la \
			libscintilla.la
if USE_GCC
gprompter_LDADD +=	-lstdc++
endif

if HAVE_HELP2MAN
gprompter.1:	gprompter$(EXEEXT) gprompter.c $(top_srcdir)/configure.ac
	help2man --output=$@ --no-info --name="the intelligent predictive GTK+ text editor" ./gprompter$(EXEEXT)

dist_man_MANS =		gprompter.1

DISTCLEANFILES =	gprompter.1
endif

desktopdir = $(datadir)/applications
dist_desktop_DATA = gprompter.desktop

svgicondir = $(datadir)/icons/hicolor/scalable/apps
dist_svgicon_DATA = gprompter.svg

pixmapsdir = $(datadir)/pixmaps
dist_pixmaps_DATA = gprompter.png gprompter.xpm

endif
