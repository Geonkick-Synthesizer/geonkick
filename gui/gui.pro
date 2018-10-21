CONFIG+=debug
QMAKE_CXXFLAGS += -std=c++1y
TEMPLATE = app
QT = core gui widgets
TARGET = geonkick
INCLUDEPATH += .
INCLUDEPATH += ../api/src/

LIBS += -L../api/ -lgeonkick -lsndfile -ljack

# Input
HEADERS += globals.h \
           geonkick_theme.h \
           geonkick_widget.h \
           geonkick_label.h \
           geonkick_button.h \
           geonkick_checkbox.h \
           geonkick_slider.h \
           geonkick_groupbox.h \
           effects_group_box.h \
           compressor_group_box.h \
           distortion_group_box.h \
           top_bar.h \
           control_area.h \
           envelope_draw_area.h \
           fader.h \
           general_envelope.h \
           general_group_box.h \
           envelope.h \
           kick_graph.h \
           envelope_widget.h \
           knob.h \
           oscillator.h \
           geonkick_api.h \
           oscillator_envelope.h \
           oscillator_group_box.h \
           export_widget.h \
           mainwindow.h

SOURCES += geonkick_theme.cpp \
           geonkick_widget.cpp \
           geonkick_label.cpp \
           geonkick_slider.cpp \
           geonkick_button.cpp \
           geonkick_checkbox.cpp \
           geonkick_groupbox.cpp \
           top_bar.cpp \
           control_area.cpp \
           envelope_draw_area.cpp \
           fader.cpp \
           general_envelope.cpp \
           effects_group_box.cpp \
           compressor_group_box.cpp \
           distortion_group_box.cpp \
           general_group_box.cpp \
           envelope.cpp \
           kick_graph.cpp \
           envelope_widget.cpp \
           knob.cpp \
           oscillator.cpp \
           geonkick_api.cpp \
           oscillator_envelope.cpp \
           oscillator_group_box.cpp \
           export_widget.cpp \
           mainwindow.cpp \
           main.cpp
