CONFIG += debug
QMAKE_CXXFLAGS += -std=c++1y
TEMPLATE = lib
QT += widgets
TARGET = geonkick_lv2
INCLUDEPATH += .
INCLUDEPATH += ../api/src/

CONFIG += shared plugin

LIBS += -L../api/ -lgeonkick -lsndfile -ljack

#include(gui.pri)

SOURCES += geonkick_lv2.cpp

