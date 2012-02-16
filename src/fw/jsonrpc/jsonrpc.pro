#-------------------------------------------------
#
# Project created by QtCreator 2012-02-13T12:24:04
#
#-------------------------------------------------

QT       += core
QT       -= gui

win32 {
   TARGET = ../../../../lib/fwjsonrpc
}
else {
   TARGET = ../../../lib/fwjsonrpc
}

TEMPLATE = lib

DEFINES += FW_JSONRPC_LIBRARY

win32 {
    LIBS += -L../../../../lib \
            -lfwcore1
}
else {
    LIBS += -L../../../lib \
            -lfwcore1
}

INCLUDEPATH += ./../..

SOURCES += \
    jsonrpc.cpp

HEADERS += \
    jsonrpc.hpp \
    jsonrpc_inl.hpp

