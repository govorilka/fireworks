#-------------------------------------------------
#
# Project created by QtCreator 2011-12-16T16:16:55
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = url
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#Fireworks lib
message($(FIREWORKS_PATH))
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4

SOURCES += main.cpp
