set(GKICK_STANDALONE_SOURCES ${GKICK_STANDALONE_DIR}/main.cpp)

add_executable(geonkick
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_RESOURCES}
  ${GKICK_STANDALONE_SOURCES})

add_dependencies(geonkick api_standalone)
add_dependencies(geonkick redkite)
target_link_libraries(geonkick  geonkick_common api_standalone redkite)
if (CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(geonkick "-mwindows -lstdc++ -lstdc++fs -lsndfile -lm -lpthread")
else()
    target_link_libraries(geonkick "-lstdc++fs")
    target_link_libraries(geonkick "-lX11 -ljack -lsndfile -lrt -lm -ldl -lpthread -lcairo")
endif()

install(TARGETS geonkick DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${GKICK_DOC_DIR}/man/geonkick.1  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1)

