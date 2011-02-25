#-------------------------------------------------
#
# Project created by QtCreator 2011-01-31T14:15:23
#
#-------------------------------------------------

QT       += core gui testlib

TARGET = ../../bin/fwmenu

TEMPLATE = app

LIBS += -L../../bin \
        -lfireworks

INCLUDEPATH += ../../src/ \
               ../qpainter/

SOURCES += main.cpp\
    menuscene.cpp

HEADERS  += \
    menuscene.h

FORMS    +=

include(../qpainter/qpainter.pri)
