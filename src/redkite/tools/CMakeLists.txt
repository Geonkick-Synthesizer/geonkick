set(RK_TOOLS_SOURCES ${RK_TOOLS_PATH}/rkpng2c.c)

if (RK_GRAPHICS_BACKEND MATCHES Cairo)
  set(RK_GRAPHICS_BACKEND_LINK_LIBS "-lcairo")
else()
  set(RK_GRAPHICS_BACKEND_LINK_LIBS "-lcairo")
endif()

add_executable(rkpng2c ${RK_EXEC_OPTION} ${RK_TOOLS_SOURCES})

if (CMAKE_SYSTEM_NAME MATCHES Windows)
  target_link_libraries(rkpng2c "-mwindows -lstdc++ -lm -lmingw32")
  target_link_libraries(rkpng2c ${RK_GRAPHICS_BACKEND_LINK_LIBS})
else()
  target_link_libraries(rkpng2c ${RK_GRAPHICS_BACKEND_LINK_LIBS})
endif()

install(TARGETS rkpng2c DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
