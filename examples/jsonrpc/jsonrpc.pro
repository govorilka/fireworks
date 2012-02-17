#-------------------------------------------------
#
# Project created by QtCreator 2012-02-16T14:33:21
#
#-------------------------------------------------

QT       += core gui

TARGET = jsonrpc
TEMPLATE = app

INCLUDEPATH += ../../src/

win32 {
    LIBS += -L../../../lib \
            -lfwcore1 \
            -lfwjsonrpc1
}
else {
    LIBS += -L../../lib \
            -lfwcore1 \
            -lfwjsonrpc1
}

SOURCES += main.cpp\
           mainwindow.cpp \
           mainform.cpp

HEADERS  += mainwindow.hpp \
            mainform.hpp

FORMS    += mainwindow.ui \
            mainform.ui
