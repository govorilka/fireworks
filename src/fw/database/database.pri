HEADERS += \
    fw/database/constructor.hpp \
    fw/database/defs.hpp \
    fw/database/controller.hpp \
    fw/database/query.hpp \
    fw/database/query_inl.hpp \
    fw/database/driver.hpp \
    fw/database/sqlite/defs.hpp \
    fw/database/sqlite/driver.hpp \
    fw/database/sqlite/querydata.hpp \
    thirdparty/sqlite/sqlite3ext.h \
    thirdparty/sqlite/sqlite3.h \
    temp.hpp

SOURCES += \
    fw/database/constructor.cpp \
    fw/database/controller.cpp \
    fw/database/query.cpp \
    fw/database/driver.cpp \
    thirdparty/sqlite/sqlite3.c \
    fw/database/sqlite/sqlite_driver.cpp \
    fw/database/sqlite/sqlite_querydata.cpp


exists($(PGSQL_PATH)) {
    include(postgresql/postgresql.pri)
}
else {
    warning("Postgresql directory is not exist")
}











































