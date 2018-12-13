cmake_minimum_required (VERSION 3.7)

project(geonkick VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(GKICK_API_DIR "api")
set(GKICK_COMMON_DIR "src")

file(GLOB GKICK_COMMON_SOURCES ${GKICK_COMMON_DIR}/"*.cpp")
file(GLOB GKICK_COMMON_HEADERS ${GKICK_COMMON_DIR}/"*.h")
set(GKICK_COMMON_RESOUCES ${GKICK_COMMON_DIR}/resources/resources.irc)

find_package(Qt5  REQUIRED)
find_package(JACK REQUIRED)
find_package(SNDFILE REQUIRED)

add_custom_command(OUTPUT <output-file>
  COMMAND make run
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/<subdir>
  )

add_custom_target(<target_name> COMMAND make run
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/<subdir>
  )


add_subdirectory(standalone);
add_subdirectory(lv2);


#include(${QT_USE_FILE})
#add_definitions( ${QT_DEFINITIONS} )
#include_directories( ${CMAKE_BINARY_DIR} )


# Next, using precompiler, compiler and linker

# using Qt meta-system (precompiler)
#QT5_ADD_RESOURCES(RES_SOURCES ${RESOURCES} )
#QT5_WRAP_CPP(MOC_SRCS ${COMMON_CODE_HEADERS} )

# compile
add_executable(PROJECT_NAME ${COMMON_CODE_SOURCES} ${MOC_SRCS} ${RES_SOURCES})
target_link_libraries(PROJECT_NAME ${QT_LIBRARIES} )

