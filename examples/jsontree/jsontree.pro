QT      += core gui

TARGET = ../../bin/jsontree

TEMPLATE = app

INCLUDEPATH += ../../src/

win32 {
    LIBS += -L../../../lib \
            -lfwcore1
}
else {
    LIBS += -L../../lib \
            -lfwcore1
}

SOURCES  += main.cpp\
            mainwindow.cpp

HEADERS  += mainwindow.h

FORMS += \
    mainwindow.ui
