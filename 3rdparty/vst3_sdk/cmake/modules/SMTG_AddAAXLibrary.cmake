
#------------------------------------------------------------------------
# Includes
#------------------------------------------------------------------------
include(SMTG_AddSMTGLibrary)

#------------------------------------------------------------------------
# Returns the windows architecture.
#
# This name will be used as a folder name inside the Plug-in package.
# The variable SMTG_WIN_ARCHITECTURE_NAME will be set.
function(smtg_target_set_aax_win_architecture_name target)
    if(${GENERATOR_PLATFORM} MATCHES "win32")
        set(WIN_ARCHITECTURE_NAME "win32")
    else()
        set(WIN_ARCHITECTURE_NAME "x64")    
    endif()

    set_target_properties(${target}
        PROPERTIES
            SMTG_WIN_ARCHITECTURE_NAME ${WIN_ARCHITECTURE_NAME}           
    )
endfunction(smtg_target_set_aax_win_architecture_name)

#------------------------------------------------------------------------
# Adds a AAX target.
#
# @param target The target to which a AAX Plug-in will be added. 
# @param pkg_name name of the created package
# for example: 
# smtg_add_aaxplugin_with_pkgname(${target} "A Gain" ${again_sources})
function(smtg_add_aaxplugin_with_pkgname target pkg_name)
    add_library(${target} MODULE ${ARGN})
    smtg_target_set_aax_win_architecture_name(${target})
    smtg_target_make_plugin_package(${target} ${pkg_name} aaxplugin)
    # smtg_dump_plugin_package_variables(${target})
    if(SMTG_MAC AND XCODE)
        # disable automatic signing for AAX
        set_target_properties(${target}
            PROPERTIES
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        )
    endif(SMTG_MAC AND XCODE)
endfunction(smtg_add_aaxplugin_with_pkgname)

#------------------------------------------------------------------------
# Adds a AAX target.
#
# @param target The target to which a AAX Plug-in will be added.
# @param PACKAGE_NAME <Name> The name of the package [optional] if not present
# @param SOURCES_LIST <sources> List of sources to add to target project [mandatory when PACKAGE_NAME is used]
# for example: 
# smtg_add_aaxplugin(${target} PACKAGE_NAME "A Gain" SOURCES_LIST ${again_sources})
# or
# smtg_add_aaxplugin(${target} ${again_sources})
function(smtg_add_aaxplugin target)
    set(oneValueArgs PACKAGE_NAME)
    set(sourcesArgs SOURCES_LIST)
    cmake_parse_arguments(PARSE_ARGV 0 PARAMS "${options}" "${oneValueArgs}" "${sourcesArgs}")

    set(SOURCES "${PARAMS_SOURCES_LIST}")
    if(SOURCES STREQUAL "")
        set(SOURCES ${ARGN})
    endif()

    set(pkg_name "${PARAMS_PACKAGE_NAME}")
    if(pkg_name STREQUAL "")
        set(pkg_name ${target})
    endif()

    smtg_add_aaxplugin_with_pkgname(${target} ${pkg_name} ${SOURCES})
endfunction(smtg_add_aaxplugin)
