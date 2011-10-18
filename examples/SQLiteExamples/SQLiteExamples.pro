#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T13:08:41
#
#-------------------------------------------------

QT       += core

QT       -= gui

win32 {
   TARGET = ../../../bin/SQLiteExamples
}
else {
   TARGET = ../../bin/SQLiteExamples
}

INCLUDEPATH += ../../src
LIBS += -L../../bin \
        -lfireworks4

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
