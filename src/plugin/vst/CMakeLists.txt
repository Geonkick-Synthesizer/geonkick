set(GKICK_VST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SMTG_ENABLE_VST3_HOSTING_EXAMPLES OFF CACHE BOOL "disable hosting examples")
set(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES OFF CACHE BOOL "disable plugin examples")
set(SMTG_ENABLE_VSTGUI_SUPPORT OFF CACHE BOOL "disable vstgui examples")
list(APPEND CMAKE_MODULE_PATH "${GKICK_VST_SDK_PATH}/cmake/modules")
include_directories(${GKICK_VST_SDK_PATH})
include_directories(${GKICK_REDKITE_SDK_PATH}/include)
link_directories(${GKICK_REDKITE_SDK_PATH}/lib)

get_filename_component(VST3SDK_FOLDER_NAME ${GKICK_VST_SDK_PATH} NAME)
if(NAME_FROM_PATH STREQUAL "" AND (VST3SDK_FOLDER_NAME STREQUAL "." OR VST3SDK_FOLDER_NAME STREQUAL ".."))
    message(FATAL_ERROR "The folder name '${VST3SDK_FOLDER_NAME}' of VST3 SDK is invalid or empty.")
else()
    link_directories(${CMAKE_BINARY_DIR}/${VST3SDK_FOLDER_NAME}/build/lib)
endif()

add_subdirectory(${GKICK_VST_SDK_PATH} EXTERN)

set(GKICK_VST_HEADERS
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstcomponentbase.h
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstsinglecomponenteffect.h
  ${GKICK_VST_DIR}/VstIds.h
  ${GKICK_VST_DIR}/GKickVstProcessor.h
  ${GKICK_VST_DIR}/GKickVstEditor.h
  )

set(GKICK_VST_SOURCES
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstcomponentbase.cpp
  ${GKICK_VST_SDK_PATH}/public.sdk/source/vst/vstsinglecomponenteffect.cpp
  ${GKICK_VST_DIR}/VstFactory.cpp
  ${GKICK_VST_DIR}/GKickVstProcessor.cpp
  ${GKICK_VST_DIR}/GKickVstEditor.cpp
  )

set(target geonkick_vst)

link_directories(${PTHREAD_LIB_PATH})

add_library(${target} SHARED ${GKICK_VST_HEADERS} ${GKICK_VST_SOURCES})

set_target_properties(${target} PROPERTIES PREFIX "")
set_target_properties(${target} PROPERTIES OUTPUT_NAME "Geonkick")
if (CMAKE_SYSTEM_NAME MATCHES Windows)
    set_target_properties(${target} PROPERTIES SUFFIX ".dll")
else ()
    set_target_properties(${target} PROPERTIES SUFFIX ".so")
endif ()

target_link_libraries(${target} PRIVATE base sdk sdk_common pluginterfaces)
target_link_libraries(${target} PRIVATE geonkick_common api_plugin redkite)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} PRIVATE
      "libcairo.a"
      "libmsimg32.a"
      "libfontconfig.a"
      "libfreetype.a"
      "libpixman-1.a"
      "libpng.a"
      "libintl.a"
      "libiconv.a"
      "libexpat.a"
      "libbrotlidec.a"
      "libharfbuzz.a"
      "libbz2.a"
      "libbrotlicommon.a"
      "libgraphite2.a"
      "librpcrt4.a"
      "libsndfile.a"
      "libvorbis.a"
      "libvorbisenc.a"
      "libogg.a"
      "libFLAC.a"
      "libmp3lame.a"
      "libopus.a"
      "libmpg123.a"
      "libshlwapi.a"
      "libz.a"
      "libwinpthread.a"
      "libdwrite.a"
      "-static-libstdc++"
      "-static-libgcc"
      )
else()
      target_link_libraries(${target} PRIVATE "-lstdc++ -lstdc++fs -lsndfile -lcairo -lm -lpthread")
endif()

add_dependencies(${target} geonkick_common)

install(TARGETS geonkick_vst DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/vst3/Geonkick.vst3/Contents/${GKICK_ARCHITECTURE}-linux)
