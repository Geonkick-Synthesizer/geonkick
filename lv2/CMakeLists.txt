set(GKICK_LV2_SOURCES ${GKICK_LV2_DIR}/geonkick_lv2.cpp)

add_library(geonkick_lv2.so SHARED
  ${GKICK_LV2_SOURCES}
  ${CMAKE_BINARY_DIR}/geonkick_common)

target_link_libraries(geonkick_lv2.so
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  ${CMAKE_BINARY_DIR}/api/libgeonkick_lv2.a)
