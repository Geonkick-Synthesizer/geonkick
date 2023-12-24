cmake_minimum_required (VERSION 3.7)

project(geonkick VERSION 3.2.0)

math(EXPR GEONKICK_VERSION_HEX "0x${PROJECT_VERSION_MAJOR} * 0x10000 + 0x${PROJECT_VERSION_MINOR} * 0x100 + 0x${PROJECT_VERSION_PATCH}")
set(GEONKICK_VERSION_STR "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

message(STATUS "Geonkick version: ${GEONKICK_VERSION_STR} (${GEONKICK_VERSION_HEX})")

add_definitions(-DGEONKICK_VERSION_HEX=${GEONKICK_VERSION_HEX})
add_definitions(-DGEONKICK_VERSION_STR="${GEONKICK_VERSION_STR}")

if (NOT CMAKE_SYSTEM_NAME MATCHES Windows)
    include(GNUInstallDirs)
endif()

if (NOT CMAKE_SYSTEM_NAME MATCHES Windows)
   option(GKICK_STANDALONE "Enable build for standalone" ON)
else()
   option(GKICK_STANDALONE "Disable build for standalone" OFF)
endif()
option(GKICK_PLUGIN "Enable build for plugin" ON)
option(GKICK_PRESETS "Enable presets" ON)

if (GKICK_PLUGIN)
  option(GKICK_PLUGIN_LV2 "Enable build LV2 plugin" ON)
  if (VST3_SDK_PATH)
    option(GKICK_PLUGIN_VST "Enable build VST plugin" ON)
  endif(VST3_SDK_PATH)
endif (GKICK_PLUGIN)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "no build type selected, set default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type Release" FORCE)
endif()

if (NOT GKICK_ARCHITECTURE)
  message(STATUS "GKICK_ARCHITECTURE variable was not set use default ${CMAKE_HOST_SYSTEM_PROCESSOR}")
  set(GKICK_ARCHITECTURE ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif ()

if (GKICK_ARCHITECTURE MATCHES x86_64)
  message(STATUS "set optimisation compiler flags for ${GKICK_ARCHITECTURE}")
  set(GKICK_OPTIMISATION_FLAGS "-O3 -msse -msse2 -mfpmath=sse -ffast-math -fomit-frame-pointer")
else ()
  message(STATUS "set optimisation compiler flags for ${GKICK_ARCHITECTURE}")
  set(GKICK_OPTIMISATION_FLAGS "-O3 -ffast-math -fomit-frame-pointer")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall ${GKICK_OPTIMISATION_FLAGS} ${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_C_FLAGS "-Wall ${GKICK_OPTIMISATION_FLAGS} ${CMAKE_C_FLAGS} -fvisibility=hidden")

if (CMAKE_SYSTEM_NAME MATCHES Windows)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGEONKICK_OS_WINDOWS")
   set(CMAKE_C_FLAGS "${GKICK_OPTIMISATION_FLAGS} -DGEONKICK_OS_WINDOWS")
else()
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGEONKICK_OS_GNU")
   set(CMAKE_C_FLAGS "${GKICK_OPTIMISATION_FLAGS} -DGEONKICK_OS_GNU")
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "default install path" FORCE)
endif()

if (NOT CMAKE_INSTALL_FULL_DATADIR)
  message(STATUS "CMAKE_INSTALL_FULL_DATADIR is not set, use default ${CMAKE_INSTALL_PREFIX}/share")
  set(CMAKE_INSTALL_FULL_DATADIR "${CMAKE_INSTALL_PREFIX}/share" CACHE PATH "default data dir path" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGEONKICK_DATA_DIR=\\\"${CMAKE_INSTALL_FULL_DATADIR}\\\"")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGEONKICK_DATA_DIR=\\\"${CMAKE_INSTALL_FULL_DATADIR}\\\"")

set(GKICK_DOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
set(GKICK_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GKICK_API_DIR ${GKICK_COMMON_DIR}/dsp)
set(GKICK_RC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/resources)
set(GKICK_STANDALONE_DIR ${GKICK_COMMON_DIR}/standalone)
set(GKICK_REDKITE_DIR ${GKICK_COMMON_DIR}/redkite)

if (GKICK_PLUGIN)
    if (GKICK_PLUGIN_VST)
      set(GKICK_VST_DIR ${GKICK_COMMON_DIR}/plugin/vst)
      set(GKICK_VST_SDK_PATH ${VST3_SDK_PATH})
    endif (GKICK_PLUGIN_VST)
    if (GKICK_PLUGIN_LV2)
      set(GKICK_LV2_DIR ${GKICK_COMMON_DIR}/plugin/lv2)
    endif (GKICK_PLUGIN_LV2)
endif(GKICK_PLUGIN)

set(GKICK_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(GKICK_PRESETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/presets)

set(GKICK_COMMON_HEADERS
  ${GKICK_COMMON_DIR}/Base64EncoderDecoder.h
  ${GKICK_COMMON_DIR}/DesktopPaths.h
  ${GKICK_COMMON_DIR}/GeonkickConfig.h
  ${GKICK_COMMON_DIR}/UiSettings.h
  ${GKICK_COMMON_DIR}/ViewState.h
  ${GKICK_COMMON_DIR}/geonkick_widget.h
  ${GKICK_COMMON_DIR}/geonkick_slider.h
  ${GKICK_COMMON_DIR}/geonkick_button.h
  ${GKICK_COMMON_DIR}/PathListModel.h
  ${GKICK_COMMON_DIR}/file_dialog.h
  ${GKICK_COMMON_DIR}/SampleBrowser.h
  ${GKICK_COMMON_DIR}/filter.h
  ${GKICK_COMMON_DIR}/geonkick_groupbox.h
  ${GKICK_COMMON_DIR}/top_bar.h
  ${GKICK_COMMON_DIR}/control_area.h
  ${GKICK_COMMON_DIR}/controls_widget.h
  ${GKICK_COMMON_DIR}/percussion_model.h
  ${GKICK_COMMON_DIR}/kit_model.h
  ${GKICK_COMMON_DIR}/percussion_view.h
  ${GKICK_COMMON_DIR}/kit_widget.h
  ${GKICK_COMMON_DIR}/envelope_draw_area.h
  ${GKICK_COMMON_DIR}/limiter.h
  ${GKICK_COMMON_DIR}/general_envelope.h
  ${GKICK_COMMON_DIR}/effects_group_box.h
  ${GKICK_COMMON_DIR}/compressor_group_box.h
  ${GKICK_COMMON_DIR}/distortion_group_box.h
  ${GKICK_COMMON_DIR}/layers_group_box.h
  ${GKICK_COMMON_DIR}/general_group_box.h
  ${GKICK_COMMON_DIR}/envelope.h
  ${GKICK_COMMON_DIR}/BufferView.h
  ${GKICK_COMMON_DIR}/kick_graph.h
  ${GKICK_COMMON_DIR}/envelope_widget.h
  ${GKICK_COMMON_DIR}/knob.h
  ${GKICK_COMMON_DIR}/oscillator.h
  ${GKICK_COMMON_DIR}/geonkick_api.h
  ${GKICK_COMMON_DIR}/percussion_state.h
  ${GKICK_COMMON_DIR}/kit_state.h
  ${GKICK_COMMON_DIR}/oscillator_envelope.h
  ${GKICK_COMMON_DIR}/oscillator_group_box.h
  ${GKICK_COMMON_DIR}/ExportAbstract.h
  ${GKICK_COMMON_DIR}/ExportSoundData.h
  ${GKICK_COMMON_DIR}/ExportToSfz.h
  ${GKICK_COMMON_DIR}/export_widget.h
  ${GKICK_COMMON_DIR}/preset.h
  ${GKICK_COMMON_DIR}/preset_folder.h
  ${GKICK_COMMON_DIR}/preset_browser_model.h
  ${GKICK_COMMON_DIR}/preset_browser_view.h
  ${GKICK_COMMON_DIR}/MidiKeyWidget.h
  ${GKICK_COMMON_DIR}/EnvelopePointContextWidget.h
  ${GKICK_COMMON_DIR}/MainWindow.h
)

set(GKICK_COMMON_SOURCES
  ${GKICK_COMMON_DIR}/Base64EncoderDecoder.cpp
  ${GKICK_COMMON_DIR}/DesktopPaths.cpp
  ${GKICK_COMMON_DIR}/GeonkickConfig.cpp
  ${GKICK_COMMON_DIR}/UiSettings.cpp
  ${GKICK_COMMON_DIR}/ViewState.cpp
  ${GKICK_COMMON_DIR}/geonkick_widget.cpp
  ${GKICK_COMMON_DIR}/geonkick_slider.cpp
  ${GKICK_COMMON_DIR}/geonkick_button.cpp
  ${GKICK_COMMON_DIR}/PathListModel.cpp
  ${GKICK_COMMON_DIR}/file_dialog.cpp
  ${GKICK_COMMON_DIR}/SampleBrowser.cpp
  ${GKICK_COMMON_DIR}/filter.cpp
  ${GKICK_COMMON_DIR}/geonkick_groupbox.cpp
  ${GKICK_COMMON_DIR}/top_bar.cpp
  ${GKICK_COMMON_DIR}/control_area.cpp
  ${GKICK_COMMON_DIR}/controls_widget.cpp
  ${GKICK_COMMON_DIR}/percussion_model.cpp
  ${GKICK_COMMON_DIR}/kit_model.cpp
  ${GKICK_COMMON_DIR}/percussion_view.cpp
  ${GKICK_COMMON_DIR}/kit_widget.cpp
  ${GKICK_COMMON_DIR}/envelope_draw_area.cpp
  ${GKICK_COMMON_DIR}/limiter.cpp
  ${GKICK_COMMON_DIR}/general_envelope.cpp
  ${GKICK_COMMON_DIR}/effects_group_box.cpp
  ${GKICK_COMMON_DIR}/compressor_group_box.cpp
  ${GKICK_COMMON_DIR}/distortion_group_box.cpp
  ${GKICK_COMMON_DIR}/layers_group_box.cpp
  ${GKICK_COMMON_DIR}/general_group_box.cpp
  ${GKICK_COMMON_DIR}/envelope.cpp
  ${GKICK_COMMON_DIR}/BufferView.cpp
  ${GKICK_COMMON_DIR}/kick_graph.cpp
  ${GKICK_COMMON_DIR}/envelope_widget.cpp
  ${GKICK_COMMON_DIR}/knob.cpp
  ${GKICK_COMMON_DIR}/oscillator.cpp
  ${GKICK_COMMON_DIR}/geonkick_api.cpp
  ${GKICK_COMMON_DIR}/percussion_state.cpp
  ${GKICK_COMMON_DIR}/kit_state.cpp
  ${GKICK_COMMON_DIR}/oscillator_envelope.cpp
  ${GKICK_COMMON_DIR}/oscillator_group_box.cpp
  ${GKICK_COMMON_DIR}/export_widget.cpp
  ${GKICK_COMMON_DIR}/ExportAbstract.cpp
  ${GKICK_COMMON_DIR}/ExportSoundData.cpp
  ${GKICK_COMMON_DIR}/ExportToSfz.cpp
  ${GKICK_COMMON_DIR}/preset.cpp
  ${GKICK_COMMON_DIR}/preset_folder.cpp
  ${GKICK_COMMON_DIR}/preset_browser_model.cpp
  ${GKICK_COMMON_DIR}/preset_browser_view.cpp
  ${GKICK_COMMON_DIR}/MidiKeyWidget.cpp
  ${GKICK_COMMON_DIR}/EnvelopePointContextWidget.cpp
  ${GKICK_COMMON_DIR}/MainWindow.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Enable application logging")
    option(ENABLE_LOGGING    "Enable logging for API and GUI" ON)
endif()

if (ENABLE_LOGGING)
  message(STATUS "set debug level")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_LOG_DEBUG_LEVEL")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_LOG_DEBUG_LEVEL")
  list(APPEND GKICK_API_FLAGS "-DGKICK_LIB_LOG_LEVEL_DEBUG")
  list(APPEND GKICK_API_FLAGS "-DGKICK_LIB_LOG_FUNCTION")
  list(APPEND GKICK_API_PLUGIN_FLAGS "-DGKICK_LIB_LOG_LEVEL_DEBUG")
  list(APPEND GKICK_API_PLUGIN_FLAGS "-DGKICK_LIB_LOG_FUNCTION")
  list(APPEND GKICK_API_PLUGIN_SINGLE_FLAGS "-DGKICK_LIB_LOG_LEVEL_DEBUG")
  list(APPEND GKICK_API_PLUGIN_SINGLE_FLAGS "-DGKICK_LIB_LOG_FUNCTION")
else(ENABLE_LOGGING)
  list(APPEND GKICK_API_FLAGS "-DGKICK_LIB_LOG_LEVEL_ERROR")
  list(APPEND GKICK_API_FLAGS "-DGKICK_LIB_LOG_FUNCTION")
  list(APPEND GKICK_API_PLUGIN_FLAGS "-DGKICK_LIB_LOG_LEVEL_ERROR")
  list(APPEND GKICK_API_PLUGIN_FLAGS "-DGKICK_LIB_LOG_FUNCTION")
  list(APPEND GKICK_API_PLUGIN_SINGLE_FLAGS "-DGKICK_LIB_LOG_LEVEL_ERROR")
  list(APPEND GKICK_API_PLUGIN_SINGLE_FLAGS "-DGKICK_LIB_LOG_FUNCTION")
endif(ENABLE_LOGGING)

find_package(PkgConfig REQUIRED)
find_package(RapidJSON REQUIRED)
if (CMAKE_SYSTEM_NAME MATCHES Windows)
   include_directories(${RapidJSON_INCLUDE_DIR})
endif()

pkg_check_modules(SNDFILE sndfile)
if (NOT SNDFILE_FOUND)
    message(FATAL_ERROR "can't find sndfile library")
endif(NOT SNDFILE_FOUND)

if (GKICK_STANDALONE)
  pkg_check_modules(JACK jack)
  if (NOT JACK_FOUND)
    message(STATUS "can't find jack library")
    set(GKICK_STANDALONE OFF)
    set(GKICK_JACK_NOT_FOUND ON)
  endif(NOT JACK_FOUND)
  list(APPEND GKICK_API_FLAGS "-DGEONKICK_AUDIO_JACK")
endif()

if (GKICK_PLUGIN)
  list(APPEND GKICK_API_PLUGIN_SINGLE_FLAGS "-DGEONKICK_SINGLE")
  message(STATUS "checking for LV2 library...")
  pkg_check_modules(LV2 lv2)
  if (NOT LV2_FOUND)
      message(FATAL_ERROR "can't find LV2 library")
  endif(NOT LV2_FOUND)
endif(GKICK_PLUGIN)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_OS_WIN")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_OS_WIN")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_OS_GNU")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_OS_GNU")
endif ()

include_directories(${GKICK_API_DIR}/src)
include_directories(${GKICK_REDKITE_DIR}/include)
link_directories(${GKICK_REDKITE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-fPIC)
add_subdirectory(${GKICK_API_DIR})
add_subdirectory(${GKICK_REDKITE_DIR})
include(${GKICK_RC_DIR}/resources.rc)

foreach(pngFile ${gkick_rc_png_files})
  message(STATUS "convert file ${pngFile}")
  get_filename_component(filename ${GKICK_RC_DIR}/${pngFile} NAME)
  string(REPLACE "." "_" filename ${filename})
  string(REPLACE " " "_" filename ${filename})
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/resources/${filename}.c
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/resources
    && ${CMAKE_BINARY_DIR}/src/redkite/tools/rkpng2c
    "${GKICK_RC_DIR}/${pngFile}" "${CMAKE_BINARY_DIR}/resources/${filename}.c" "rk__${filename}")
    set(GKICK_RC_OUTPUT ${GKICK_RC_OUTPUT} ${CMAKE_BINARY_DIR}/resources/${filename}.c)
endforeach()

add_custom_target(gkick_resources ALL DEPENDS ${GKICK_RC_OUTPUT})
add_dependencies(gkick_resources redkite rkpng2c)

include_directories(${GKICK_COMMON_DIR})
link_directories(${REDKITE_LIBRARY_DIR})
add_library(geonkick_common STATIC
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_SOURCES}
  ${GKICK_RC_OUTPUT})
add_dependencies(geonkick_common gkick_resources)

if (GKICK_PLUGIN)
    add_library(geonkick_common_single STATIC
      ${GKICK_COMMON_HEADERS}
      ${GKICK_COMMON_SOURCES}
      ${GKICK_RC_OUTPUT})
    add_dependencies(geonkick_common_single gkick_resources)
    target_compile_options(geonkick_common_single PUBLIC ${GKICK_API_PLUGIN_SINGLE_FLAGS})
endif(GKICK_PLUGIN)

message(STATUS "------------ Summary ---------")
if (GKICK_STANDALONE)
  message(STATUS "Standalone: yes" )
else(GKICK_STANDALONE)
  if (GKICK_JACK_NOT_FOUND)
    message(STATUS "Standalone: no (install jack development library)")
  else ()
    message(STATUS "Standalone: no")
  endif()
endif(GKICK_STANDALONE)

if (GKICK_PLUGIN_LV2)
  message(STATUS "LV2 plugin: yes" )
else(GKICK_PLUGIN_LV2)
  message(STATUS "LV2 plugin: no")
endif(GKICK_PLUGIN_LV2)

if (GKICK_PLUGIN_VST)
  message(STATUS "VST plugin: yes" )
else(GKICK_PLUGIN_VST)
  message(STATUS "VST plugin: no")
endif(GKICK_PLUGIN_VST)

if(GKICK_PRESETS)
  message(STATUS "Geonkick presets: yes" )
else(GKICK_PRESETS)
  message(STATUS "Geonkick presets: no")
endif(GKICK_PRESETS)

if (ENABLE_LOGGING)
  message(STATUS "Debug enabled: yes" )
else(ENABLE_LOGGING)
  message(STATUS "Debug enabled: no" )
endif(ENABLE_LOGGING)
message(STATUS "------------------------------")

if (GKICK_STANDALONE)
   add_subdirectory(${GKICK_STANDALONE_DIR})
endif (GKICK_STANDALONE)

if (GKICK_PLUGIN)
   add_subdirectory(${GKICK_COMMON_DIR}/plugin)
endif (GKICK_PLUGIN)

add_subdirectory(data)

if (GKICK_PRESETS)
   add_subdirectory(presets)
endif (GKICK_PRESETS)
