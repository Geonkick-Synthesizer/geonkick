QMAKE_CXXFLAGS += -std=c++1y
TEMPLATE = app
QT = core gui widgets
TARGET = geonkick
INCLUDEPATH += .
INCLUDEPATH += ../src/
INCLUDEPATH += ../api/src/

LIBS += -L../api/ -lgeonkick -lsndfile -ljack

include(../src/gui.pri)

SOURCES += main.cpp
