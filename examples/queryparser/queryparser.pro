#-------------------------------------------------
#
# Project created by QtCreator 2011-10-05T16:07:45
#
#-------------------------------------------------

QT       += core
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

win32 {
   TARGET = ../../../bin/examples/queryparser
}
else {
   TARGET = ../../bin/examples/queryparser
}

TEMPLATE = app

message("Fireworks library path: $(FIREWORKS_PATH)")
INCLUDEPATH += \
    . \
    $(FIREWORKS_PATH)/src/
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4


SOURCES += \
    main.cpp
