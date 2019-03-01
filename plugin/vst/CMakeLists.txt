include(AddVST3Library)

set(GKICK_VST_HEADERS
  ${GKICK_VST_DIR}/controller_vst.h
  ${GKICK_VST_DIR}/ids_vst.h
  ${GKICK_VST_DIR}/processor_vst.h)

set(GKICK_VST_SOURCES
  ${GKICK_VST_DIR}/factory_vst.cpp
  ${GKICK_VST_DIR}/controller_vst.cpp
  ${GKICK_VST_DIR}/processor_vst.cpp)

smtg_add_vst3plugin(geonkick ${GKICK_VST_HEADERS} ${GKICK_VST_SOURCES})
target_link_libraries(geonkick PRIVATE base sdk)
