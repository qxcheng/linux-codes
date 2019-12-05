#-------------------------------------------------
#
# Project created by QtCreator 2014-12-13T16:45:29
#
#-------------------------------------------------

QT       -= gui

TARGET = T14Library
TEMPLATE = lib

DEFINES += T14LIBRARY_LIBRARY

SOURCES += t14library.cpp

HEADERS += t14library.h\
        t14library_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
