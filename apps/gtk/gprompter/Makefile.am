
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
scintilla/src/CellBuffer.cxx \
scintilla/src/CellBuffer.h \
scintilla/src/CharacterSet.h \
scintilla/src/CharClassify.cxx \
scintilla/src/CharClassify.h \
scintilla/src/ContractionState.cxx \
scintilla/src/ContractionState.h \
scintilla/src/Decoration.cxx \
scintilla/src/Decoration.h \
scintilla/src/DocumentAccessor.cxx \
scintilla/src/DocumentAccessor.h \
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
scintilla/src/KeyWords.cxx \
scintilla/src/LexAbaqus.cxx \
scintilla/src/LexAda.cxx \
scintilla/src/LexAPDL.cxx \
scintilla/src/LexAsm.cxx \
scintilla/src/LexAsn1.cxx \
scintilla/src/LexASY.cxx \
scintilla/src/LexAU3.cxx \
scintilla/src/LexAVE.cxx \
scintilla/src/LexBaan.cxx \
scintilla/src/LexBash.cxx \
scintilla/src/LexBasic.cxx \
scintilla/src/LexBullant.cxx \
scintilla/src/LexCaml.cxx \
scintilla/src/LexCLW.cxx \
scintilla/src/LexCmake.cxx \
scintilla/src/LexCOBOL.cxx \
scintilla/src/LexConf.cxx \
scintilla/src/LexCPP.cxx \
scintilla/src/LexCrontab.cxx \
scintilla/src/LexCsound.cxx \
scintilla/src/LexCSS.cxx \
scintilla/src/LexD.cxx \
scintilla/src/LexEiffel.cxx \
scintilla/src/LexErlang.cxx \
scintilla/src/LexEScript.cxx \
scintilla/src/LexFlagship.cxx \
scintilla/src/LexForth.cxx \
scintilla/src/LexFortran.cxx \
scintilla/src/LexGAP.cxx \
scintilla/src/LexGen.py \
scintilla/src/LexGui4Cli.cxx \
scintilla/src/LexHaskell.cxx \
scintilla/src/LexHTML.cxx \
scintilla/src/LexInno.cxx \
scintilla/src/LexKix.cxx \
scintilla/src/LexLisp.cxx \
scintilla/src/LexLout.cxx \
scintilla/src/LexLua.cxx \
scintilla/src/LexMagik.cxx \
scintilla/src/LexMatlab.cxx \
scintilla/src/LexMetapost.cxx \
scintilla/src/LexMMIXAL.cxx \
scintilla/src/LexMPT.cxx \
scintilla/src/LexMSSQL.cxx \
scintilla/src/LexMySQL.cxx \
scintilla/src/LexNimrod.cxx \
scintilla/src/LexNsis.cxx \
scintilla/src/LexOpal.cxx \
scintilla/src/LexOthers.cxx \
scintilla/src/LexPascal.cxx \
scintilla/src/LexPB.cxx \
scintilla/src/LexPerl.cxx \
scintilla/src/LexPLM.cxx \
scintilla/src/LexPOV.cxx \
scintilla/src/LexPowerPro.cxx \
scintilla/src/LexPowerShell.cxx \
scintilla/src/LexProgress.cxx \
scintilla/src/LexPS.cxx \
scintilla/src/LexPython.cxx \
scintilla/src/LexR.cxx \
scintilla/src/LexRebol.cxx \
scintilla/src/LexRuby.cxx \
scintilla/src/LexScriptol.cxx \
scintilla/src/LexSmalltalk.cxx \
scintilla/src/LexSML.cxx \
scintilla/src/LexSorcus.cxx \
scintilla/src/LexSpecman.cxx \
scintilla/src/LexSpice.cxx \
scintilla/src/LexSQL.cxx \
scintilla/src/LexTACL.cxx \
scintilla/src/LexTADS3.cxx \
scintilla/src/LexTAL.cxx \
scintilla/src/LexTCL.cxx \
scintilla/src/LexTeX.cxx \
scintilla/src/LexVB.cxx \
scintilla/src/LexVerilog.cxx \
scintilla/src/LexVHDL.cxx \
scintilla/src/LexYAML.cxx \
scintilla/src/LineMarker.cxx \
scintilla/src/LineMarker.h \
scintilla/src/Partitioning.h \
scintilla/src/PerLine.cxx \
scintilla/src/PerLine.h \
scintilla/src/PositionCache.cxx \
scintilla/src/PositionCache.h \
scintilla/src/PropSet.cxx \
scintilla/src/PropSetSimple.h \
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
scintilla/src/StyleContext.cxx \
scintilla/src/StyleContext.h \
scintilla/src/Style.cxx \
scintilla/src/Style.h \
scintilla/src/SVector.h \
scintilla/src/UniConversion.cxx \
scintilla/src/UniConversion.h \
scintilla/src/ViewStyle.cxx \
scintilla/src/ViewStyle.h \
scintilla/src/WindowAccessor.cxx \
scintilla/src/XPM.cxx \
scintilla/src/XPM.h

SCINTILLA_INCLUDE = \
scintilla/include/Accessor.h \
scintilla/include/KeyWords.h \
scintilla/include/Platform.h \
scintilla/include/PropSet.h \
scintilla/include/SciLexer.h \
scintilla/include/Scintilla.h \
scintilla/include/ScintillaWidget.h \
scintilla/include/WindowAccessor.h 

#scintilla/include/Face.py
#scintilla/include/HFacer.py
#scintilla/include/Scintilla.iface


libscintilla_la_SOURCES =	$(SCINTILLA_SRC) $(SCINTILLA_GTK) $(SCINTILLA_INCLUDE)
libscintilla_la_CXXFLAGS =	-DNDEBUG -Os -DGTK -DSCI_LEXER
libscintilla_la_CPPFLAGS =	$(GNOME_CFLAGS) $(GTHREAD_CFLAGS) \
				-I$(srcdir)/scintilla/include \
				-I$(srcdir)/scintilla/src 
libscintilla_la_LIBADD = 	$(GNOME_LIBS) $(GTHREAD_LIBS) 

# Distribute files from here.
EXTRA_DIST = scintilla/License.txt scintilla/version.txt
#
# End of rules to build scintilla
#################################


bin_PROGRAMS =		gprompter

gprompter_SOURCES = 	gprompter.cpp
gprompter_CPPFLAGS =	$(GNOME_CFLAGS) $(GTHREAD_CFLAGS) \
			-I$(top_srcdir)/src/lib \
			-I$(srcdir)/scintilla/include 
gprompter_LDADD = 	$(GNOME_LIBS) $(GTHREAD_LIBS) \
			../../../src/lib/libpresage.la \
			libscintilla.la

if HAVE_HELP2MAN
gprompter.1:	gprompter.cpp $(top_srcdir)/configure.ac
	$(MAKE) $(AM_MAKEFLAGS) gprompter$(EXEEXT)
	help2man --output=$@ --name="gprompter" ./gprompter$(EXEEXT)

dist_man_MANS =	gprompter.1
endif

endif
