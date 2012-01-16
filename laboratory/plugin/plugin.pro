#-------------------------------------------------
#
# Project created by QtCreator 2012-01-12T16:38:46
#
#-------------------------------------------------

QT       -= gui

TARGET = plugin
TEMPLATE = lib

DEFINES += PLUGIN_LIBRARY

win32 {
    TARGET = ../../../bin/plugins/sqlite3
}
else {
    TARGET = ../../bin/plugins/sqlite3
}


#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin \
        -lfireworks4

SOURCES += plugin.cpp \
           sqlite3plugin.cpp

HEADERS += plugin.hpp\
        plugin_global.hpp \
        sqlite3plugin.hpp

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3E58FC0
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = plugin.dll
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
