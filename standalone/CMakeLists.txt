set(GKICK_STANDALONE_SOURCES ${GKICK_STANDALONE_DIR}/main.cpp)

add_executable(geonkick
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_RESOURCES}
  ${GKICK_STANDALONE_SOURCES})

set_target_properties(geonkick PROPERTIES LINK_FLAGS "-ljack ${GKICK_API_LINK_FLAGS}")
target_link_libraries(geonkick
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  ${CMAKE_BINARY_DIR}/libgeonkick_common.a
  ${CMAKE_BINARY_DIR}/api/libgeonkick.a)

