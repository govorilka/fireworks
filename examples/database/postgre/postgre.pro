#-------------------------------------------------
#
# Project created by QtCreator 2011-10-31T11:12:57
#
#-------------------------------------------------

QT       += core
QT       -= gui

win32 {
    TARGET = ../../../../bin/examples/postgre
}
else {
    TARGET = ../../../bin/examples/postgre
}

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include ($(FIREWORKS_PATH)/src/fwdb/postgresql.pri)

#Fireworks lib
message($(FIREWORKS_PATH))
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4

SOURCES += main.cpp
