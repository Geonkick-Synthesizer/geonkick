set(GKICK_LV2_SOURCES ${GKICK_LV2_DIR}/geonkick_lv2.cpp)

add_library(geonkick_lv2 SHARED
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_RESOURCES}
  ${GKICK_LV2_SOURCES})

set_target_properties(geonkick_lv2 PROPERTIES LINK_FLAGS "${GKICK_API_LINK_FLAGS}")
target_link_libraries(geonkick_lv2
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  geonkick_common
  ${CMAKE_BINARY_DIR}/api_lv2/libgeonkick_lv2.a)
