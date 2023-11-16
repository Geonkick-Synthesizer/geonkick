set(GKICK_VST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
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
link_directories(/ucrt64/lib)

set(THREADS_PREFER_PTHREAD_FLAG ON)

# Set the linker flags to link against static version of pthreads
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static-libwinpthread")

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
target_link_libraries(${target} PRIVATE
      "-mwindows"
# Fix this in order ot use static linking      
      "libcairo.dll.a"
      "libfontconfig.a"
      "libfreetype.a"
      "libpixman-1.a"
      "libpng.a"
      "libmsimg32.a"
      "libintl.a"
      "libiconv.a"
      "libexpat.a"
      "libbrotlidec.a"
      "libharfbuzz.a"
      "libz.a"
      "libbz2.a"
      "libbrotlicommon.a"
      "libgraphite2.a"
      "librpcrt4.a"
      "libsndfile.a"
      "libvorbis.a"
      "libvorbisenc.a"
      "libogg.a"
      "libFLAC.a"
      "libmpg123.a"
      "libmp3lame.a"
      "libopus.a"
      "libout123.a"
      "libmpg123.a"
      "libshlwapi.a"
      "-static-libgcc"
      "-static-libstdc++"
      "libm.a"
      Threads::Threads
      )

add_dependencies(${target} geonkick_common)

install(TARGETS geonkick_vst DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/vst3/Geonkick.vst3/Contents/${GKICK_ARCHITECTURE}-linux)
