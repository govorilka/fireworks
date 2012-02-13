#-------------------------------------------------
#
# Project created by QtCreator 2012-02-13T12:24:04
#
#-------------------------------------------------

QT       += core
QT       -= gui

win32 {
   TARGET = ../../../../lib/fwcore1
}
else {
   TARGET = ../../../lib/fwcore1
}

TEMPLATE = lib

DEFINES += FW_CORE_LIBRARY

INCLUDEPATH += ./../..

SOURCES += \
    exception.cpp \
    json.cpp

HEADERS += defs.hpp \
    exception.hpp \
    json.hpp

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEC5D0D1B
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = core.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
