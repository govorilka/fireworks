#-------------------------------------------------
#
# Project created by QtCreator 2011-01-21T12:09:52
#
#-------------------------------------------------

TARGET = ../../bin/qp_backend

TEMPLATE = lib

DEFINES += QPAINTER_FIREWORKS_BACKEND

LIBS += -L../../bin \
        -lfireworks

INCLUDEPATH += ../../src/

SOURCES += qpainterbackend.cpp \
    qpgraphicsview.cpp \
    qpfontdata.cpp \
    qprender.cpp

HEADERS += qpainterbackend.h \
    qpgraphicsview.h \
    qpfontdata.h \
    qprender.h
