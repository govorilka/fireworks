#-------------------------------------------------
#
# Project created by QtCreator 2010-12-12T23:22:45
#
#-------------------------------------------------

QT       -= gui

TARGET = ../bin/fireworks

TEMPLATE = lib

DEFINES += FIREWORKS_LIBRARY

INCLUDEPATH += .

SOURCES += fireworks.cpp

HEADERS += fireworks.h

include (fwcore/fwcore.pri)
