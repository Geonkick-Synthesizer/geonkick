cmake_minimum_required (VERSION 3.8)

project(redkite VERSION 2.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(RK_ENABLE_EXAMPLES "Build examples" OFF)
option(RK_BUILD_TOOLS "Build tools" ON)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
  message(STATUS "Build Redkite for Windows")
  add_compile_options(-DRK_OS_WIN)
  add_compile_options(-DRK_FOR_SHARED)
  add_compile_options(-DCAIRO_WIN32_STATIC_BUILD)
elseif(CMAKE_SYSTEM_NAME MATCHES Darwin)
  message(FATAL_ERROR "Redkite is not implemented yet for Mac OS X")
elseif (CMAKE_SYSTEM_NAME MATCHES Linux)
  message(STATUS " Build Redkite for GNU/Linux")
elseif (CMAKE_SYSTEM_NAME MATCHES FreeBSD)
  message(STATUS " Build Redkite for FreeBSD")
else()
  message(FATAL_ERROR "Redkite is not implemented yet for ${CMAKE_SYSTEM_NAME}")
endif()

set(RK_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(RK_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(RK_EXAMPLES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(RK_TOOLS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tools)
set(RK_DOC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/doc)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "no build type selected, default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type Release" FORCE)
endif()

if (RK_ARCHITECTURE MATCHES x86_64)
  message(STATUS "set optimisation compiler flags for ${RK_ARCHITECTURE}")
  set(RK_OPTIMISATION_FLAGS "-O3 -msse -msse2 -mfpmath=sse -funsafe-math-optimizations -fno-math-errno -fno-trapping-math -fomit-frame-pointer")
else ()
  message(STATUS "set optimisation compiler flags for ${RK_ARCHITECTURE}")
  set(RK_OPTIMISATION_FLAGS "-O3 -funsafe-math-optimizations -fno-math-errno -fno-trapping-math -fomit-frame-pointer")
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall ${RK_OPTIMISATION_FLAGS} ${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_C_FLAGS "-Wall ${RK_OPTIMISATION_FLAGS} ${CMAKE_C_FLAGS} -fvisibility=hidden")

if (NOT RK_EXPORT_INTERFACE)
  message(STATUS "don't export Redkite interface")
else ()
  message(STATUS "export Redkite interface")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_EXPORT_INTERFACE")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_EXPORT_INTERFACE")
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "default install path" FORCE)
endif()

if (NOT CMAKE_INSTALL_LIBDIR)
  message(STATUS "setting the default lib dir path to /usr/lib")
  set(CMAKE_INSTALL_LIBDIR /usr/lib CACHE STRING "lib dir to /usr/lib" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "enable logging")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_LOG_DEBUG_LEVEL")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_LOG_DEBUG_LEVEL")
endif()

if (NOT RK_GRAPHICS_BACKEND)
  message(STATUS "use default Cairo graphics backend")
  set(RK_GRAPHICS_BACKEND "Cairo")
  find_package(Cairo REQUIRED)
  if (CAIRO_FOUND)
    message(STATUS "Cairo library found")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_GRAPHICS_CAIRO_BACKEND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_GRAPHICS_CAIRO_BACKEND")
  else(CAIRO_FOUND)
    message(FATAL_ERROR "Can't find Cairo library")
  endif(CAIRO_FOUND)
elseif (RK_GRAPHICS_BACKEND MATCHES Cairo)
  find_package(Cairo REQUIRED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_GRAPHICS_CAIRO_BACKEND")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_GRAPHICS_CAIRO_BACKEND")
endif()

include_directories(${RK_INCLUDE_PATH})
include_directories(${RK_INCLUDE_PATH}/impl)

set(RK_HEADERS
  ${RK_INCLUDE_PATH}/Rk.h
  ${RK_INCLUDE_PATH}/RkVariant.h
  ${RK_INCLUDE_PATH}/RkObserver.h
  ${RK_INCLUDE_PATH}/RkObject.h
  ${RK_INCLUDE_PATH}/RkAction.h
  ${RK_INCLUDE_PATH}/RkLog.h
  ${RK_INCLUDE_PATH}/RkPoint.h
  ${RK_INCLUDE_PATH}/RkRealPoint.h
  ${RK_INCLUDE_PATH}/RkSize.h
  ${RK_INCLUDE_PATH}/RkRect.h
  ${RK_INCLUDE_PATH}/RkColor.h
  ${RK_INCLUDE_PATH}/RkPen.h
  ${RK_INCLUDE_PATH}/RkFont.h
  ${RK_INCLUDE_PATH}/RkPlatform.h
  ${RK_INCLUDE_PATH}/RkEvent.h
  ${RK_INCLUDE_PATH}/RkEventQueue.h
  ${RK_INCLUDE_PATH}/RkTimer.h
  ${RK_INCLUDE_PATH}/RkWidget.h
  ${RK_INCLUDE_PATH}/RkLabel.h
  ${RK_INCLUDE_PATH}/RkButton.h
  ${RK_INCLUDE_PATH}/RkSpinBox.h
  ${RK_INCLUDE_PATH}/RkLineEdit.h
  ${RK_INCLUDE_PATH}/RkList.h
  ${RK_INCLUDE_PATH}/RkProgressBar.h
  ${RK_INCLUDE_PATH}/RkCanvas.h
  ${RK_INCLUDE_PATH}/RkImage.h
  ${RK_INCLUDE_PATH}/RkPainter.h
  ${RK_INCLUDE_PATH}/RkMain.h
  ${RK_INCLUDE_PATH}/RkModel.h
  ${RK_INCLUDE_PATH}/RkContainerItem.h
  ${RK_INCLUDE_PATH}/RkContainerWidgetItem.h
  ${RK_INCLUDE_PATH}/RkContainer.h
  ${RK_INCLUDE_PATH}/RkFlowContainer.h
  ${RK_INCLUDE_PATH}/RkTransition.h)

set(RK_HEADERS_IMPL
  ${RK_INCLUDE_PATH}/impl/RkObjectImpl.h
  ${RK_INCLUDE_PATH}/impl/RkMainImpl.h
  ${RK_INCLUDE_PATH}/impl/RkShortcut.h
  ${RK_INCLUDE_PATH}/impl/RkEventQueueImpl.h
  ${RK_INCLUDE_PATH}/impl/RkSystemWindow.h
  ${RK_INCLUDE_PATH}/impl/RkWidgetImpl.h
  ${RK_INCLUDE_PATH}/impl/RkLabelImpl.h
  ${RK_INCLUDE_PATH}/impl/RkListImpl.h
  ${RK_INCLUDE_PATH}/impl/RkModelImpl.h
  ${RK_INCLUDE_PATH}/impl/RkButtonImpl.h
  ${RK_INCLUDE_PATH}/impl/RkSpinBoxImpl.h
  ${RK_INCLUDE_PATH}/impl/RkLineEditImpl.h
  ${RK_INCLUDE_PATH}/impl/RkProgressBarImpl.h
  ${RK_INCLUDE_PATH}/impl/RkCanvasInfo.h
  ${RK_INCLUDE_PATH}/impl/RkImageImpl.h
  ${RK_INCLUDE_PATH}/impl/RkPainterImpl.h
  )

if (RK_GRAPHICS_BACKEND MATCHES Cairo)
  set(RK_GRAPHICS_BACKEND_HEADRES
    ${RK_INCLUDE_PATH}/impl/RkCairoGraphicsBackend.h
    ${RK_INCLUDE_PATH}/impl/RkCairoImageBackendCanvas.h)
  set(RK_GRAPHICS_BACKEND_SOURCES
    ${RK_SRC_PATH}/RkCairoGraphicsBackend.cpp
    ${RK_SRC_PATH}/RkCairoImageBackendCanvas.cpp)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(RK_HEADERS_PLATFORM
  ${RK_INCLUDE_PATH}/impl/platforms/win/RkWindowWin.h
  ${RK_INCLUDE_PATH}/impl/platforms/win/RkEventQueueWin.h)
elseif (CMAKE_SYSTEM_NAME MATCHES Darwin)
set(RK_HEADERS_PLATFORM
  ${RK_INCLUDE_PATH}/impl/platforms/mac/RkWindowMac.h
  ${RK_INCLUDE_PATH}/impl/platforms/mac/RkEventQueueMac.h)
else() # defaut GNU/Linux
  set(RK_HEADERS_PLATFORM
  ${RK_INCLUDE_PATH}/impl/platforms/xwin/RkWindowX.h
  ${RK_INCLUDE_PATH}/impl/platforms/xwin/RkEventQueueX.h)
endif()

set(RK_SOURCES
  ${RK_SRC_PATH}/RkObject.cpp
  ${RK_SRC_PATH}/RkCanvas.cpp
  ${RK_SRC_PATH}/RkObjectImpl.cpp
  ${RK_SRC_PATH}/RkSystemWindow.cpp
  ${RK_SRC_PATH}/RkWidget.cpp
  ${RK_SRC_PATH}/RkWidgetImpl.cpp
  ${RK_SRC_PATH}/RkMain.cpp
  ${RK_SRC_PATH}/RkEventQueue.cpp
  ${RK_SRC_PATH}/RkTimer.cpp
  ${RK_SRC_PATH}/RkEventQueueImpl.cpp
  ${RK_SRC_PATH}/RkMainImpl.cpp
  ${RK_SRC_PATH}/RkModel.cpp
  ${RK_SRC_PATH}/RkModelImpl.cpp
  ${RK_SRC_PATH}/RkLabel.cpp
  ${RK_SRC_PATH}/RkLabelImpl.cpp
  ${RK_SRC_PATH}/RkSpinBox.cpp
  ${RK_SRC_PATH}/RkSpinBoxImpl.cpp
  ${RK_SRC_PATH}/RkButton.cpp
  ${RK_SRC_PATH}/RkButtonImpl.cpp
  ${RK_SRC_PATH}/RkLineEdit.cpp
  ${RK_SRC_PATH}/RkLineEditImpl.cpp
  ${RK_SRC_PATH}/RkList.cpp
  ${RK_SRC_PATH}/RkListImpl.cpp
  ${RK_SRC_PATH}/RkProgressBar.cpp
  ${RK_SRC_PATH}/RkProgressBarImpl.cpp
  ${RK_SRC_PATH}/RkPainter.cpp
  ${RK_SRC_PATH}/RkImage.cpp
  ${RK_SRC_PATH}/RkImageImpl.cpp
  ${RK_SRC_PATH}/RkPainterImpl.cpp
  ${RK_SRC_PATH}/RkContainer.cpp
  ${RK_SRC_PATH}/RkFlowContainer.cpp)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
  set(RK_SOURCES_PLATFORM
    ${RK_SRC_PATH}/platforms/win/RkLogWin.cpp
    ${RK_SRC_PATH}/platforms/win/RkPlatformWin.cpp
    ${RK_SRC_PATH}/platforms/win/RkWindowWin.cpp
    ${RK_SRC_PATH}/platforms/win/RkEventQueueWin.cpp)
elseif (CMAKE_SYSTEM_NAME MATCHES Darwin)
  set(RK_SOURCES_PLATFORM
    ${RK_SRC_PATH}/platforms/mac/RkPlatformMac.cpp
    ${RK_SRC_PATH}/platforms/mac/RkWindowMac.cpp
    ${RK_SRC_PATH}/platforms/mac/RkEventQueueMac.cpp)
else() # defaut GNU/Linux
  set(RK_SOURCES_PLATFORM
    ${RK_SRC_PATH}/platforms/xwin/RkLogX.cpp
    ${RK_SRC_PATH}/platforms/xwin/RkPlatformX.cpp
    ${RK_SRC_PATH}/platforms/xwin/RkWindowX.cpp
    ${RK_SRC_PATH}/platforms/xwin/RkEventQueueX.cpp)
endif()

if (CMAKE_SYSTEM_NAME MATCHES Windows)
  include_directories(${RK_INCLUDE_PATH}/impl/platforms/win)
elseif (CMAKE_SYSTEM_NAME MATCHES Darwin)
  include_directories(${RK_INCLUDE_PATH}/impl/platforms/mac)
# not implemented yet
else() # defualt GNU/Linux
add_definitions(-fPIC)
include_directories(${RK_INCLUDE_PATH}/impl/platforms/xwin)
endif()

add_library(redkite STATIC
  ${RK_HEADERS}
  ${RK_HEADERS_IMPL}
  ${RK_HEADERS_PLATFORM}
  ${RK_GRAPHICS_BACKEND_HEADRES}
  ${RK_GRAPHICS_BACKEND_SOURCES}
  ${RK_SOURCES}
  ${RK_SOURCES_PLATFORM})

if (CMAKE_SYSTEM_NAME MATCHES Windows)
  install(TARGETS redkite DESTINATION ${CMAKE_INSTALL_PREFIX})
  install(FILES ${RK_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
else()
  install(TARGETS redkite DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
  install(FILES ${RK_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/redkite)
endif()

if(RK_BUILD_TOOLS)
	add_subdirectory(tools)
endif(RK_BUILD_TOOLS)
if(RK_ENABLE_EXAMPLES)
   add_subdirectory(examples)
endif(RK_ENABLE_EXAMPLES)
