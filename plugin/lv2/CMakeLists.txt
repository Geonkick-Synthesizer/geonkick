set(GKICK_LV2_SOURCES ${GKICK_LV2_DIR}/geonkick_lv2.cpp)

add_library(geonkick_lv2 SHARED
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_RESOURCES}
  ${GKICK_LV2_SOURCES})

set_target_properties(geonkick_lv2 PROPERTIES PREFIX "")
target_link_libraries(geonkick_lv2  geonkick_common api_plugin)
target_link_libraries(geonkick_lv2 "-lstdc++fs")
target_link_libraries(geonkick_lv2 "-lredkite -lX11 -lsndfile -lrt -lm -ldl -lpthread -lcairo")
add_dependencies(geonkick_lv2 api_plugin)

if (GKICK_PLUGIN)
  add_library(geonkick_single_lv2 SHARED
    ${GKICK_COMMON_HEADERS}
    ${GKICK_COMMON_RESOURCES}
    ${GKICK_LV2_SOURCES})
  set_target_properties(geonkick_single_lv2 PROPERTIES PREFIX "")
  target_link_libraries(geonkick_single_lv2  geonkick_common_single api_plugin_single)
  target_link_libraries(geonkick_single_lv2 "-lstdc++fs")
  target_link_libraries(geonkick_single_lv2 "-lredkite -lX11 -lsndfile -lrt -lm -ldl -lpthread -lcairo")
  add_dependencies(geonkick_single_lv2 api_plugin_single)
  target_compile_options(geonkick_single_lv2 PUBLIC ${GKICK_API_PLUGIN_SINGLE_FLAGS})
  install(TARGETS geonkick_single_lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lv2/geonkick.lv2)
endif(GKICK_PLUGIN)

install(TARGETS geonkick_lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lv2/geonkick.lv2)
install(DIRECTORY ${GKICK_LV2_DIR}/geonkick.lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lv2)
