QMAKE_CXXFLAGS += -std=c++1y
TEMPLATE = lib
QT += widgets
TARGET = geonkick_lv2
INCLUDEPATH += .
INCLUDEPATH += ../src/
INCLUDEPATH += ../api/src/
DEFINES += GEONKICK_LV2_PLUGIN

CONFIG += shared plugin

LIBS += -L../api/ -lgeonkick -lsndfile

include(../src/gui.pri)

SOURCES += geonkick_lv2.cpp

