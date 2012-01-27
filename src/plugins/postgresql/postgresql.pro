#-------------------------------------------------
#
# Project created by QtCreator 2012-01-23T14:02:18
#
#-------------------------------------------------

QT       += core
QT       -= gui

TEMPLATE = lib

win32 {
   TARGET = ../../../../bin/plugins/postgresql
}
else {
   TARGET = ../../../bin/plugins/postgresql
}

INCLUDEPATH += ./ \

#Fireworks library
INCLUDEPATH += $(FIREWORKS_PATH)/src
LIBS += -L$(FIREWORKS_PATH)/bin -lfireworks4

exists($(PGSQL_INCLUDE_PATH)) {
    message( "External path: $(PGSQL_INCLUDE_PATH)" )
}
else {
    error("Postgresql include directory is not exist")
}

exists($(PGSQL_LIB_PATH)) {
    message( "External path: $(PGSQL_LIB_PATH)" )
}
else {
    error("Postgresql lib directory is not exist")
}

#//PostgreSQL
INCLUDEPATH += $(PGSQL_INCLUDE_PATH)
LIBS += -L$(PGSQL_LIB_PATH) -lpq


HEADERS += \
    defs.hpp \
    driver.hpp \
    query.hpp \
    plugin.hpp

SOURCES += \
    driver.cpp\
    query.cpp \
    plugin.cpp
