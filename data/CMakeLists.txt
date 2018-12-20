install(DIRECTORY ${GKICK_DATA_DIR}/icons DESTINATION /usr/share)
install(FILES ${GKICK_DATA_DIR}/geonkick.desktop DESTINATION /usr/share/applications)
install(FILES ${GKICK_DATA_DIR}/geonkick.xml DESTINATION /usr/share/mime/packages)
install(CODE "execute_process(COMMAND gtk-update-icon-cache /usr/share/icons/hicolor)")

