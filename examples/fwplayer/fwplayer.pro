#-------------------------------------------------
#
# Project created by QtCreator 2011-01-21T12:08:14
#
#-------------------------------------------------

QT       += core gui

TARGET = ../../bin/fwplayer
TEMPLATE = app

LIBS += -L../../bin \
        -lfireworks

INCLUDEPATH += ../../src/ \
               ../qpainter/

SOURCES += main.cpp \
    playerscene.cpp

HEADERS  += \
    playerscene.h

FORMS    +=

include(../qpainter/qpainter.pri)

