install(DIRECTORY ${GKICK_DATA_DIR}/icons DESTINATION ${CMAKE_INSTALL_PREFIX}/share)
if (GKICK_STANDALONE)
  install(FILES ${GKICK_DATA_DIR}/geonkick.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
endif()
install(FILES ${GKICK_DATA_DIR}/geonkick.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/mime/packages)

