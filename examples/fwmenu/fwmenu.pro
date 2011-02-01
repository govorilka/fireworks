#-------------------------------------------------
#
# Project created by QtCreator 2011-01-31T14:15:23
#
#-------------------------------------------------

QT       += core gui

TARGET = ../../bin/fwmenu

TEMPLATE = app

LIBS += -L../../bin \
        -lfireworks

INCLUDEPATH += ../../src/ \
               ../qpainter/

SOURCES += main.cpp\
        mainwindow.cpp \
    menuscene.cpp

HEADERS  += mainwindow.h \
    menuscene.h

FORMS    +=

include(../qpainter/qpainter.pri)
