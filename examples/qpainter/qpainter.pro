
QT += gui core

win32 {
   TARGET = ../../../bin/qpainter
}
else {
   TARGET = ../../bin/qpainter
}

TEMPLATE = lib

INCLUDEPATH += .

INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin \
        -lfireworks \


HEADERS += \
    qprender.h \
    qpgraphicsview.h \
    qpfontdata.h \
    qppixmapdata.h

SOURCES += \
    qprender.cpp \
    qpgraphicsview.cpp \
    qpfontdata.cpp \
    qppixmapdata.cpp
