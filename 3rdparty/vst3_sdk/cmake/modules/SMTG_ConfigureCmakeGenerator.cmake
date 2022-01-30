
#------------------------------------------------------------------------
# Specify Output directories
#
# XCode is creating the "Debug/Release" folder on its own and does not need to be added.
macro(smtg_specify_output_directories)
    if(SMTG_WIN OR (SMTG_MAC AND CMAKE_GENERATOR STREQUAL Xcode))
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    else ()
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
    endif(SMTG_WIN OR (SMTG_MAC AND CMAKE_GENERATOR STREQUAL Xcode))
endmacro(smtg_specify_output_directories)

#------------------------------------------------------------------------
# Specify build types
#
# Specifies what build types (configurations) will be available.
macro(smtg_specify_build_types)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

    # This variable is only meaningful to single-configuration generators (such as make and Ninja).
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
endmacro(smtg_specify_build_types)

#------------------------------------------------------------------------
# Group predefined cmake projects
#
# Put predefined targets like "ALL_BUILD" and "ZERO_CHECK" into a separate folder within the IDE.
macro(smtg_group_predefined_cmake_projects)
    set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER Predefined)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endmacro(smtg_group_predefined_cmake_projects)

#------------------------------------------------------------------------
# Configure CMake generator
macro(smtg_configure_cmake_generator)
    smtg_specify_output_directories()
    smtg_specify_build_types()
    smtg_group_predefined_cmake_projects()

    # Generates a compile_commands.json file containing the exact compiler calls.
    # (This option is implemented only by Makefile Generators and the Ninja. It 
    # is ignored on other generators.)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE STRING "Generate compile commands" FORCE)
endmacro(smtg_configure_cmake_generator)
