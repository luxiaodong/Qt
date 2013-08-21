#-------------------------------------------------
#
# Project created by QtCreator 2011-08-05T21:16:29
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = cocos2dUIDesign
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qgraphicslayerscene.cpp \
    qentername.cpp \
    qsizeselect.cpp \
    qexportsourcecode.cpp

HEADERS  += mainwindow.h \
    qgraphicslayerscene.h \
    qentername.h \
    qsizeselect.h \
    qexportsourcecode.h

FORMS    += mainwindow.ui \
    qentername.ui \
    qsizeselect.ui

include(extension/extension.pri)
include(qtpropertybrowser/qtpropertybrowser.pri)
