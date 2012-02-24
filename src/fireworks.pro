#-------------------------------------------------
#
# Project created by QtCreator 2010-12-12T23:22:45
#
#-------------------------------------------------

QT       += core network
QT       -= gui

win32 {
   TARGET = ../../bin/fireworks3
}
else {
   TARGET = ../bin/fireworks3
}

TEMPLATE = lib

DEFINES += FIREWORKS_LIBRARY

INCLUDEPATH += .

LIBS += -L$(FIREWORKS_PATH)/lib \
        -lfwcore1 \
        -lfwjsonrpc1

SOURCES += fireworks.cpp

HEADERS += fireworks.h

include (fwcore/fwcore.pri)
include (fwtypography/fwtypography.pri)
include (fwgui/fwgui.pri)
include (fwdb/fwdb.pri)
include (fwutils/fwutils.pri)

