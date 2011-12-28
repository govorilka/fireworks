#-------------------------------------------------
#
# Project created by QtCreator 2011-12-22T08:42:19
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4
