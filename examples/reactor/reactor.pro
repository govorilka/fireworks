#-------------------------------------------------
#
# Project created by QtCreator 2011-03-09T22:08:31
#
#-------------------------------------------------

QT       += core gui

win32 {
    TARGET = ../../../bin/reactor
}
else {
    TARGET = ../../bin/reactor
}

TEMPLATE = app

LIBS += -L../../bin \
        -lfireworks

INCLUDEPATH += ../../src/

SOURCES += main.cpp\
           mainwindow.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    database.h

FORMS    += mainwindow.ui

RESOURCES += \
    reactor.qrc

OTHER_FILES += \
    sql/init.sql
