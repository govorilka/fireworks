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
LIBS += -L$(FIREWORKS_PATH)/bin \
        -lfireworks1 \
        -lqpainter1

INCLUDEPATH += $(FIREWORKS_PATH)/src/ \
               ../qpainter/

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

OTHER_FILES += \
    $(FIREWORKS_PATH)/bin/example2.fwml \
    $(FIREWORKS_PATH)/bin/example.fwml \
    $(FIREWORKS_PATH)/bin/example3.fwml \
    $(FIREWORKS_PATH)/bin/example4.fwml \
    $(FIREWORKS_PATH)/bin/example5.fwml
