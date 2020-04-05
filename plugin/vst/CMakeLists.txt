set(GKICK_VST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
if (GKICK_VST_SDK_PATH)
  list(APPEND CMAKE_MODULE_PATH "${GKICK_VST_SDK_PATH}/cmake/modules")
  include(Global)
  include(AddVST3Library)
else (GKICK_VST_SDK_PATH)
  message(STATUS "GKICK_VST_SDK_PATH was not set")
endif(GKICK_VST_SDK_PATH)

include_directories(${GKICK_VST_SDK_PATH})
include_directories(${GKICK_REDKITE_SDK_PATH}/include)
link_directories(${GKICK_VST_SDK_PATH}/build/lib/Debug)
link_directories(${GKICK_REDKITE_SDK_PATH}/lib)

set(GKICK_VST_HEADERS
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstsinglecomponenteffect.h
  ${GKICK_VST_DIR}/VstIds.h
  ${GKICK_VST_DIR}/GKickVstProcessor.h
  ${GKICK_VST_DIR}/GKickVstEditor.h)

set(GKICK_VST_SOURCES
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstsinglecomponenteffect.cpp
  ${GKICK_VST_DIR}/VstFactory.cpp
  ${GKICK_VST_DIR}/GKickVstProcessor.cpp
  ${GKICK_VST_DIR}/GKickVstEditor.cpp)

set(target geonkick_vst)

smtg_add_vst3plugin(${target} ${GKICK_VST_HEADERS} ${GKICK_VST_SOURCES})
target_link_libraries(${target} PRIVATE geonkick_common api_plugin)
target_link_libraries(${target} PRIVATE "-lstdc++fs")
target_link_libraries(${target} PRIVATE base sdk pluginterfaces)
target_link_libraries(${target} PRIVATE "-lredkite -lX11 -lsndfile -lrt -lm -ldl -lpthread -lcairo")
target_link_libraries(${target} PRIVATE "-Wl,--no-undefined")
add_dependencies(${target} geonkick_common)
