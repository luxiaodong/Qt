#-------------------------------------------------
#
# Project created by QtCreator 2013-08-30T09:59:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QProtocolStructParser
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    qtcptestdialog.cpp \
    qtcpstruct.cpp

HEADERS  += MainWindow.h \
    qtcptestdialog.h \
    qtcpstruct.h

FORMS    += MainWindow.ui \
    qtcptestdialog.ui

include(extension/extension.pri);
include(qtpropertybrowser/qtpropertybrowser.pri);
