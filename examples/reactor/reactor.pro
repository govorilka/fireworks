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
    database.cpp \
    databaseview.cpp \
    datatype.cpp \
    datanode.cpp \
    dataedit.cpp \
    selectdatatypedialog.cpp

HEADERS  += mainwindow.h \
    database.h \
    databaseview.h \
    datatype.h \
    datanode.h \
    dataedit.h \
    selectdatatypedialog.h

FORMS    += mainwindow.ui \
    databaseview.ui \
    selectdatatypedialog.ui \
    dataeditroot.ui \
    dataeditquestion.ui

RESOURCES += \
    reactor.qrc

OTHER_FILES += \
    sql/init.sql

 TRANSLATIONS = reactor_ru.ts
