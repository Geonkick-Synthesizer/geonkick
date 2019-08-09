install(DIRECTORY ${GKICK_DATA_DIR}/icons DESTINATION ${CMAKE_INSTALL_PREFIX}/share)
if (ENABLE_STANDALONE)
  install(FILES ${GKICK_DATA_DIR}/geonkick.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
endif()
install(FILES ${GKICK_DATA_DIR}/geonkick.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/mime/packages)
install(CODE "execute_process(COMMAND gtk-update-icon-cache --ignore-theme-index ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor)")
install(CODE "execute_process(COMMAND update-mime-database ${CMAKE_INSTALL_PREFIX}/share/mime)")

