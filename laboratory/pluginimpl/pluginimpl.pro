#-------------------------------------------------
#
# Project created by QtCreator 2012-01-17T13:09:36
#
#-------------------------------------------------

QT       -= gui

TEMPLATE = lib

DEFINES += PLUGINIMPL_LIBRARY

win32 {
    TARGET = ../../../bin/plugins/pluginimpl
}
else {
    TARGET = ../../bin/plugins/pluginimpl
}

HEADERS += \
    pluginimpl.hpp

SOURCES += \
    pluginimpl.cpp

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
#LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4

#plugin library
INCLUDEPATH += $(FIREWORKS_PATH)/laboratory/plugin
LIBS += -L$(FIREWORKS_PATH)/bin/plugins -lplugin

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3E58FC0
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = pluginimpl.dll
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
