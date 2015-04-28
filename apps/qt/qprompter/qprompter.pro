
PRESAGE_ROOT = $$(PRESAGE_ROOT)
isEmpty(PRESAGE_ROOT) {
        win32 {
                error(Environment variable PRESAGE_ROOT must be set)
        }
        unix {
                PRESAGE_ROOT = /usr
        }
}
! exists($${PRESAGE_ROOT}) {
        error(PRESAGE_ROOT does not exist. Please set environment variable PRESAGE_ROOT to a valid presage installation.)
}

CONFIG       += release
HEADERS       = mainwindow.h
SOURCES       = main.cpp \
                mainwindow.cpp
RESOURCES     = qprompter.qrc

QT           += widgets
LIBS         += -lqscintilla2 -L$${PRESAGE_ROOT}/lib -lpresage
INCLUDEPATH   = $${PRESAGE_ROOT}/include


# install
target.path = $$[QT_INSTALL_EXAMPLES]/mainwindows/qprompter
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS qprompter.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/mainwindows/qprompter
INSTALLS += target sources

symbian: include($$PWD/../../symbianpkgrules.pri)
maemo5: include($$PWD/../../maemo5pkgrules.pri)

symbian: warning(This example might not fully work on Symbian platform)
simulator: warning(This example might not fully work on Simulator platform)
