set(GKICK_STANDALONE_SOURCES ${GKICK_STANDALONE_DIR}/main.cpp)

add_executable(geonkick
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_RESOURCES}
  ${GKICK_STANDALONE_SOURCES})

add_dependencies(geonkick api_standalone)
target_link_libraries(geonkick  geonkick_common)
target_link_libraries(geonkick ${CMAKE_BINARY_DIR}/api/libgeonkick.a)
target_link_libraries(geonkick "-lredkite -lX11 -ljack -lsndfile -lrt -lm -ldl -lpthread")

install(TARGETS geonkick DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

