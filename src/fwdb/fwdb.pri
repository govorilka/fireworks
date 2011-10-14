
HEADERS += \
    fwdb/sqlite/sqlite3.h \
    fwdb/fwsqlite.h \
    fwdb/fwdb.h \
    fwdb/dbfactory.h

SOURCES += \
    fwdb/sqlite/sqlite3.c \
    fwdb/fwsqlite.cpp \
    fwdb/fwdb.cpp \
    fwdb/dbfactory.cpp

exists($(PGSQL_PATH)) {
    include (postgresql.pri)
    HEADERS += fwdb/fwpg.h
    SOURCES += fwdb/fwpg.cpp
}
else {
    warning("Postgresql directory is not exist")
}



