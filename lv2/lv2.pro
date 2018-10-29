CONFIG+=debug
#QMAKE_CXXFLAGS += -std=c++1y
TEMPLATE = lib
QT += widgets
TARGET = geonkick_lv2
INCLUDEPATH += .

CONFIG += shared plugin

#LIBS += -L../api/ -lgeonkick -lsndfile -ljack

HEADERS += geonkick_lv2.h
SOURCES += geonkick_lv2_ui.cpp


# Input
