#-------------------------------------------------
#
# Project created by QtCreator 2011-02-06T20:33:00
#
#-------------------------------------------------

QT       += core gui

TARGET = scripter
TEMPLATE = app

INCLUDEPATH += ../../src/

LIBS     += -L../../bin \
            -lfireworks

SOURCES  += main.cpp\
            mainwindow.cpp \
            treemodel.cpp

HEADERS  += mainwindow.h \
            treemodel.h

FORMS    += mainwindow.ui
