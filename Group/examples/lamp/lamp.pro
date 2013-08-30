# -------------------------------------------------
# Project created by QtCreator 2010-09-05T16:34:49
# -------------------------------------------------
QT += core \
    gui
TARGET = lamp
TEMPLATE = app
SOURCES += main.cpp \
    qlamp.cpp \
    ../../kernel/qabstractgroup.cpp \
    ../../kernel/qquaterniongroup.cpp \
    ../../kernel/qpermutegroup.cpp \
    ../../kernel/qabstractcyclicgroup.cpp \
    ../../kernel/qcongruencegroup.cpp \
    ../../kernel/qabstractdihedralgroup.cpp \
    ../../model/qbinarymodel.cpp \
    ../../kernel/qmath.cpp
HEADERS += qlamp.h \
    ../../kernel/qabstractgroup.h \
    ../../kernel/define.h \
    ../../kernel/depend/gmatrix.h \
    ../../kernel/depend/glist.h \
    ../../kernel/qquaterniongroup.h \
    ../../kernel/qpermutegroup.h \
    ../../kernel/qabstractcyclicgroup.h \
    ../../kernel/qcongruencegroup.h \
    ../../kernel/qabstractdihedralgroup.h \
    ../../model/qbinarymodel.h \
    ../../kernel/qmath.h \
    ../../kernel/depend/gbitarray.h
FORMS += qlamp.ui
