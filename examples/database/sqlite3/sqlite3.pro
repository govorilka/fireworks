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

TEMPLATE = app

INCLUDEPATH += ../../../src/fwdb/sqlite/

SOURCES += main.cpp \
    ../../../src/fwdb/sqlite/sqlite3.c

HEADERS += \
    ../../../src/fwdb/sqlite/sqlite3.h \
    ../../../src/fwdb/sqlite/sqlite3ext.h


