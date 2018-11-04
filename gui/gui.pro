CONFIG+=debug
QMAKE_CXXFLAGS += -std=c++1y
TEMPLATE = app
QT = core gui widgets
TARGET = geonkick
INCLUDEPATH += .
INCLUDEPATH += ../api/src/

include(gui.pri)

SOURCES += main.cpp

LIBS += -L../api/ -lgeonkick -lsndfile -ljack

