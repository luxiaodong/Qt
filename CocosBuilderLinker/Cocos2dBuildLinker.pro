#-------------------------------------------------
#
# Project created by QtCreator 2012-12-18T11:02:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cocos2dBuildLinker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qparseccbfile.cpp \
    qcocos2ddata.cpp \
    qcreateuifile.cpp

HEADERS  += mainwindow.h \
    qparseccbfile.h \
    qcocos2ddata.h \
    qcreateuifile.h

FORMS    += mainwindow.ui
