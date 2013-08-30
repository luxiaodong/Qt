#-------------------------------------------------
#
# Project created by QtCreator 2010-10-10T01:30:35
#
#-------------------------------------------------

QT       += core gui
QT       += phonon

TARGET = QWaveAnalyse
TEMPLATE = app


SOURCES += main.cpp\
        qwaveanalyse.cpp \
    decmpa/decodefile.cpp \
    decmpa/src/MPADecoder.cpp \
    decmpa/src/DecMPAFileAccess.cpp \
    decmpa/src/DecMPA.cpp \
    decmpa/src/MPAFrameFinder.cpp \
    decmpa/src/MPAInfo.cpp \
    decmpa/src/frame/rawDataBuffer.cpp \
    decmpa/src/frame/pcmFrame.cpp \
    decmpa/src/frame/frame.cpp \
    decmpa/src/frame/floatFrame.cpp \
    decmpa/src/frame/audioFrame.cpp \
    decmpa/src/hip/DecodeEngine_Hip.cpp \
    decmpa/src/hip/VbrTag.c \
    decmpa/src/hip/tabinit.c \
    decmpa/src/hip/layer3.c \
    decmpa/src/hip/layer2.c \
    decmpa/src/hip/layer1.c \
    decmpa/src/hip/interface.c \
    decmpa/src/hip/decode_i386.c \
    decmpa/src/hip/dct64_i386.c \
    decmpa/src/hip/common.c \
    decmpa/src/mpegAudioFrame/mpegAudioHeader.cpp \
    decmpa/src/mpegAudioFrame/dxHead.cpp \
    decmpa/src/splay/window.cpp \
    decmpa/src/splay/synthesis.cpp \
    decmpa/src/splay/synth_Std.cpp \
    decmpa/src/splay/synth_filter.cpp \
    decmpa/src/splay/synth_Down.cpp \
    decmpa/src/splay/splayDecoder.cpp \
    decmpa/src/splay/mpegtoraw.cpp \
    decmpa/src/splay/mpegtable.cpp \
    decmpa/src/splay/mpeglayer3.cpp \
    decmpa/src/splay/mpeglayer2.cpp \
    decmpa/src/splay/mpeglayer1.cpp \
    decmpa/src/splay/mpegAudioStream.cpp \
    decmpa/src/splay/mpegAudioBitWindow.cpp \
    decmpa/src/splay/huffmantable.cpp \
    decmpa/src/splay/huffmanlookup.cpp \
    decmpa/src/splay/dct64_down.cpp \
    decmpa/src/splay/dct64.cpp \
    decmpa/src/splay/dct36_12.cpp

HEADERS  += qwaveanalyse.h \
    decmpa/include/decmpa.h \
    decmpa/src/MPAInfo.h \
    decmpa/src/MPAFrameFinder.h \
    decmpa/src/MPADecoder.h \
    decmpa/src/MemBuffer.h \
    decmpa/src/IFileAccess.h \
    decmpa/src/DefInc.h \
    decmpa/src/DecodeEngine.h \
    decmpa/src/DecMPAFileAccess.h \
    decmpa/src/frame/rawDataBuffer.h \
    decmpa/src/frame/pcmFrame.h \
    decmpa/src/frame/frame.h \
    decmpa/src/frame/floatFrame.h \
    decmpa/src/frame/audioFrame.h \
    decmpa/src/hip/VbrTag.h \
    decmpa/src/hip/tabinit.h \
    decmpa/src/hip/mpglib.h \
    decmpa/src/hip/mpg123.h \
    decmpa/src/hip/layer3.h \
    decmpa/src/hip/layer2.h \
    decmpa/src/hip/layer1.h \
    decmpa/src/hip/l2tables.h \
    decmpa/src/hip/interface.h \
    decmpa/src/hip/huffman.h \
    decmpa/src/hip/HIPDefines.h \
    decmpa/src/hip/decode_i386.h \
    decmpa/src/hip/dct64_i386.h \
    decmpa/src/hip/common.h \
    decmpa/src/mpegAudioFrame/mpegAudioHeader.h \
    decmpa/src/mpegAudioFrame/dxHead.h \
    decmpa/src/splay/window.h \
    decmpa/src/splay/synthesis.h \
    decmpa/src/splay/splayDecoder.h \
    decmpa/src/splay/op.h \
    decmpa/src/splay/mpegsound.h \
    decmpa/src/splay/mpegAudioStream.h \
    decmpa/src/splay/mpegAudioBitWindow.h \
    decmpa/src/splay/mpeg2tables.h \
    decmpa/src/splay/huffmanlookup.h \
    decmpa/src/splay/dct.h \
    decmpa/src/splay/common.h \
    decmpa/src/splay/attribute.h
