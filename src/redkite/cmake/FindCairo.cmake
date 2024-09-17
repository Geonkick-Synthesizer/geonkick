# FindCairo.cmake

find_path(CAIRO_INCLUDE_DIRS NAMES cairo/cairo.h)
if (CMAKE_SYSTEM_NAME MATCHES Windows)
    message(STATUS "Find Cairo for Windows/MSYS2/UCRT64...")
    find_library(CAIRO_LIBRARIES NAMES cairo PATHS "/ucrt64/lib")
else()
    message(STATUS "Find Cairo for GNU/Linux...")
    find_library(CAIRO_LIBRARIES NAMES cairo)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Cairo DEFAULT_MSG
                                  CAIRO_INCLUDE_DIRS CAIRO_LIBRARIES)

message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message("CAIRO_INCLUDE_DIRS: ${CAIRO_INCLUDE_DIRS}")
message("CAIRO_LIBRARIES: ${CAIRO_LIBRARIES}")

mark_as_advanced(CAIRO_INCLUDE_DIRS CAIRO_LIBRARIES)
