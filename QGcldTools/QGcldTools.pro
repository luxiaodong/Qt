#-------------------------------------------------
#
# Project created by QtCreator 2013-07-08T22:27:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGcldTools
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qconvertdialogure.cpp \
    qbezierpoly.cpp \
    qcityroadmap.cpp \
    qcityposition.cpp \
    qcubepoly.cpp \
    qpolytime.cpp \
    qcollectxiaoqiandialogure.cpp \
    qbuildresourcedialog.cpp \
    qresourcediffdialog.cpp

HEADERS  += mainwindow.h \
    qconvertdialogure.h \
    qbezierpoly.h \
    qcityroadmap.h \
    qcityposition.h \
    qcubepoly.h \
    qpolytime.h \
    qcollectxiaoqiandialogure.h \
    qbuildresourcedialog.h \
    qresourcediffdialog.h

FORMS    += mainwindow.ui \
    qbuildresourcedialog.ui \
    qresourcediffdialog.ui

OTHER_FILES += \
    test.qml
