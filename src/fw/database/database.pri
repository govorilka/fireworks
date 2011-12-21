HEADERS += \
    fw/database/constructor.hpp \
    fw/database/defs.hpp \
    fw/database/controller.hpp \
    fw/database/query.hpp \
    fw/database/query_inl.hpp \
    fw/database/driver.hpp

SOURCES += \
    fw/database/constructor.cpp \
    fw/database/controller.cpp \
    fw/database/query.cpp \
    fw/database/driver.cpp

include(sqlite/sqlite.pri)

exists($(PGSQL_PATH)) {
    include(postgresql/postgresql.pri)
}
else {
    warning("Postgresql directory is not exist")
}













































