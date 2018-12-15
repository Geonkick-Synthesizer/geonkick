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
if (ENABLE_STANDALONE)
  find_package(JACK REQUIRED)
endif()
if (ENABLE_EXPORT)
  find_package(SNDFILE REQUIRED)
endif()
if (ENABLE_LV2)
  find_package(LV2 REQUIRED)
endif()

#add_custom_command(OUTPUT <output-file>
#  COMMAND make run
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/<subdir>
#  )

#add_custom_target(<target_name> COMMAND make run
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/<subdir>
#  )


#add_subdirectory(standalone);
#add_subdirectory(lv2);


#include(${QT_USE_FILE})
#add_definitions( ${QT_DEFINITIONS} )
#include_directories( ${CMAKE_BINARY_DIR} )


# Next, using precompiler, compiler and linker

# using Qt meta-system (precompiler)
#QT5_ADD_RESOURCES(RES_SOURCES ${RESOURCES} )
#QT5_WRAP_CPP(MOC_SRCS ${COMMON_CODE_HEADERS} )

# compile
#add_executable(PROJECT_NAME ${COMMON_CODE_SOURCES} ${MOC_SRCS} ${RES_SOURCES})
#target_link_libraries(PROJECT_NAME ${QT_LIBRARIES} )

