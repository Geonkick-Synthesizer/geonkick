cmake_minimum_required (VERSION 3.7)

project(geonkick VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(GKICK_API_DIR "api")
set(GKICK_COMMON_DIR "src")

file(GLOB GKICK_COMMON_SOURCES ${GKICK_COMMON_DIR}/"*.cpp")
file(GLOB GKICK_COMMON_HEADERS ${GKICK_COMMON_DIR}/"*.h")
set(GKICK_COMMON_RESOUCES ${GKICK_COMMON_DIR}/resources/resources.irc)

# Disabling standalone build there also will no more be need to install jack dependence.
option(ENABLE_STANDALONE "Enbale build for standalone application" ON)
# Disabling LV2 plugin build there also will not be nedded to install LV2 library dependence.
option(ENABLE_LV2 "Enbale build for LV2 plugin" ON)
option(ENABLE_LOGGING    "Enbale logigng for API and GUI" ON)

# Enables the double precision calculation for API.
# By default is single point precision.
option(ENABLE_DOUIBLE_PRECISION "Enbale double precision for API" OFF)

# Enables the export to audio formats.
# Disabling it there will not be needed to install libsndfile library dependence.
option(ENABLE_EXPORT "Enable export to audio formats" ON)

if (ENABLE_LOGGING)
      set(DEFAULT_LOG_LEVEL "GEONKICK_LOG_LEVEL_DEBUG")
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(PkgConfig REQUIRED)

if (ENABLE_STANDALONE)
  pkg_check_modules(JACK jack)
  if (NOT JACK_FOUND)
    message(FATAL_ERROR "can't find jack library")
  endif(NOT JACK_FOUND)
endif()

if (ENABLE_EXPORT)
  pkg_check_modules(SNDFILE sndfile)
  if (NOT SNDFILE_FOUND)
    message(FATAL_ERROR "can't find sndfile library")
  endif(NOT SNDFILE_FOUND)
endif()

if (ENABLE_LV2)
  pkg_check_modules(LV2 lv2)
  if (NOT LV2_FOUND)
    message(FATAL_ERROR "can't find LV2 library")
  endif(NOT LV2_FOUND)
endif()

#add_custom_command(OUTPUT <output-file>
#  COMMAND make run
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/<subdir>
#  )

#add_custom_target(<target_name> COMMAND make run
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/<subdir>
#  )

#add_subdirectory(standalone)
#add_subdirectory(lv2)



