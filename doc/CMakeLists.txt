find_program(PANDOC_EXECUTABLE pandoc)

if(NOT PANDOC_EXECUTABLE)
    message(FATAL_ERROR "Pandoc not found. Please install Pandoc to generate documentation.")
endif()

set(MD_FILE ${GKICK_DOC_DIR}/Geonkick_User_Guide.md)
set(PDF_OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/Geonkick_User_Guide.pdf)
set(HTML_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/Geonkick_User_Guide_HTML)

file(MAKE_DIRECTORY ${HTML_OUTPUT_DIR})

configure_file(${GKICK_DOC_DIR}/diagram.png ${HTML_OUTPUT_DIR}/diagram.png COPYONLY)
configure_file(${GKICK_DOC_DIR}/diagram.png ${CMAKE_CURRENT_BINARY_DIR}/diagram.png COPYONLY)

add_custom_command(
    OUTPUT ${PDF_OUTPUT_FILE}
    COMMAND ${PANDOC_EXECUTABLE} ${MD_FILE} --metadata title="Geonkick User Guide" -o ${PDF_OUTPUT_FILE}
    DEPENDS ${MD_FILE} ${CMAKE_CURRENT_BINARY_DIR}/diagram.png
    COMMENT "Generating PDF documentation"
    )

add_custom_command(
    OUTPUT ${HTML_OUTPUT_DIR}/index.html
    COMMAND ${PANDOC_EXECUTABLE} ${MD_FILE} --metadata title="Geonkick User Guide" -s -o ${HTML_OUTPUT_DIR}/index.html
    DEPENDS ${MD_FILE} ${GKICK_DOC_DIR}/diagram.png
    COMMENT "Generating HTML documentation"
)

add_custom_target(
    GenerateDocumentation ALL
    DEPENDS ${PDF_OUTPUT_FILE} ${HTML_OUTPUT_DIR}/index.html
)

install(FILES ${PDF_OUTPUT_FILE} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/geonkick)
install(DIRECTORY ${HTML_OUTPUT_DIR} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/geonkick)
