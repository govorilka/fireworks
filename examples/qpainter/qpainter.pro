
QT += gui core

win32 {
   TARGET = ../../../bin/qpainter4
}
else {
   TARGET = ../../bin/qpainter4
}

TEMPLATE = lib

DEFINES += FW_BACKEND_QPAINTER_LIBRARY

INCLUDEPATH += .

message( "External path: $(FIREWORKS_PATH)" )

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4

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
