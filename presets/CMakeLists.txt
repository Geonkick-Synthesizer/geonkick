set(GKICK_PRESETS
  GeonkickBells.tar.xz
  AVL-Drumkit.tar
  DigitalSamba.tar.xz
  DSoS.tar.xz
  TimeKit.tar.xz
  UnfaTutorial.tar.xz
  HaraldReveryKicks.tar.xz
  SPKickPresetsVol1.tar.xz)

message(STATUS "GKICK_PRESETS ${GKICK_PRESETS}") 

foreach(compressedFolder ${GKICK_PRESETS})
  message(STATUS "uncompress file ${compressedFolder}")
  
  get_filename_component(presetsFolder ${compressedFolder} NAME_WE)
  set(outputDir "${CMAKE_BINARY_DIR}/presets/${presetsFolder}")

  add_custom_command(
    OUTPUT ${outputDir}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${outputDir}
    COMMAND ${CMAKE_COMMAND} -E tar xvf "${GKICK_PRESETS_DIR}/${compressedFolder}"
    COMMENT "Extracting ${compressedFolder}"
  )

  set(GKICK_PRESETS_OUTPUT ${GKICK_PRESETS_OUTPUT} ${outputDir})
endforeach()

add_custom_target(gkick_presets ALL DEPENDS ${GKICK_PRESETS_OUTPUT})
install(DIRECTORY ${GKICK_PRESETS_OUTPUT} DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/geonkick/presets)
