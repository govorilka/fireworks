HEADERS += \
    fw/database/constructor.hpp \
    fw/database/defs.hpp \
    fw/database/controller.hpp \
    fw/database/query.hpp \
    fw/database/query_inl.hpp \
    fw/database/driver.hpp \
    fw/database/factory.hpp \
    fw/database/sqlite/defs.hpp \
    fw/database/sqlite/driver.hpp \
    thirdparty/sqlite/sqlite3ext.h \
    thirdparty/sqlite/sqlite3.h \
    fw/database/sqlite/querydata.hpp

SOURCES += \
    fw/database/constructor.cpp \
    fw/database/controller.cpp \
    fw/database/query.cpp \
    fw/database/driver.cpp \
    fw/database/factory.cpp \
    thirdparty/sqlite/sqlite3.c \
    fw/database/sqlite/sqlite_driver.cpp \
    fw/database/sqlite/sqlite_querydata.cpp


include(postgresql/postgresql.pri)




































