
#------------------------------------------------------------------------
# Includes
#------------------------------------------------------------------------
include(SMTG_AddSMTGLibrary)

set(SMTG_RUN_VST_VALIDATOR_DEFAULT ON)

# Run the Validator after each new compilation of VST3 Plug-ins
option(SMTG_RUN_VST_VALIDATOR "Run VST validator on VST3 Plug-ins" ${SMTG_RUN_VST_VALIDATOR_DEFAULT})

#------------------------------------------------------------------------
# Runs the validator on a VST3 target.
#
# The validator will be run on the target as a post build step.
#
# @param target The target which the validator will validate. 
function(smtg_target_run_vst_validator target)
    if(TARGET validator)
        message(STATUS "[SMTG] Setup running validator for ${target}")
        add_dependencies(${target} validator)
        if(SMTG_WIN)
            set(TARGET_PATH $<TARGET_FILE:${target}>)
            add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND echo [SMTG] Validator started...
                COMMAND 
                    $<TARGET_FILE:validator> 
                    "${TARGET_PATH}" 
                    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
                COMMAND echo [SMTG] Validator finished.
            )
        else()
            get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
            add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND 
                    $<TARGET_FILE:validator> 
                    $<$<CONFIG:Debug>:${PLUGIN_PACKAGE_PATH}>
                    $<$<CONFIG:Release>:${PLUGIN_PACKAGE_PATH}> 
                    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            )
        endif(SMTG_WIN)
    endif(TARGET validator)
endfunction(smtg_target_run_vst_validator)

#------------------------------------------------------------------------
# Set SMTG_PLUGIN_TARGET_PATH to default VST3 Path
function(smtg_get_vst3_path)
    # here you can define the VST3 Plug-ins folder (it will be created), SMTG_PLUGIN_TARGET_DEFAULT_PATH will be set
    smtg_get_default_vst3_path()

    set(SMTG_PLUGIN_TARGET_USER_PATH "" CACHE PATH "Here you can redefine the VST3 Plug-ins folder")
    if(NOT ${SMTG_PLUGIN_TARGET_USER_PATH} STREQUAL "")
        set(PLUGIN_TARGET_PATH ${SMTG_PLUGIN_TARGET_USER_PATH})
    else()
        set(PLUGIN_TARGET_PATH ${SMTG_PLUGIN_TARGET_DEFAULT_PATH})
    endif()
    
    if(NOT ${PLUGIN_TARGET_PATH} STREQUAL "" AND SMTG_CREATE_PLUGIN_LINK)
        if(NOT EXISTS ${PLUGIN_TARGET_PATH})
            message(STATUS "[SMTG] Create folder: " ${PLUGIN_TARGET_PATH})
            if(SMTG_WIN)
                smtg_create_directory_as_admin_win(${PLUGIN_TARGET_PATH})
            else()
                file(MAKE_DIRECTORY ${PLUGIN_TARGET_PATH})
            endif(SMTG_WIN)
        endif(NOT EXISTS ${PLUGIN_TARGET_PATH})
    endif(NOT ${PLUGIN_TARGET_PATH} STREQUAL "" AND SMTG_CREATE_PLUGIN_LINK)
    if(EXISTS ${PLUGIN_TARGET_PATH})
        message(STATUS "[SMTG] SMTG_PLUGIN_TARGET_PATH is set to: " ${PLUGIN_TARGET_PATH})
    else()
        message(STATUS "[SMTG] SMTG_PLUGIN_TARGET_PATH is not set!")
    endif(EXISTS ${PLUGIN_TARGET_PATH})
    set(SMTG_PLUGIN_TARGET_PATH ${PLUGIN_TARGET_PATH} PARENT_SCOPE)
endfunction(smtg_get_vst3_path)

#------------------------------------------------------------------------
# Adds a VST3 target.
#
# @param target The target to which a VST3 Plug-in will be added.
# @param pkg_name name of the created package
function(smtg_add_vst3plugin_with_pkgname target pkg_name)
   #message(STATUS "[SMTG] target is ${target}")
   #message(STATUS "[SMTG] pkg_name is ${pkg_name}")

    if(NOT EXISTS ${SMTG_PACKAGE_ICON_PATH})
        set(SMTG_PACKAGE_ICON_PATH ${SDK_ROOT}/vst3_doc/artwork/VST_Logo_Steinberg.ico)
    endif()
   #message(STATUS "[SMTG] SMTG_PACKAGE_ICON_PATH is ${SMTG_PACKAGE_ICON_PATH}")

    add_library(${target} MODULE ${ARGN})
    smtg_target_set_vst_win_architecture_name(${target})
    smtg_target_make_plugin_package(${target} ${pkg_name} vst3)

    if(SMTG_ENABLE_TARGET_VARS_LOG)
        smtg_target_dump_plugin_package_variables(${target})
    endif(SMTG_ENABLE_TARGET_VARS_LOG)

    if(SMTG_RUN_VST_VALIDATOR)
        smtg_target_run_vst_validator(${target})
    endif(SMTG_RUN_VST_VALIDATOR)

    if(SMTG_CREATE_PLUGIN_LINK)
        smtg_get_vst3_path()
        smtg_target_create_link_to_plugin(${target} ${SMTG_PLUGIN_TARGET_PATH})
    endif(SMTG_CREATE_PLUGIN_LINK)

endfunction(smtg_add_vst3plugin_with_pkgname)

#------------------------------------------------------------------------
# Adds a VST3 target.
#
# @param target The target to which a VST3 Plug-in will be added. 
# @param PACKAGE_NAME <Name> The name of the package [optional] if not present
# @param SOURCES_LIST <sources> List of sources to add to target project [mandatory when PACKAGE_NAME is used]
# for example: 
# smtg_add_vst3plugin(${target} PACKAGE_NAME "A Gain" SOURCES_LIST ${again_sources})
# or
# smtg_add_vst3plugin(${target} ${again_sources})
function(smtg_add_vst3plugin target)
    set(oneValueArgs PACKAGE_NAME)
    set(sourcesArgs SOURCES_LIST)
    cmake_parse_arguments(PARSE_ARGV 0 PARAMS "${options}" "${oneValueArgs}" "${sourcesArgs}")
   
    #message(STATUS "[SMTG] PARAMS_UNPARSED_ARGUMENTS is ${PARAMS_UNPARSED_ARGUMENTS}")
    #message(STATUS "[SMTG] PARAMS_PACKAGE_NAME is ${PARAMS_PACKAGE_NAME}")
    #message(STATUS "[SMTG] PARAM_SOURCES is ${PARAMS_SOURCES_NAME}")
    
    set(SOURCES "${PARAMS_SOURCES_LIST}")
    if(SOURCES STREQUAL "")
        set(SOURCES ${ARGN})
    endif()
   
    set(pkg_name "${PARAMS_PACKAGE_NAME}")
    if(pkg_name STREQUAL "")
        set(pkg_name ${target})
    endif()

    #message(STATUS "[SMTG] target is ${target}.")
    #message(STATUS "[SMTG] pkg_name is ${pkg_name}.")
    #message(STATUS "[SMTG] SOURCES is ${SOURCES}.")
  
    smtg_add_vst3plugin_with_pkgname(${target} ${pkg_name} ${SOURCES})
endfunction(smtg_add_vst3plugin)

#------------------------------------------------------------------------
# Adds a VST3 target for iOS
#
# @param sign_identity which code signing identity to use
# @param target The target to which a VST3 Plug-in will be added.
function(smtg_add_ios_vst3plugin target sign_identity pkg_name)
    if(SMTG_MAC AND XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)
        add_library(${target} MODULE ${ARGN})
        smtg_target_make_plugin_package(${target} ${pkg_name} vst3)

        if(SMTG_ENABLE_TARGET_VARS_LOG)
            smtg_target_dump_plugin_package_variables(${target})
        endif(SMTG_ENABLE_TARGET_VARS_LOG)

        smtg_target_set_platform_ios(${target})
        set_target_properties(${target}
            PROPERTIES 
                XCODE_ATTRIBUTE_DEVELOPMENT_TEAM    ${SMTG_IOS_DEVELOPMENT_TEAM}
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY  "${SMTG_CODE_SIGN_IDENTITY_IOS}"
                XCODE_ATTRIBUTE_ENABLE_BITCODE      "NO"
        )

        get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMENT "[SMTG] Codesign"            
            COMMAND codesign --force --verbose --sign "${sign_identity}"
                "${PLUGIN_PACKAGE_PATH}"
        )

    endif(SMTG_MAC AND XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)
endfunction(smtg_add_ios_vst3plugin)

#------------------------------------------------------------------------
# Deprecated since 3.7.4 -----------------------------
function(smtg_run_vst_validator target)
    message(DEPRECATION "[SMTG] Use smtg_target_run_vst_validator instead of smtg_run_vst_validator")
    smtg_target_run_vst_validator (${target})
endfunction(smtg_run_vst_validator)

# Deprecated since 3.7.4 -----------------------------
function(smtg_add_vst3_resource target input_file)
    message(DEPRECATION "[SMTG] Use smtg_target_add_plugin_resources instead of smtg_add_vst3_resource")
    smtg_target_add_plugin_resources (${target}
        RESOURCES
            ${input_file}
    )
endfunction(smtg_add_vst3_resource)

# Deprecated since 3.7.4 -----------------------------
function(smtg_add_vst3_snapshot target snapshot)
    message(DEPRECATION "[SMTG] Use smtg_target_add_plugin_snapshots instead of smtg_add_vst3_snapshot")
    smtg_target_add_plugin_snapshots (${target}
        RESOURCES
            ${snapshot}
    )
endfunction(smtg_add_vst3_snapshot)
