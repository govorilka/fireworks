#-------------------------------------------------
#
# Project created by QtCreator 2011-11-29T11:20:15
#
#-------------------------------------------------

QT       += core
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

win32 {
   TARGET = ../../../bin/console
}
else {
   TARGET = ../../bin/console
}

TEMPLATE = app

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4

SOURCES += main.cpp
