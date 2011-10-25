
DEFINES += FW_SUPPORT_POSTGRESQL

unix {
    LIBS += -L/usr/lib -lpq
}

message( "External path: $(PGSQL_PATH)" )

win32 {
    INCLUDEPATH += $(PGSQL_PATH)/include
    LIBS += -L$(PGSQL_PATH)/lib -lpq
}
