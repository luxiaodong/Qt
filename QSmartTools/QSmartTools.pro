#-------------------------------------------------
#
# Project created by QtCreator 2012-06-07T10:00:50
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = QSmartTools
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qpointtranslatedialog.cpp \
    qrenamedialog.cpp \
    qiphonetoitouchdialog.cpp \
    qiphonetoipaddialog.cpp \
    qtcptestdialog.cpp \
    qtcpstruct.cpp

HEADERS  += mainwindow.h \
    qpointtranslatedialog.h \
    qrenamedialog.h \
    qiphonetoitouchdialog.h \
    qiphonetoipaddialog.h \
    qtcptestdialog.h \
    qtcpstruct.h

FORMS    += mainwindow.ui \
    qpointtranslatedialog.ui \
    qrenamedialog.ui \
    qiphonetoitouchdialog.ui \
    qiphonetoipaddialog.ui \
    qtcptestdialog.ui

include(extension/extension.pri)
include(qtpropertybrowser/qtpropertybrowser.pri)

