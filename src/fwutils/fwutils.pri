
HEADERS += \
    fwutils/fwnetworkconfig.h \
    fwutils/fwsystem.h \
    fwutils/fwscheduler.h \
    fwutils/fwscheduler_inl.h \
    fwutils/fwversion.h \
    fwutils/fwconsole.h

SOURCES += \
    fwutils/fwnetworkconfig.cpp \
    fwutils/fwnetworkconfig_linux.cpp \
    fwutils/fwnetworkconfig_win32.cpp \
    fwutils/fwsystem.cpp \
    fwutils/fwscheduler.cpp \
    fwutils/fwversion.cpp \
    fwutils/fwconsole.cpp
