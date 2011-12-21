
DEFINES += FW_SUPPORT_POSTGRESQL

unix {
    LIBS += -L/usr/lib -lpq
}

message( "External path: $(PGSQL_PATH)" )

win32 {
    INCLUDEPATH += $(PGSQL_PATH)/include
    LIBS += -L$(PGSQL_PATH)/lib -lpq
}

HEADERS += \
    fw/database/postgresql/defs.hpp \
    fw/database/postgresql/driver_postgresql.hpp \
    fw/database/postgresql/query_postgresql.hpp \

SOURCES += \
    fw/database/postgresql/driver_postgresql.cpp\
    fw/database/postgresql/query_postgresql.cpp
