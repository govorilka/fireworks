#-------------------------------------------------
#
# Project created by QtCreator 2011-10-05T16:07:45
#
#-------------------------------------------------

QT       += core
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

TARGET = ../../../bin/examples/queryparser

TEMPLATE = app

#Fireworks library
message("Fireworks library path: $(FIREWORKS_PATH)")
INCLUDEPATH += $(FIREWORKS_PATH)/src/
LIBS += -L$(FIREWORKS_PATH)/bin \
        -lfireworks4

include ( $(FIREWORKS_PATH)/src/fwdb/postgresql.pri )

SOURCES += \
    main.cpp


variable {

}
else {

}
