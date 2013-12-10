#-------------------------------------------------
#
# Project created by QtCreator 2013-12-10T16:02:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QEncryption
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qlfsr.cpp \
    qrsa.cpp \
    qecc.cpp

HEADERS  += mainwindow.h \
    qlfsr.h \
    qrsa.h \
    qecc.h

FORMS    += mainwindow.ui
