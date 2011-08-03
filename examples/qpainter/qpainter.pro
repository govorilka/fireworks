
QT += gui core

win32 {
   TARGET = ../../../bin/qpainter
}
else {
   TARGET = ../../bin/qpainter
}

TEMPLATE = lib

DEFINES += FW_BACKEND_QPAINTER_LIBRARY

INCLUDEPATH += .

INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin \
        -lfireworks1 \


HEADERS += \
    qprender.h \
    qpgraphicsview.h \
    qpfontdata.h \
    qppixmapdata.h \
    fw_backend_qpainter.h

SOURCES += \
    qprender.cpp \
    qpgraphicsview.cpp \
    qpfontdata.cpp \
    qppixmapdata.cpp
