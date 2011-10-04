#-------------------------------------------------
#
# Project created by QtCreator 2011-05-19T17:22:20
#
#-------------------------------------------------


QT       += core gui testlib

win32 {
    TARGET = ../../../bin/systemInfo
}
else {
    TARGET = ../../bin/systemInfo
}

LIBS += -L../../bin \
        -lfireworks

INCLUDEPATH += ../../src/

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

OTHER_FILES += \
    ../../bin/network.fwml
