#-------------------------------------------------
#
# Project created by QtCreator 2011-12-12T18:08:24
#
#-------------------------------------------------

QT       += core

QT       -= gui

win32 {
   TARGET = ../../../bin/scheduler
}
else {
   TARGET = ../../bin/scheduler
}
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4

SOURCES += main.cpp
