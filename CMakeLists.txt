cmake_minimum_required (VERSION 3.7)

project(geonkick VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(GKICK_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/api)
set(GKICK_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GKICK_STANDALONE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/standalone)
set(GKICK_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

set(GKICK_API_FLAGS "")

set(GKICK_COMMON_HEADERS
  ${GKICK_COMMON_DIR}/geonkick_widget.h
  ${GKICK_COMMON_DIR}/geonkick_label.h
  ${GKICK_COMMON_DIR}/geonkick_slider.h
  ${GKICK_COMMON_DIR}/geonkick_button.h
  ${GKICK_COMMON_DIR}/geonkick_checkbox.h
  ${GKICK_COMMON_DIR}/geonkick_groupbox.h
  ${GKICK_COMMON_DIR}/top_bar.h
  ${GKICK_COMMON_DIR}/control_area.h
  ${GKICK_COMMON_DIR}/envelope_draw_area.h
  ${GKICK_COMMON_DIR}/fader.h
  ${GKICK_COMMON_DIR}/general_envelope.h
  ${GKICK_COMMON_DIR}/effects_group_box.h
  ${GKICK_COMMON_DIR}/compressor_group_box.h
  ${GKICK_COMMON_DIR}/distortion_group_box.h
  ${GKICK_COMMON_DIR}/general_group_box.h
  ${GKICK_COMMON_DIR}/envelope.h
  ${GKICK_COMMON_DIR}/kick_graph.h
  ${GKICK_COMMON_DIR}/envelope_widget.h
  ${GKICK_COMMON_DIR}/knob.h
  ${GKICK_COMMON_DIR}/oscillator.h
  ${GKICK_COMMON_DIR}/geonkick_api.h
  ${GKICK_COMMON_DIR}/geonkick_state.h
  ${GKICK_COMMON_DIR}/oscillator_envelope.h
  ${GKICK_COMMON_DIR}/oscillator_group_box.h
  ${GKICK_COMMON_DIR}/export_widget.h
  ${GKICK_COMMON_DIR}/mainwindow.h)

set(GKICK_COMMON_SOURCES
  ${GKICK_COMMON_DIR}/geonkick_widget.cpp
  ${GKICK_COMMON_DIR}/geonkick_label.cpp
  ${GKICK_COMMON_DIR}/geonkick_slider.cpp
  ${GKICK_COMMON_DIR}/geonkick_button.cpp
  ${GKICK_COMMON_DIR}/geonkick_checkbox.cpp
  ${GKICK_COMMON_DIR}/geonkick_groupbox.cpp
  ${GKICK_COMMON_DIR}/top_bar.cpp
  ${GKICK_COMMON_DIR}/control_area.cpp
  ${GKICK_COMMON_DIR}/envelope_draw_area.cpp
  ${GKICK_COMMON_DIR}/fader.cpp
  ${GKICK_COMMON_DIR}/general_envelope.cpp
  ${GKICK_COMMON_DIR}/effects_group_box.cpp
  ${GKICK_COMMON_DIR}/compressor_group_box.cpp
  ${GKICK_COMMON_DIR}/distortion_group_box.cpp
  ${GKICK_COMMON_DIR}/general_group_box.cpp
  ${GKICK_COMMON_DIR}/envelope.cpp
  ${GKICK_COMMON_DIR}/kick_graph.cpp
  ${GKICK_COMMON_DIR}/envelope_widget.cpp
  ${GKICK_COMMON_DIR}/knob.cpp
  ${GKICK_COMMON_DIR}/oscillator.cpp
  ${GKICK_COMMON_DIR}/geonkick_api.cpp
  ${GKICK_COMMON_DIR}/geonkick_state.cpp
  ${GKICK_COMMON_DIR}/oscillator_envelope.cpp
  ${GKICK_COMMON_DIR}/oscillator_group_box.cpp
  ${GKICK_COMMON_DIR}/export_widget.cpp
  ${GKICK_COMMON_DIR}/mainwindow.cpp)

set(GKICK_COMMON_RESOURCES ${GKICK_COMMON_DIR}/resources/resources.qrc)

option(ENABLE_LOGGING    "Enable logging for API and GUI" ON)

# Enables the double precision calculation for API.
# By default is single point precision.
option(ENABLE_DOUBLE_PRECISION "Enable double precision for API" OFF)

if (ENABLE_DOUBLE_PRECISION)
  list(APPEND GKICK_API_FLAGS "-DGEONKICK_DOUBLE_PRECISION")
endif()

if (ENABLE_LOGGING)
  set(DEFAULT_LOG_LEVEL "GEONKICK_LOG_LEVEL_DEBUG")
  list(APPEND GKICK_API_FLAGS "-D${DEFAULT_LOG_LEVEL} -DGKICK_LIB_LOG_FUNCTION")
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE sndfile)
if (NOT SNDFILE_FOUND)
    message(FATAL_ERROR "can't find sndfile library")
endif(NOT SNDFILE_FOUND)

pkg_check_modules(JACK jack)
  if (NOT JACK_FOUND)
    message(FATAL_ERROR "can't find jack library")
  endif(NOT JACK_FOUND)
list(APPEND GKICK_API_FLAGS "-DGEONKICK_AUDIO_JACK")

include_directories(${GKICK_API_DIR}/src)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/api/libgeonkick.a
    COMMAND mkdir -p api
    && cd ${GKICK_API_DIR}
    && $(MAKE) DEFINES="${GKICK_API_FLAGS}" BUILD="${CMAKE_BINARY_DIR}/api" TARGET="libgeonkick.a")
add_custom_target(api_standalone DEPENDS ${CMAKE_BINARY_DIR}/api/libgeonkick.a)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_definitions(-fPIC)
include_directories(${Qt5Core_INCLUDES}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Gui_INCLUDES}
  ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDES}
  ${Qt5Widgets_INCLUDE_DIRS})

include_directories(${GKICK_COMMON_DIR})
add_library(geonkick_common STATIC
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_SOURCES})

add_subdirectory(standalone)
add_subdirectory(data)
