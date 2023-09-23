set(GKICK_VST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
if (GKICK_VST_SDK_PATH)
  list(APPEND CMAKE_MODULE_PATH "${GKICK_VST_SDK_PATH}/cmake/modules")
else (GKICK_VST_SDK_PATH)
  message(STATUS "GKICK_VST_SDK_PATH was not set")
endif(GKICK_VST_SDK_PATH)

include_directories(${GKICK_VST_SDK_PATH})
include_directories(${GKICK_REDKITE_SDK_PATH}/include)
link_directories(${GKICK_VST_SDK_PATH}/build/lib)
link_directories(${GKICK_REDKITE_SDK_PATH}/lib)

set(GKICK_VST_HEADERS
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstcomponentbase.h
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstsinglecomponenteffect.h
  ${GKICK_VST_DIR}/VstIds.h
  ${GKICK_VST_DIR}/GKickVstProcessor.h
#  ${GKICK_VST_DIR}/GKickVstEditor.h
  )

set(GKICK_VST_SOURCES
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstcomponentbase.cpp
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstsinglecomponenteffect.cpp
  ${GKICK_VST_DIR}/VstFactory.cpp
  ${GKICK_VST_DIR}/GKickVstProcessor.cpp
#  ${GKICK_VST_DIR}/GKickVstEditor.cpp
  )

set(target geonkick_vst)

add_library(${target} SHARED ${GKICK_VST_HEADERS} ${GKICK_VST_SOURCES})

set_target_properties(${target} PROPERTIES PREFIX "")
set_target_properties(${target} PROPERTIES OUTPUT_NAME "Geonkick")
if (CMAKE_SYSTEM_NAME MATCHES Windows)
    set_target_properties(${target} PROPERTIES SUFFIX ".dll")
else ()
    set_target_properties(${target} PROPERTIES SUFFIX ".so")
endif ()

target_link_libraries(${target} PRIVATE geonkick_common api_plugin redkite)
target_link_libraries(${target} PRIVATE base sdk sdk_common pluginterfaces)
#target_link_libraries(${target} PRIVATE "-lX11 -lsndfile -lcrypto -lrt -lm -ldl -lpthread -lcairo")
target_link_libraries(${target} PRIVATE "-lstdc++ -lstdc++fs -lcrypto -lm -lpthread")
target_link_libraries(${target} PRIVATE "-Wl,--no-undefined")

add_dependencies(${target} geonkick_common)

install(TARGETS geonkick_vst DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/vst3/Geonkick.vst3/Contents/${GKICK_ARCHITECTURE}-linux)
