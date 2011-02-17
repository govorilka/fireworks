#-------------------------------------------------
#
# Project created by QtCreator 2010-12-12T23:22:45
#
#-------------------------------------------------

QT       += core gui //gui - temporary, for QRegion class

TARGET = ../bin/fireworks

TEMPLATE = lib

DEFINES += FIREWORKS_LIBRARY

INCLUDEPATH += .

SOURCES += fireworks.cpp

HEADERS += fireworks.h

include (fwcore/fwcore.pri)
include (fwtypography/fwtypography.pri)
include (fwgui/fwgui.pri)
include (fwprimitives/fwprimitives.pri)
