
HEADERS += \
    fwdb/sqlite/sqlite3.h \
    fwdb/fwsqlite.h \
    fwdb/fwdb.h

SOURCES += \
    fwdb/sqlite/sqlite3.c \
    fwdb/fwsqlite.cpp \
    fwdb/fwdb.cpp

postgresql {
    LIBS += -L/usr/lib -lpq
    HEADERS += fwdb/fwpg.h
    SOURCES += fwdb/fwpg.cpp
}



