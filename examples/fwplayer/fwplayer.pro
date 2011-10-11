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

#Fireworks library
message("Fireworks library path: $(FIREWORKS_PATH)")
INCLUDEPATH += $(FIREWORKS_PATH)/src/ \
               ../qpainter/
LIBS += -L$(FIREWORKS_PATH)/bin \
        -lfireworks1 \
        -lqpainter1

SOURCES += main.cpp \
    playerscene.cpp \
    playerscene2.cpp \
    playerscene3.cpp \
    playerscene4.cpp \
    playerscene5.cpp \
    playerscene6.cpp

HEADERS  += \
    playerscene.h \
    playerscene2.h \
    playerscene3.h \
    playerscene4.h \
    playerscene5.h \
    playerscene6.h

include(../../bin/examples/examples.pri)

