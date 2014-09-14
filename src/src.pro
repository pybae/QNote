#-------------------------------------------------
#
# Project created by QtCreator 2014-09-12T22:25:14
#
#-------------------------------------------------

QT       += core gui declarative

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qnote
TEMPLATE = lib

DEFINES += SRC_LIBRARY

SOURCES += qnote.cpp \
    fileviewmodel.cpp

HEADERS += qnote.h\
        src_global.h \
        fileviewmodel.h

FORMS += qnote.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}
