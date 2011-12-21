HEADERS += \
    fw/database/sqlite/defs.hpp \
    fw/database/sqlite/driver_sqlite.hpp \
    fw/database/sqlite/query_sqlite.hpp \
    thirdparty/sqlite/sqlite3ext.h \
    thirdparty/sqlite/sqlite3.h

SOURCES += \
    thirdparty/sqlite/sqlite3.c \
    fw/database/sqlite/driver_sqlite.cpp \
    fw/database/sqlite/query_sqlite.cpp
