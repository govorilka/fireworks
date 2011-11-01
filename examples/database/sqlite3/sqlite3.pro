QT       += core
QT       -= gui

win32 {
    TARGET = ../../../../bin/sqlite3
}
else {
    TARGET = ../../../bin/sqlite3
}

CONFIG   += console
CONFIG   -= app_bundle

DEFINES += FW_SQLITE

TEMPLATE = app

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin \
        -lfireworks4

INCLUDEPATH += ../../../src/fwdb/sqlite/

SOURCES += main.cpp \
    ../../../src/fwdb/sqlite/sqlite3.c

HEADERS += \
    ../../../src/fwdb/sqlite/sqlite3.h \
    ../../../src/fwdb/sqlite/sqlite3ext.h


