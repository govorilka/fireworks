#-------------------------------------------------
#
# Project created by QtCreator 2012-01-12T16:13:46
#
#-------------------------------------------------

QT       += core
QT       -= gui

win32 {
    TARGET = ../../../bin/pluginloader
}
else {
    TARGET = ../../bin/pluginloader
}

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin \
        -lfireworks4

SOURCES += main.cpp
