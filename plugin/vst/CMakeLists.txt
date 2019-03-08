set(GKICK_VST_HEADERS
  ${GKICK_VST_DIR}/GKickVstcontroller.h
  ${GKICK_VST_DIR}/VstIds.h
  ${GKICK_VST_DIR}/GKickVstProcessor.h
  ${GKICK_VST_DIR}/GKickVstEditor.h)

set(GKICK_VST_SOURCES
  ${GKICK_VST_DIR}/VstFactory.cpp
  ${GKICK_VST_DIR}/GKickVstcontroller.cpp
  ${GKICK_VST_DIR}/GKickVstProcessor.cpp)
  ${GKICK_VST_DIR}/GKickVstEditor.cpp)

smtg_add_vst3plugin(geonkick ${GKICK_VST_HEADERS} ${GKICK_VST_SOURCES})
target_link_libraries(geonkick PRIVATE base sdk redkite)
target_sources(geonkick PRIVATE ${GKICK_VST_DIR}/info.rc)

