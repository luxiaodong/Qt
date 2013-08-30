#-------------------------------------------------
#
# Project created by QtCreator 2011-01-31T19:02:15
#
#-------------------------------------------------

QT       += core gui network

TARGET = QSha
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcard.cpp \
    qhero.cpp \
    qskill.cpp \
    qplayer.cpp \
    server/qserver.cpp \
    server/qround.cpp \
    server/qgame.cpp \
    server/qdesk.cpp \
    client/deskwidget.cpp \
    client/qclient.cpp \
    client/logindialog.cpp

HEADERS  += mainwindow.h \
    qcard.h \
    qhero.h \
    qskill.h \
    qplayer.h \
    protocol.h \
    server/qserver.h \
    server/qround.h \
    server/qgame.h \
    server/qdesk.h \
    client/deskwidget.h \
    client/qclient.h \
    client/logindialog.h \
    client/string.h

FORMS    += mainwindow.ui \
    client/deskwidget.ui \
    client/logindialog.ui

TRANSLATIONS = QSha_zh_CN.ts
