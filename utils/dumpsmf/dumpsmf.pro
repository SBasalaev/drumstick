TEMPLATE = app
TARGET = drumstick-dumpsmf
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += qt console link_prl
DESTDIR = ../../build/bin
INCLUDEPATH += . ../../library/include ../common ../../build/common
LIBS = -L../../build/lib -ldrumstick-common -ldrumstick-file
PRE_TARGETDEPS += $$OUT_PWD/../../build/lib/libdrumstick-common.a
include (../../global.pri)
# Input
HEADERS += dumpsmf.h
SOURCES += dumpsmf.cpp
