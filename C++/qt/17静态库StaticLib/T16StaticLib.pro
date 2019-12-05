#-------------------------------------------------
#
# Project created by QtCreator 2014-12-13T16:58:16
#
#-------------------------------------------------

QT       -= gui

TARGET = T16StaticLib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += t16staticlib.cpp

HEADERS += t16staticlib.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
