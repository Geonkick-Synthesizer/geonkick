set(GKICK_LV2_SOURCES ${GKICK_LV2_DIR}/geonkick_lv2.cpp)

add_library(geonkick_lv2 SHARED
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_RESOURCES}
  ${GKICK_LV2_SOURCES})

target_link_libraries(geonkick_lv2
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  geonkick_common)
target_link_libraries(geonkick_lv2 ${CMAKE_BINARY_DIR}/api_lv2/libgeonkick_lv2.a)
target_link_libraries(geonkick_lv2 "-lredkite -lsndfile -lrt -lm -ldl -lpthread")
add_dependencies(geonkick_lv2 api_lv2)

install(DIRECTORY ${GKICK_LV2_DIR}/geonkick.lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2)
install(TARGETS geonkick_lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/geonkick.lv2)
