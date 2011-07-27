
QT       += core gui

TARGET = ../../bin/jsontree

TEMPLATE = app

INCLUDEPATH += ../../src/

LIBS += -L../../bin \
        -lfireworks1

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
