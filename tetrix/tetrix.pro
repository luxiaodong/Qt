QT += widgets

HEADERS       = tetrixboard.h \
                tetrixpiece.h \
                tetrixwindow.h \
    tetrixnature.h
SOURCES       = main.cpp \
                tetrixboard.cpp \
                tetrixpiece.cpp \
                tetrixwindow.cpp \
    tetrixnature.cpp

# install
INSTALLS += target
