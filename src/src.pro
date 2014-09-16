#-------------------------------------------------
#
# Project created by QtCreator 2014-09-12T22:25:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qnote
TEMPLATE = lib

DEFINES += SRC_LIBRARY

SOURCES += fileviewmodel.cpp

HEADERS += fileviewmodel.h \
           src_global.h \
        
unix {
    target.path = /usr/lib
    INSTALLS += target
}
