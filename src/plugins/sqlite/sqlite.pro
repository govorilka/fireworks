#-------------------------------------------------
#
# Project created by QtCreator 2012-01-23T14:00:33
#
#-------------------------------------------------

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = lib

win32 {
   TARGET = ../../../../bin/plugins/sqlite
}
else {
   TARGET = ../../../bin/plugins/sqlite
}

INCLUDEPATH += ./ \

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4

HEADERS += \
    query.hpp \
    driver.hpp \
    defs.hpp \
    plugin.hpp

SOURCES += \
    query.cpp \
    driver.cpp \
    plugin.cpp

include(sqlite3.pri)
