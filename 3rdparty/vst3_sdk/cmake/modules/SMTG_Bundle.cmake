
#------------------------------------------------------------------------
# Includes
#------------------------------------------------------------------------
include(CMakeParseArguments)

#------------------------------------------------------------------------
# Bundle configuration
#------------------------------------------------------------------------

#------------------------------------------------------------------------
function(smtg_target_set_bundle target)
    if(NOT SMTG_MAC)
        message(FATAL_ERROR "[SMTG] smtg_target_set_bundle only works on macOS, use it in an if(SMTG_MAC) block")
    endif(NOT SMTG_MAC)

    cmake_parse_arguments(ARG "PREPROCESS" "EXTENSION;INFOPLIST" "RESOURCES;PREPROCESSOR_FLAGS;BUNDLE_IDENTIFIER" ${ARGN})
    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "[SMTG] The following parameters are unrecognized: ${ARG_UNPARSED_ARGUMENTS}")
    endif(ARG_UNPARSED_ARGUMENTS)

    # Adding the bundle resources to the target sources creates a warning, see https://cmake.org/Bug/view.php?id=15272
    target_sources(${target} 
        PRIVATE
            ${ARG_RESOURCES}
    )

    get_target_property(type ${target} TYPE)
    if(type STREQUAL MODULE_LIBRARY)
        set_target_properties(${target}
            PROPERTIES
                BUNDLE TRUE
        )
    elseif(type STREQUAL EXECUTABLE)
        set_target_properties(${target}
            PROPERTIES
                MACOSX_BUNDLE TRUE
        )
    endif()

    if(ARG_EXTENSION)
        if(XCODE)
            set_target_properties(${target}
                PROPERTIES
                    XCODE_ATTRIBUTE_WRAPPER_EXTENSION ${ARG_EXTENSION}
            )
        else()
            set_target_properties(${target}
                PROPERTIES
                    BUNDLE_EXTENSION ${ARG_EXTENSION}
            )
        endif(XCODE)
    endif(ARG_EXTENSION)

    if(ARG_RESOURCES)
        set_source_files_properties(${ARG_RESOURCES}
            PROPERTIES
                MACOSX_PACKAGE_LOCATION Resources
        )
    endif(ARG_RESOURCES)

    if(ARG_INFOPLIST)
        if(XCODE)
            set_target_properties(${target}
                PROPERTIES
                    XCODE_ATTRIBUTE_INFOPLIST_FILE "${ARG_INFOPLIST}"
            )
            if(ARG_PREPROCESS)
                set_target_properties(${target}
                    PROPERTIES
                        XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS "YES"
                )
            endif(ARG_PREPROCESS)
            if(ARG_PREPROCESSOR_FLAGS)
                set_target_properties(${target} 
                    PROPERTIES
                        XCODE_ATTRIBUTE_INFOPLIST_OTHER_PREPROCESSOR_FLAGS "${ARG_PREPROCESSOR_FLAGS}"
                )
            endif(ARG_PREPROCESSOR_FLAGS)
        endif(XCODE)
    endif(ARG_INFOPLIST)

	if(ARG_BUNDLE_IDENTIFIER)
        if(XCODE)
        	set_target_properties(${target}
                PROPERTIES
                    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${ARG_BUNDLE_IDENTIFIER}"
            )
        endif(XCODE)
	endif(ARG_BUNDLE_IDENTIFIER)
endfunction(smtg_target_set_bundle)

#------------------------------------------------------------------------
# Deprecated since 3.7.4 -----------------------------
function(smtg_set_bundle target)
    message(DEPRECATION "[SMTG] Use smtg_target_set_bundle instead of smtg_set_bundle")
    smtg_target_set_bundle (${target})
endfunction(smtg_set_bundle)
