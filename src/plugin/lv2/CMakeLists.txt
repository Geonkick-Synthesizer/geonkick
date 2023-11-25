set(GKICK_LV2_SOURCES ${GKICK_LV2_DIR}/geonkick_lv2.cpp)

add_library(geonkick_lv2 SHARED
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_RESOURCES}
  ${GKICK_LV2_SOURCES})

set_target_properties(geonkick_lv2 PROPERTIES PREFIX "")
target_link_libraries(geonkick_lv2 PRIVATE  geonkick_common api_plugin redkite)
target_link_libraries(geonkick_lv2 PRIVATE "-lstdc++fs")

if (CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(geonkick_lv2 PRIVATE
    "-mwindows"
    # Fix this in order to use static linking
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
else()
    target_link_libraries(geonkick_lv2 PRIVATE "-lX11 -lsndfile -lrt -lm -ldl -lpthread -lcairo")
endif()

add_dependencies(geonkick_lv2 api_plugin)
add_dependencies(geonkick_lv2 redkite)

add_library(geonkick_single_lv2 SHARED
    ${GKICK_COMMON_HEADERS}
    ${GKICK_COMMON_RESOURCES}
    ${GKICK_LV2_SOURCES})
set_target_properties(geonkick_single_lv2 PROPERTIES PREFIX "")
target_link_libraries(geonkick_single_lv2 PRIVATE  geonkick_common_single api_plugin_single redkite)
target_link_libraries(geonkick_single_lv2 PRIVATE "-lstdc++fs")

if (CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(geonkick_single_lv2 PRIVATE
    "-mwindows"
    # Fix this in order to use static linking
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
else()
  target_link_libraries(geonkick_single_lv2 PRIVATE "-lX11 -lsndfile -lrt -lm -ldl -lpthread -lcairo")
endif()

add_dependencies(geonkick_single_lv2 api_plugin_single)
target_compile_options(geonkick_single_lv2 PRIVATE PUBLIC ${GKICK_API_PLUGIN_SINGLE_FLAGS})
 
install(TARGETS geonkick_single_lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lv2/geonkick.lv2)
install(TARGETS geonkick_lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lv2/geonkick.lv2)
if (CMAKE_SYSTEM_NAME MATCHES Windows)
    install(DIRECTORY ${GKICK_LV2_DIR}/geonkick_win.lv2/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lv2/geonkick.lv2
    FILES_MATCHING PATTERN "*"
)
else()
    install(DIRECTORY ${GKICK_LV2_DIR}/geonkick.lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lv2)
endif()
