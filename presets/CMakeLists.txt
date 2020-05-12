set(GKICK_PRESETS
  AVL-Drumkit.tar.xz
  DigitalSamba.tar.xz
  DSoS.tar.xz
  TimeKit.tar.xz
  UnfaTutorial.tar.xz)

foreach(compressedFolder ${GKICK_PRESETS})
  message(STATUS "uncompress file ${compressedFolder}")
  get_filename_component(presetsFolder ${compressedFolder} NAME)
  string(REPLACE ".tar.xz" "" presetsFolder ${presetsFolder})
  add_custom_command(OUTPUT ${presetsFolder}
    COMMAND tar xvf ${GKICK_PRESETS_DIR}/${compressedFolder})
  set(GKICK_PRESETS_OUTPUT ${GKICK_PRESETS_OUTPUT} ${CMAKE_BINARY_DIR}/presets/${presetsFolder})
endforeach()

add_custom_target(gkick_presets ALL DEPENDS ${GKICK_PRESETS_OUTPUT})
install(DIRECTORY ${GKICK_PRESETS_OUTPUT} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/geonkick/presets)
