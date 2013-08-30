#-------------------------------------------------
#
# Project created by QtCreator 2012-01-21T17:20:56
#
#-------------------------------------------------

QT       += core gui

TARGET = GMath
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    numberTheory/GNumberTheory.cpp \
    linearAlgebra/GVector.cpp \
    linearAlgebra/GMatrix.cpp \
    combinatorial/GSpecialSeriesOfNumbers.cpp

HEADERS  += mainwindow.h \
    numberTheory/GNumberTheory.h \
    linearAlgebra/GVector.h \
    linearAlgebra/GMatrix.h \
    combinatorial/GSpecialSeriesOfNumbers.h

FORMS    += mainwindow.ui
