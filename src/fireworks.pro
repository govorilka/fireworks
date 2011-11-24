#-------------------------------------------------
#
# Project created by QtCreator 2010-12-12T23:22:45
#
#-------------------------------------------------

QT       += core network
QT       -= gui

win32 {
   TARGET = ../../bin/fireworks4
}
else {
   TARGET = ../bin/fireworks4
}

TEMPLATE = lib

DEFINES += FIREWORKS_LIBRARY

INCLUDEPATH += .

SOURCES += \
    fireworks.cpp \
    fwinterfaces.cpp \
    fwdescriptions.cpp \
    fwplatform.cpp \
    fw/exception.cpp

HEADERS += \
    fireworks.h \
    fwinterfaces.h \
    fwdescriptions.h \
    fwplatform.hpp \
    fw/exception.hpp \
    fw/defs.hpp

include (fwcore/fwcore.pri)
include (fwtypography/fwtypography.pri)
include (fwgui/fwgui.pri)
include (fwutils/fwutils.pri)
include (fw/console/console.pri)
include (fw/parse/parse.pri)
include (fw/database/database.pri)
include (fw/parse/parse.pri)
include (fw/ml/ml.pri)























