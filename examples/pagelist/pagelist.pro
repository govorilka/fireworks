#-------------------------------------------------
#
# Project created by QtCreator 2011-03-02T11:07:20
#
#-------------------------------------------------

QT       += core gui

win32 {
    TARGET = ../../../bin/pagelist
}
else {
    TARGET = ../../bin/pagelist
}


TEMPLATE = app

LIBS += -L../../bin \
        -lfireworks

INCLUDEPATH += ../../src/ \
               ../qpainter/

SOURCES += main.cpp \
    menuscene.cpp

include(../qpainter/qpainter.pri)

HEADERS += \
    menuscene.h

OTHER_FILES += \
    ../../bin/pagelist.fwml
