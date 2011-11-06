CONFIG       += release
HEADERS       = mainwindow.h
SOURCES       = main.cpp \
                mainwindow.cpp
#! [0]
RESOURCES     = qprompter.qrc
#! [0]

LIBS         += -lqscintilla2 -Lc:/presage/bin -lpresage-1
INCLUDEPATH   = c:/presage/include

# install
target.path = $$[QT_INSTALL_EXAMPLES]/mainwindows/qprompter
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS qprompter.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/mainwindows/qprompter
INSTALLS += target sources

symbian: include($$PWD/../../symbianpkgrules.pri)
maemo5: include($$PWD/../../maemo5pkgrules.pri)

symbian: warning(This example might not fully work on Symbian platform)
simulator: warning(This example might not fully work on Simulator platform)
