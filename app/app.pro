#-------------------------------------------------
#
# Project created by QtCreator 2014-09-02T16:07:14
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += qt

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lsrc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lsrc
else:unix: LIBS += -L$$OUT_PWD/../src/ -lsrc

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src