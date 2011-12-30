#-------------------------------------------------
#
# Project created by QtCreator 2011-12-30T11:16:15
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test_event
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4

SOURCES += main.cpp \
    object.cpp \
    event.cpp

HEADERS += \
    object.hpp \
    event.hpp
