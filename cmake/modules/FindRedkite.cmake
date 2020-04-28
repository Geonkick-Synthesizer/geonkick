find_path(REDKITE_INCLUDE_DIR Rk.h
          /usr/include/redkite
          /usr/local/include/redkite
	  ${CMAKE_INSTALL_PREFIX}/include/redkite
	  ${CMAKE_FIND_ROOT_PATH}/include/redkite
	  ${GKICK_REDKITE_SDK_PATH}/include/redkite)

find_path(REDKITE_LIBRARY_DIR libredkite.a
          /usr/lib
          /usr/lib64
          /usr/local/lib
          /usr/local/lib64
	  ${CMAKE_INSTALL_PREFIX}/lib
	  ${CMAKE_INSTALL_PREFIX}/lib64
	  ${CMAKE_FIND_ROOT_PATH}/lib
	  ${CMAKE_FIND_ROOT_PATH}/lib64
	  ${GKICK_REDKITE_SDK_PATH}/lib)

find_path(REDKITE_BINARY_DIR rkpng2c
          /usr/bin
          /usr/local/bin
	  ${CMAKE_INSTALL_PREFIX}/bin
	  ${CMAKE_FIND_ROOT_PATH}/bin
	  ${GKICK_REDKITE_SDK_PATH}/bin)

if (REDKITE_INCLUDE_DIR AND REDKITE_LIBRARY_DIR AND REDKITE_BINARY_DIR)
   set(REDKITE_FOUND TRUE)
endif (REDKITE_INCLUDE_DIR AND REDKITE_LIBRARY_DIR AND REDKITE_BINARY_DIR)

if (REDKITE_FOUND)
      message(STATUS "Found Redkite")
      message(STATUS "Redkite include path: ${REDKITE_INCLUDE_DIR}")
      message(STATUS "Redkite library path: ${REDKITE_LIBRARY_DIR}")
      message(STATUS "Redkite binary path: ${REDKITE_BINARY_DIR}")
else (REDKITE_FOUND)
      if (NOT REDKITE_INCLUDE_DIR)
        message(STATUS "Redkite include path: not found")
      endif ()
      if (NOT REDKITE_LIBRARY_DIR)
        message(STATUS "Redkite library path: not found")
      endif ()
      if (NOT REDKITE_BINARY_DIR)
        message(STATUS "Redkite binary path: not found")
      endif ()
      message(FATAL_ERROR "Could not find Redkite")
endif (REDKITE_FOUND)

