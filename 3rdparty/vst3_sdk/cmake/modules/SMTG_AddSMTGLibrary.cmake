
#-------------------------------------------------------------------------------
# Includes
#-------------------------------------------------------------------------------
include(CMakePrintHelpers)
include(SMTG_UniversalBinary)
include(SMTG_CodeSign)
include(SMTG_SetDebugExecutable)

# Customize output location for binaries by setting SMTG_CUSTOM_BINARY_LOCATION via command line
# cmd: cmake -GXcode -DSMTG_CUSTOM_BINARY_LOCATION="MyPath"
set(SMTG_CUSTOM_BINARY_LOCATION "" CACHE PATH "Customize output location for binaries")

# define the C++ standard version to use for plugins
set(SMTG_CXX_STANDARD "" CACHE STRING "C++ standard version used for plugins: 14, 17, 20")

#------------------------------------------------------------------------
# Returns the windows architecture.
#
# This name will be used as a folder name inside the Plug-in package.
# The variable SMTG_WIN_ARCHITECTURE_NAME will be set.
function(smtg_target_set_vst_win_architecture_name target)
    if(SMTG_WIN)
        if(DEFINED CMAKE_GENERATOR_PLATFORM AND CMAKE_GENERATOR_PLATFORM)
            string(TOLOWER ${CMAKE_GENERATOR_PLATFORM} GENERATOR_PLATFORM)
        endif()

        if(${GENERATOR_PLATFORM} MATCHES "arm64*")
            set(WIN_ARCHITECTURE_NAME "arm_64")
        elseif(${GENERATOR_PLATFORM} MATCHES "arm")
            set(WIN_ARCHITECTURE_NAME "arm")
        elseif(${GENERATOR_PLATFORM} MATCHES "win32")
            set(WIN_ARCHITECTURE_NAME "x86")
        else()
            set(WIN_ARCHITECTURE_NAME "x86_64")
        endif()
        set(WIN_ARCHITECTURE_NAME ${WIN_ARCHITECTURE_NAME}-win)

        set_target_properties(${target}
            PROPERTIES
                SMTG_WIN_ARCHITECTURE_NAME ${WIN_ARCHITECTURE_NAME}
        )
    endif(SMTG_WIN)
endfunction(smtg_target_set_vst_win_architecture_name)

#------------------------------------------------------------------------
# Prints out all relevant properties of a target for debugging.
#
# @param target The target whose properties will be printed.
function(smtg_target_dump_plugin_package_variables target)
    cmake_print_properties(
        TARGETS
            ${target}
        PROPERTIES
            SMTG_PLUGIN_PACKAGE_NAME
            SMTG_PLUGIN_BINARY_DIR
            SMTG_PLUGIN_EXTENSION
            LIBRARY_OUTPUT_DIRECTORY
            SMTG_PLUGIN_PACKAGE_PATH
            SMTG_PLUGIN_PACKAGE_CONTENTS
            SMTG_PLUGIN_PACKAGE_RESOURCES
            SMTG_PLUGIN_PACKAGE_SNAPSHOTS
            SMTG_WIN_ARCHITECTURE_NAME
    )
endfunction(smtg_target_dump_plugin_package_variables)

#------------------------------------------------------------------------
# Strips all symbols on linux
#
# @param target The target whose build symbols will be stripped
function(smtg_target_strip_symbols target)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_STRIP} $ENV{CROSS_COMPILE} --strip-debug --strip-unneeded $<TARGET_FILE:${target}>
    )
endfunction(smtg_target_strip_symbols)

#------------------------------------------------------------------------
#! smtg_target_strip_symbols_with_dbg : Strips all symbols on and creates debug file on Linux 
#
# @param target The target whose build symbols will be stripped
function(smtg_target_strip_symbols_with_dbg target)
    add_custom_command(
        TARGET ${target} POST_BUILD
        # Create a target.so.dbg file with debug information
        COMMAND ${CMAKE_OBJECT_COPY} $ENV{CROSS_COMPILE}objcopy --only-keep-debug $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.dbg
        COMMAND ${CMAKE_STRIP} $ENV{CROSS_COMPILE} --strip-debug --strip-unneeded $<TARGET_FILE:${target}>
        COMMAND ${CMAKE_OBJECT_COPY} $ENV{CROSS_COMPILE}objcopy --add-gnu-debuglink=$<TARGET_FILE:${target}>.dbg $<TARGET_FILE:${target}>
    )
endfunction(smtg_target_strip_symbols_with_dbg)

#------------------------------------------------------------------------
# Creates a symlink to the targets output resp Plug-in.
#
# A symlink to the output Plug-in will be created as a post build step.
#
# @param target The target whose output is the symlink's source.
function(smtg_target_create_link_to_plugin target destination)
    if(${destination} STREQUAL "")
        message(FATAL_ERROR "[SMTG] no destination defined!")
    endif()

    get_target_property(PLUGIN_PACKAGE_NAME ${target} SMTG_PLUGIN_PACKAGE_NAME)
    get_target_property(TARGET_SOURCE       ${target} SMTG_PLUGIN_PACKAGE_PATH)
    set(TARGET_DESTINATION ${destination})

    if(SMTG_WIN)
        get_target_property(PLUGIN_BINARY_DIR ${target} SMTG_PLUGIN_BINARY_DIR)

        file(TO_NATIVE_PATH "${TARGET_DESTINATION}/${PLUGIN_PACKAGE_NAME}" SRC_NATIVE_PATH)
        file(TO_NATIVE_PATH "${PLUGIN_BINARY_DIR}/Debug/${PLUGIN_PACKAGE_NAME}" TARGET_DESTINATION_DEBUG)
        file(TO_NATIVE_PATH "${PLUGIN_BINARY_DIR}/Release/${PLUGIN_PACKAGE_NAME}" TARGET_DESTINATION_RELEASE)

        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND echo [SMTG] Delete previous link...
            COMMAND rmdir "${SRC_NATIVE_PATH}" & del "${SRC_NATIVE_PATH}"
            COMMAND echo [SMTG] Creation of the new link...
            COMMAND mklink /D
                ${SRC_NATIVE_PATH}
                "$<$<CONFIG:Debug>:${TARGET_DESTINATION_DEBUG}>"
                "$<$<CONFIG:Release>:${TARGET_DESTINATION_RELEASE}>"
            COMMAND echo [SMTG] Finished.
        )
    else()
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND mkdir -p "${TARGET_DESTINATION}"
            COMMAND ln -svfF "${TARGET_SOURCE}" "${TARGET_DESTINATION}"
        )
    endif(SMTG_WIN)
endfunction(smtg_target_create_link_to_plugin)

set(SMTG_DESKTOP_INI_PATH ${CMAKE_CURRENT_LIST_DIR}/../templates/desktop.ini.in)
get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
    set(SMTG_DESKTOP_INI_PATH ${SMTG_DESKTOP_INI_PATH} PARENT_SCOPE)
endif(hasParent)

#------------------------------------------------------------------------
# Customizes folder icon on windows
#
# Customizes folder icon on windows by copying desktop.ini and PlugIn.ico into the package.
#
# @param target The target whose folder icon will be customized.
function(smtg_target_add_folder_icon target icon)
    get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMENT "[SMTG] Copy PlugIn.ico and desktop.ini and change their attributes."
        COMMAND ${CMAKE_COMMAND} -E copy
            ${icon}
            ${PLUGIN_PACKAGE_PATH}/PlugIn.ico
        COMMAND ${CMAKE_COMMAND} -E copy
            ${SMTG_DESKTOP_INI_PATH}
            ${PLUGIN_PACKAGE_PATH}/desktop.ini
        COMMAND attrib +s ${PLUGIN_PACKAGE_PATH}/desktop.ini
        COMMAND attrib +s ${PLUGIN_PACKAGE_PATH}/PlugIn.ico
        COMMAND attrib +s ${PLUGIN_PACKAGE_PATH}
    )
endfunction(smtg_target_add_folder_icon)

#------------------------------------------------------------------------
# Adds the Plug-in's main entry point.
#
# The variable public_sdk_SOURCE_DIR needs to be specified.
#
# @param target The target to which the main entry point will be added.
function(smtg_target_add_library_main target)
    if(public_sdk_SOURCE_DIR)
        if(SMTG_MAC)
            target_sources (${target} 
                PRIVATE 
                    ${public_sdk_SOURCE_DIR}/source/main/macmain.cpp
            )
        elseif(SMTG_WIN)
            target_sources (${target} 
                PRIVATE 
                    ${public_sdk_SOURCE_DIR}/source/main/dllmain.cpp
            )
        elseif(SMTG_LINUX)
            target_sources (${target} 
                PRIVATE 
                    ${public_sdk_SOURCE_DIR}/source/main/linuxmain.cpp
            )
        endif(SMTG_MAC)
    endif(public_sdk_SOURCE_DIR)
endfunction(smtg_target_add_library_main)

#------------------------------------------------------------------------
# Returns the linux architecture name.
#
# This name will be used as a folder name inside the Plug-in package.
# The variable LINUX_ARCHITECTURE_NAME will be set.
function(smtg_get_linux_architecture_name)
    if(ANDROID)
        set(ARCHITECTURE ${CMAKE_ANDROID_ARCH_ABI})
        set(LINUX_ARCHITECTURE_NAME ${ARCHITECTURE}-android PARENT_SCOPE)
    else()
        # Check if we are cross-compiling for a different architecture
        if(NOT $ENV{CROSS_COMPILE} STREQUAL "")
            string(REGEX MATCH "^[A-Z|a-z|0-9|_]*-" CROSS_COMPILE_PREFIX $ENV{CROSS_COMPILE})
            # Strip trailing - from regex result
            string(LENGTH ${CROSS_COMPILE_PREFIX} CROSS_COMPILE_PREFIX_LEN)
            math(EXPR CROSS_COMPILE_PREFIX_LEN "${CROSS_COMPILE_PREFIX_LEN} -1")
            string(SUBSTRING ${CROSS_COMPILE_PREFIX} 0 ${CROSS_COMPILE_PREFIX_LEN} ARCHITECTURE)
        else()
            EXECUTE_PROCESS(
                COMMAND uname -m
                COMMAND tr -d '\n'
                OUTPUT_VARIABLE ARCHITECTURE
            )
        endif()
        set(LINUX_ARCHITECTURE_NAME ${ARCHITECTURE}-linux PARENT_SCOPE)
    endif(ANDROID)
endfunction(smtg_get_linux_architecture_name)

#------------------------------------------------------------------------
# Prepare the target to build a Plug-in package.
#
# @param target The target whose output will be put into a package.
# @param extension The package's extension
function(smtg_target_make_plugin_package target pkg_name extension)
    if(${pkg_name} STREQUAL "")
        set(pkg_name ${target})
    endif()
    set(pkg_extension ${extension})
    if(SMTG_WIN)
        if(${extension} STREQUAL "bundle")
            set(extension dll)
            if(NOT SMTG_CREATE_BUNDLE_FOR_WINDOWS)
                set(pkg_extension dll)
            endif(NOT SMTG_CREATE_BUNDLE_FOR_WINDOWS)
        endif()
    endif(SMTG_WIN)
    string(TOUPPER ${extension} PLUGIN_EXTENSION_UPPER)

    if(SMTG_CUSTOM_BINARY_LOCATION)
        set(SMTG_PLUGIN_BINARY_LOCATION ${SMTG_CUSTOM_BINARY_LOCATION})
    else()
        set(SMTG_PLUGIN_BINARY_LOCATION ${CMAKE_BINARY_DIR})
    endif(SMTG_CUSTOM_BINARY_LOCATION)

    set_target_properties(${target}
        PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY        ${SMTG_PLUGIN_BINARY_LOCATION}/${PLUGIN_EXTENSION_UPPER}
            SMTG_PLUGIN_BINARY_DIR          ${SMTG_PLUGIN_BINARY_LOCATION}/${PLUGIN_EXTENSION_UPPER}
            SMTG_PLUGIN_EXTENSION           ${extension}
            SMTG_PLUGIN_PACKAGE_NAME        ${pkg_name}.${pkg_extension}
            SMTG_PLUGIN_PACKAGE_CONTENTS    Contents
            SMTG_PLUGIN_PACKAGE_RESOURCES   Contents/Resources
            SMTG_PLUGIN_PACKAGE_SNAPSHOTS   Snapshots
    )

    get_target_property(PLUGIN_BINARY_DIR   ${target} SMTG_PLUGIN_BINARY_DIR)
    get_target_property(PLUGIN_EXTENSION    ${target} SMTG_PLUGIN_EXTENSION)
    get_target_property(PLUGIN_PACKAGE_NAME ${target} SMTG_PLUGIN_PACKAGE_NAME)

    smtg_target_add_library_main(${target})
    
    if(SMTG_CXX_STANDARD)
        target_compile_features(${target}
            PRIVATE
                cxx_std_${SMTG_CXX_STANDARD}
        )
    endif(SMTG_CXX_STANDARD)

    if(SMTG_MAC)
        set_target_properties(${target}
            PROPERTIES
                BUNDLE TRUE
        )
        if(XCODE)
            set_target_properties(${target}
                PROPERTIES 
                    XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE           YES
                    XCODE_ATTRIBUTE_WRAPPER_EXTENSION               ${PLUGIN_EXTENSION}
                    XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS  YES
                    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT        $<$<CONFIG:Debug>:dwarf>$<$<CONFIG:Release>:dwarf-with-dsym>
                    XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING		$<$<CONFIG:Debug>:NO>$<$<CONFIG:Release>:YES>
            )
            if(SMTG_CUSTOM_BINARY_LOCATION)
              set_target_properties(${target}
                PROPERTIES 
                    SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_BINARY_DIR}/${PLUGIN_PACKAGE_NAME}
              )
            else()
              set_target_properties(${target}
                PROPERTIES 
                    SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${PLUGIN_PACKAGE_NAME}
              )
            endif(SMTG_CUSTOM_BINARY_LOCATION)
        else()
            set_target_properties(${target}
                PROPERTIES 
                    BUNDLE_EXTENSION            ${PLUGIN_EXTENSION}
                    LIBRARY_OUTPUT_DIRECTORY    ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}
                    SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PLUGIN_PACKAGE_NAME}
            )
        endif(XCODE)

        target_link_libraries(${target}
            PRIVATE
                "-framework CoreFoundation"
        )
        smtg_target_setup_universal_binary(${target})
        smtg_target_codesign(${target} ${SMTG_IOS_DEVELOPMENT_TEAM} "${SMTG_CODE_SIGN_IDENTITY_MAC}")

    elseif(SMTG_WIN)
        if(SMTG_CUSTOM_BINARY_LOCATION)
            set(PLUGIN_PACKAGE_PATH ${PLUGIN_BINARY_DIR}/${PLUGIN_PACKAGE_NAME})
        else()
            set(PLUGIN_PACKAGE_PATH ${PLUGIN_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${PLUGIN_PACKAGE_NAME})
        endif(SMTG_CUSTOM_BINARY_LOCATION)
        set_target_properties(${target}
            PROPERTIES 
                SUFFIX                      .${PLUGIN_EXTENSION}
                SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_PACKAGE_PATH}
        )
        if(MSVC)
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set_target_properties(${target}
                    PROPERTIES 
                        LINK_FLAGS          /EXPORT:GetPluginFactory
                )
            endif()
        endif(MSVC)
        if(MINGW) # no "lib" prefix for MinGw
            set_target_properties(${target}
                PROPERTIES 
                    PREFIX                  ""
            )   
        endif(MINGW)
        
        # In order not to have the PDB inside the Plug-in package in release builds, 
        # we specify a different location (only for MSVC).
        if(MSVC)
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(WIN_PDB WIN_PDB32)
            else()
                set(WIN_PDB WIN_PDB64)
            endif()
            set_target_properties(${target}
                PROPERTIES
                    PDB_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${WIN_PDB}
            )
        endif(MSVC)

        # Create Bundle on Windows
        if(SMTG_CREATE_BUNDLE_FOR_WINDOWS)
            get_target_property(WIN_ARCHITECTURE_NAME ${target} SMTG_WIN_ARCHITECTURE_NAME)

            # When using LIBRARY_OUTPUT_DIRECTORY, cmake adds another /Debug resp. /Release folder at the end of the path.
            # In order to prevent cmake from doing that we set LIBRARY_OUTPUT_DIRECTORY_DEBUG and LIBRARY_OUTPUT_DIRECTORY_RELEASE
            # (or LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPERCASE}) directly.
            get_target_property(PLUGIN_PACKAGE_CONTENTS ${target} SMTG_PLUGIN_PACKAGE_CONTENTS)
            foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
                string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
                if(SMTG_CUSTOM_BINARY_LOCATION)
                    set_target_properties(${target}
                        PROPERTIES 
                            LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${PLUGIN_BINARY_DIR}/${PLUGIN_PACKAGE_NAME}/${PLUGIN_PACKAGE_CONTENTS}/${WIN_ARCHITECTURE_NAME}
                    )
                else()
                    set_target_properties(${target}
                        PROPERTIES 
                            LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${PLUGIN_BINARY_DIR}/${OUTPUTCONFIG}/${PLUGIN_PACKAGE_NAME}/${PLUGIN_PACKAGE_CONTENTS}/${WIN_ARCHITECTURE_NAME}
                    )
                endif(SMTG_CUSTOM_BINARY_LOCATION)
            endforeach()
            if(EXISTS ${SMTG_PACKAGE_ICON_PATH})
                smtg_target_add_folder_icon(${target} ${SMTG_PACKAGE_ICON_PATH})
            endif()
        endif(SMTG_CREATE_BUNDLE_FOR_WINDOWS)
        # Disable warning LNK4221: "This object file does not define any previously undefined public symbols...".
        # Enable "Generate Debug Information" in release config by setting "/Zi" and "/DEBUG" flags.
        if(MSVC)
            target_compile_options(${target} 
                PRIVATE 
                    /wd4221
                    $<$<CONFIG:RELEASE>:/Zi>
            )
            set_property(TARGET ${target} 
                APPEND PROPERTY 
                    LINK_FLAGS_RELEASE /DEBUG
            )
        endif(MSVC)
    elseif(SMTG_LINUX)
        smtg_get_linux_architecture_name() # Sets var LINUX_ARCHITECTURE_NAME
        message(STATUS "[SMTG] Linux architecture name is ${LINUX_ARCHITECTURE_NAME}.")

        get_target_property(PLUGIN_PACKAGE_CONTENTS ${target} SMTG_PLUGIN_PACKAGE_CONTENTS)
        set_target_properties(${target}
            PROPERTIES
                PREFIX                   ""
                LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PLUGIN_PACKAGE_NAME}/${PLUGIN_PACKAGE_CONTENTS}/${LINUX_ARCHITECTURE_NAME}
                SMTG_PLUGIN_PACKAGE_PATH ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PLUGIN_PACKAGE_NAME}
        )

        # Strip symbols in Release config
        if(${CMAKE_BUILD_TYPE} MATCHES Release)
            smtg_target_strip_symbols(${target})
        elseif(${CMAKE_BUILD_TYPE} MATCHES RelWithDebInfo)
            smtg_target_strip_symbols_with_dbg(${target})
        endif()
    endif(SMTG_MAC)
endfunction(smtg_target_make_plugin_package)

#------------------------------------------------------------------------
# Adds a resource for a target 
#
# The resource which gets copied into the targets Resource Bundle directory.
#
# @param target cmake target
# @param input_file resource file
# @param ARGV2 destination subfolder inside the Resource folder [optional]
function(smtg_target_add_plugin_resource target input_file)
    if(SMTG_LINUX OR (SMTG_WIN AND SMTG_CREATE_BUNDLE_FOR_WINDOWS))
        get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
        get_target_property(PLUGIN_PACKAGE_RESOURCES ${target} SMTG_PLUGIN_PACKAGE_RESOURCES)
        set(destination_folder "${PLUGIN_PACKAGE_PATH}/${PLUGIN_PACKAGE_RESOURCES}")
        if(ARGC GREATER 2 AND ARGV2)
            set(destination_folder "${destination_folder}/${ARGV2}")
        endif()

        # Make the incoming path absolute.
        get_filename_component(absolute_input_file_path "${input_file}" ABSOLUTE)

        add_custom_command(
            TARGET ${target} PRE_LINK
            COMMAND ${CMAKE_COMMAND} 
                -E make_directory 
                    "${destination_folder}"
            COMMAND ${CMAKE_COMMAND} 
                -E copy_if_different
                    ${absolute_input_file_path}
                    ${destination_folder}
            COMMAND ${CMAKE_COMMAND} 
                -E echo
                    "[SMTG] Copied ${input_file} to ${destination_folder}"
        )
    elseif(SMTG_MAC)
        target_sources(${target}
            PRIVATE
                ${input_file}
        )
        set(destination_folder "Resources")
        if(ARGC GREATER 2 AND ARGV2)
            set(destination_folder "${destination_folder}/${ARGV2}")
        endif()

        set_source_files_properties(${input_file} 
            PROPERTIES 
                MACOSX_PACKAGE_LOCATION "${destination_folder}"
        )
    endif(SMTG_LINUX OR (SMTG_WIN AND SMTG_CREATE_BUNDLE_FOR_WINDOWS))
endfunction(smtg_target_add_plugin_resource)

#------------------------------------------------------------------------
# Adds multiple snapshots to target.
#
# Usage:
#  smtg_target_add_plugin_snapshots (target
#    RESOURCES
#      bitmap0.png
#      bitmap1.png
#  )
# This adds both bitmaps to <Bundle>/Resources/Snapshots
#
# @param target The target to which the resources will be added. 
function(smtg_target_add_plugin_snapshots target)
    cmake_parse_arguments(
        PARSED_ARGS # Prefix of output variables e.g. PARSED_ARGS_RESOURCES
        ""          # List of names for boolean arguments
        ""          # List of names for mono-valued arguments
        "RESOURCES" # List of names for multi-valued arguments resp. lists
        ${ARGN}     # Arguments of the function to parse
    )
    get_target_property(PLUGIN_PACKAGE_SNAPSHOTS ${target} SMTG_PLUGIN_PACKAGE_SNAPSHOTS)
  
    foreach(rsrc ${PARSED_ARGS_RESOURCES})
        smtg_target_add_plugin_resource (${target} ${rsrc} ${PLUGIN_PACKAGE_SNAPSHOTS}) 
    endforeach()
endfunction(smtg_target_add_plugin_snapshots)

#------------------------------------------------------------------------
# Adds multiple resources to target.
#
# Usage:
#  smtg_target_add_plugin_resources (target
#    RESOURCES
#      bitmap0.png
#      bitmap1.png
#    OUTPUT_SUBDIRECTORY
#      Graphics
#  )
# This adds both bitmaps to <Bundle>/Resources/Graphics
#
# @param target The target to which the resources will be added. 
function(smtg_target_add_plugin_resources target)
    cmake_parse_arguments(
        PARSED_ARGS # Prefix of output variables e.g. PARSED_ARGS_RESOURCES
        ""          # List of names for boolean arguments
        "OUTPUT_SUBDIRECTORY" # List of names for mono-valued arguments
        "RESOURCES" # List of names for multi-valued arguments resp. lists
        ${ARGN}     # Arguments of the function to parse
    )

    foreach(rsrc ${PARSED_ARGS_RESOURCES})
        smtg_target_add_plugin_resource (${target} ${rsrc} ${PARSED_ARGS_OUTPUT_SUBDIRECTORY}) 
    endforeach()
endfunction(smtg_target_add_plugin_resources)

#------------------------------------------------------------------------
# Deprecated since 3.7.4 -----------------------------
function(smtg_add_folder_icon target icon)
    message(DEPRECATION "[SMTG] Use smtg_target_add_folder_icon instead of smtg_add_folder_icon")
    smtg_target_add_folder_icon (${target} ${icon})
endfunction(smtg_add_folder_icon)

# Deprecated since 3.7.4 -----------------------------
function(smtg_create_link_to_plugin target)
    message(DEPRECATION "[SMTG] Use smtg_target_create_link_to_plugin instead of smtg_create_link_to_plugin")
    smtg_get_default_plugin_path()
    smtg_target_create_link_to_plugin(${target} ${SMTG_PLUGIN_TARGET_DEFAULT_PATH})
endfunction(smtg_create_link_to_plugin)

# Deprecated since 3.7.4 -----------------------------
function(smtg_make_plugin_package target pkg_name extension)
    message(DEPRECATION "[SMTG] Use smtg_target_make_plugin_package instead of smtg_make_plugin_package")
    smtg_target_make_plugin_package (${target} ${pkg_name} ${extension})
endfunction(smtg_make_plugin_package)

# Deprecated since 3.7.4 -----------------------------
function(smtg_strip_symbols target)
    message(DEPRECATION "[SMTG] Use smtg_target_strip_symbols instead of smtg_strip_symbols")
    smtg_target_strip_symbols (${target})
endfunction(smtg_strip_symbols)

# Deprecated since 3.7.4 -----------------------------
function(smtg_add_library_main target)
    message(DEPRECATION "[SMTG] Use smtg_target_add_library_main instead of smtg_add_library_main")
    smtg_target_add_library_main (${target})
endfunction(smtg_add_library_main)

# Deprecated since 3.7.4 -----------------------------
function(smtg_strip_symbols_with_dbg target)
    message(DEPRECATION "[SMTG] Use smtg_target_strip_symbols_with_dbg instead of smtg_strip_symbols_with_dbg")
    smtg_target_strip_symbols_with_dbg (${target})
endfunction(smtg_strip_symbols_with_dbg)

# Deprecated since 3.7.4 -----------------------------
function(smtg_dump_plugin_package_variables target)
    message(DEPRECATION "[SMTG] Use smtg_target_dump_plugin_package_variables instead of smtg_dump_plugin_package_variables")
    smtg_target_dump_plugin_package_variables (${target})
endfunction(smtg_dump_plugin_package_variables)

# Deprecated since 3.7.4 -----------------------------
function(smtg_add_plugin_resource target input_file)
    message(DEPRECATION "[SMTG] Use smtg_target_add_plugin_resource instead of smtg_add_plugin_resource")
    smtg_target_add_plugin_resource (${target} ${input_file})
endfunction(smtg_add_plugin_resource)

# Deprecated since 3.7.4 -----------------------------
function(smtg_add_plugin_snapshot target snapshot)
    message(DEPRECATION "[SMTG] Use smtg_target_add_plugin_snapshots instead of smtg_add_plugin_snapshot")
    smtg_target_add_plugin_snapshots (${target}
        RESOURCES
            ${snapshot}
    )
endfunction(smtg_add_plugin_snapshot)