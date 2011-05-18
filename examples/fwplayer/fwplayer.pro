#-------------------------------------------------
#
# Project created by QtCreator 2011-01-21T12:08:14
#
#-------------------------------------------------

QT       += core gui

win32 {
    TARGET = ../../../bin/fwplayer
}
else {
    TARGET = ../../bin/fwplayer
}

TEMPLATE = app

LIBS += -L../../bin \
        -lfireworks

INCLUDEPATH += ../../src/ \
               ../qpainter/

SOURCES += main.cpp \
    playerscene.cpp \
    playerscene2.cpp \
    playerscene3.cpp \
    playerscene4.cpp \
    playerscene5.cpp

HEADERS  += \
    playerscene.h \
    playerscene2.h \
    playerscene3.h \
    playerscene4.h \
    playerscene5.h


include(../qpainter/qpainter.pri)

OTHER_FILES += \
    ../../bin/example2.fwml \
    ../../bin/example.fwml \
    ../../bin/example3.fwml \
    ../../bin/example4.fwml \
    ../../bin/example5.fwml
