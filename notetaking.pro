#-------------------------------------------------
#
# Project created by QtCreator 2014-09-02T16:07:14
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = notetaking
TEMPLATE = app


SOURCES += main.cpp\
    fileviewmodel.cpp \
    qnote.cpp

HEADERS  += \
    fileviewmodel.h \
    qnote.h

FORMS    += \
    qnote.ui
