
DEFINES += PLUGIN_LIBRARY

HEADERS += \
    fw/database/defs.hpp \
    fw/database/controller.hpp \
    fw/database/query.hpp \
    fw/database/query_inl.hpp \
    fw/database/driver.hpp \
    fw/database/plugin.hpp

SOURCES += \
    fw/database/controller.cpp \
    fw/database/query.cpp \
    fw/database/driver.cpp \
    fw/database/plugin.cpp


symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3E58FC0
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = plugin.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}














































