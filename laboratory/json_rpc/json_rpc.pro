#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T17:27:18
#
#-------------------------------------------------

QT       += core network
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle
win32 {
   TARGET = ./bin/json_rpc
}
else {
   TARGET = ./bin/json_rpc
}
TEMPLATE = app

INCLUDEPATH += .

include(cjson.pri)

HEADERS += \
    jsonrpc.hpp \
    caschannel.hpp \
    caschannelcategory.hpp \
    caschannelprogram.hpp \
    globals.hpp

SOURCES += main.cpp \
    jsonrpc.cpp \
    caschannel.cpp \
    caschannelcategory.cpp \
    caschannelprogram.cpp



