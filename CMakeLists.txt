cmake_minimum_required (VERSION 3.7)

project(geonkick VERSION 1.1.0)

# For Widnows. Temporary.
#set(CMAKE_SYSTEM_NAME Windows)
#set(CMAKE_SYSTEM_VERSION 1)
#set(GKICK_VST_SDK_PATH C:/VST_SDK/VST3_SDK)
#set(GKICK_REDKITE_SDK_PATH C:/redkite)

option(GKICK_STANDALONE "Enable build for standalone" ON)
option(GKICK_PLUGIN "Enable build for plugin" ON)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "no build type selected, set default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type Release" FORCE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS} -fvisibility=hidden")

#if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#  set (CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "default install path" FORCE)
#endif()

set(GKICK_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/api)
set(GKICK_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GKICK_RC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/resources)
set(GKICK_STANDALONE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/standalone)

if (GKICK_PLUGIN)
  if (CMAKE_SYSTEM_NAME MATCHES Windows)
      if (GKICK_VST_SDK_PATH)
          list(APPEND CMAKE_MODULE_PATH "${GKICK_VST_SDK_PATH}/cmake/modules")
          include(Global)
          include(AddVST3Library)
      else(GKICK_VST_SDK_PATH)
          message(FATAL " GKICK_VST_SDK_PATH was not set")
      endif(GKICK_VST_SDK_PATH)
      set(GKICK_VST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/plugin/vst)
  else()
      set(GKICK_LV2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/plugin/lv2)
  endif()
endif(GKICK_PLUGIN)

set(GKICK_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

#set(GKICK_API_FLAGS "")
#set(GKICK_API_FLAGS_LV2 "")

set(GKICK_COMMON_HEADERS
  ${GKICK_COMMON_DIR}/geonkick_slider.h
  ${GKICK_COMMON_DIR}/geonkick_button.h
  ${GKICK_COMMON_DIR}/file_dialog.h
  ${GKICK_COMMON_DIR}/geonkick_groupbox.h
  ${GKICK_COMMON_DIR}/top_bar.h
  ${GKICK_COMMON_DIR}/control_area.h
  ${GKICK_COMMON_DIR}/envelope_draw_area.h
  ${GKICK_COMMON_DIR}/limiter.h
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
  ${GKICK_COMMON_DIR}/about.h
  ${GKICK_COMMON_DIR}/export_widget.h
  ${GKICK_COMMON_DIR}/mainwindow.h)

set(GKICK_COMMON_SOURCES
  ${GKICK_COMMON_DIR}/geonkick_widget.cpp
  ${GKICK_COMMON_DIR}/geonkick_slider.cpp
  ${GKICK_COMMON_DIR}/geonkick_button.cpp
  ${GKICK_COMMON_DIR}/file_dialog.cpp
  ${GKICK_COMMON_DIR}/geonkick_groupbox.cpp
  ${GKICK_COMMON_DIR}/top_bar.cpp
  ${GKICK_COMMON_DIR}/control_area.cpp
  ${GKICK_COMMON_DIR}/envelope_draw_area.cpp
  ${GKICK_COMMON_DIR}/limiter.cpp
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
  ${GKICK_COMMON_DIR}/about.cpp
  ${GKICK_COMMON_DIR}/mainwindow.cpp)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Enable application logging")
    option(ENABLE_LOGGING    "Enable logging for API and GUI" ON)
endif()

# Enables the double precision calculation for API.
# By default is single point precision.
option(ENABLE_DOUBLE_PRECISION "Enable double precision for API" OFF)

if (ENABLE_DOUBLE_PRECISION)
#  list(APPEND GKICK_API_FLAGS "-DGEONKICK_DOUBLE_PRECISION")
#  list(APPEND GKICK_API_FLAGS_LV2 "-DGEONKICK_DOUBLE_PRECISION")
endif()

if (ENABLE_LOGGING)
message(STATUS "set debug level")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_LOG_DEBUG_LEVEL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_LOG_DEBUG_LEVEL")
  list(APPEND GKICK_API_FLAGS "-DGKICK_LIB_LOG_LEVEL_DEBUG")
  list(APPEND GKICK_API_FLAGS "-DGKICK_LIB_LOG_FUNCTION")
  list(APPEND GKICK_API_FLAGS_LV2 "-DGKICK_LIB_LOG_LEVEL_DEBUG")
  list(APPEND GKICK_API_FLAGS_LV2 "-DGKICK_LIB_LOG_FUNCTION")
endif()

#find_package(redkite REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE sndfile)
if (NOT SNDFILE_FOUND)
    message(FATAL_ERROR "can't find sndfile library")
endif(NOT SNDFILE_FOUND)

if (GKICK_STANDALONE)
  pkg_check_modules(JACK jack)
  if (NOT JACK_FOUND)
    message(FATAL_ERROR "can't find jack library")
  endif(NOT JACK_FOUND)
  list(APPEND GKICK_API_FLAGS "-DGEONKICK_AUDIO_JACK")
endif()

if (GKICK_PLUGIN)
  if(CMAKE_SYSTEM_NAME MATCHES Windows)
	message(STATUS "Checking for VST SDK...")
  else()
	message(STATUS "Checking for LV2 library...")
	pkg_check_modules(LV2 lv2)
	if (NOT LV2_FOUND)
		message(FATAL_ERROR "can't find LV2 library")
	endif(NOT LV2_FOUND)
  endif()
endif(GKICK_PLUGIN)

# Check RekKite GUI Toolkit.
# TODO...
if (CMAKE_SYSTEM_NAME MATCHES Windows)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_OS_WIN")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_OS_WIN")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_FOR_SHARED")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_OS_GNU")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_OS_GNU")
endif ()

include_directories(${GKICK_API_DIR}/src)

# Define custom target to build the geonkick api for standalone.
if (GKICK_STANDALONE)
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/api/libgeonkick.a
    COMMAND mkdir -p api
    && cd ${GKICK_API_DIR}
    && $(MAKE) DEFINES="${GKICK_API_FLAGS}" BUILD="${CMAKE_BINARY_DIR}/api" TARGET="libgeonkick.a")
  add_custom_target(api_standalone DEPENDS ${CMAKE_BINARY_DIR}/api/libgeonkick.a)
endif(GKICK_STANDALONE)

if (GKICK_PLUGIN)
  if(CMAKE_SYSTEM_NAME MATCHES Windows)
	message(STATUS "not implemented yet")
  else()
	add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/api_lv2/libgeonkick_lv2.a
		COMMAND mkdir -p api_lv2
		&& cd ${GKICK_API_DIR}
		&& $(MAKE) DEFINES="${GKICK_API_FLAGS_LV2}" BUILD="${CMAKE_BINARY_DIR}/api_lv2" TARGET="libgeonkick_lv2.a")
	add_custom_target(api_lv2 DEPENDS ${CMAKE_BINARY_DIR}/api_lv2/libgeonkick_lv2.a)
  endif()
endif(GKICK_PLUGIN)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
    include_directories(${GKICK_VST_SDK_PATH})
	include_directories(${GKICK_REDKITE_SDK_PATH}/include)
	link_directories(${GKICK_VST_SDK_PATH}/build/lib/Debug)
	link_directories(${GKICK_REDKITE_SDK_PATH}/lib)
else ()
	add_definitions(-fPIC)
    include_directories(/usr/local/include/redkite)
endif()

include(${GKICK_RC_DIR}/resources.rc)

foreach(pngFile ${gkick_rc_png_files})
  message(STATUS "convert file ${pngFile}")
  get_filename_component(filename ${GKICK_RC_DIR}/${pngFile} NAME)
  string(REPLACE "." "_" filename ${filename})
  string(REPLACE " " "_" filename ${filename})
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/resources/${filename}.c
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/resources
    && rkpng2c "${GKICK_RC_DIR}/${pngFile}" "${CMAKE_BINARY_DIR}/resources/${filename}.c" "rk_${filename}")
    set(GKICK_RC_OUTPUT ${GKICK_RC_OUTPUT} ${CMAKE_BINARY_DIR}/resources/${filename}.c)
endforeach()

add_custom_target(gkick_resources ALL DEPENDS ${GKICK_RC_OUTPUT})

include_directories(${GKICK_COMMON_DIR})
add_library(geonkick_common STATIC
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_SOURCES}
  ${GKICK_RC_OUTPUT})
add_dependencies(geonkick_common gkick_resources)

if (GKICK_STANDALONE)
  add_subdirectory(standalone)
endif (GKICK_STANDALONE)

if (GKICK_PLUGIN)
	add_subdirectory(plugin)
endif (GKICK_PLUGIN)

add_subdirectory(data)
