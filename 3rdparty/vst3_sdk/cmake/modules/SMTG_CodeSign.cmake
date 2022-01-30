
if(XCODE)
    option(SMTG_DISABLE_CODE_SIGNING "Disable All Code Signing" OFF)
    set(SMTG_XCODE_OTHER_CODE_SIGNING_FLAGS "--timestamp" CACHE STRING "Other code signing flags [Xcode]")
    option(SMTG_XCODE_MANUAL_CODE_SIGN_STYLE "Manual Xcode sign style" OFF)
endif()

#------------------------------------------------------------------------
function(smtg_target_codesign target)
    if(XCODE AND (NOT SMTG_DISABLE_CODE_SIGNING))
        if(ARGC GREATER 2)
            set(team "${ARGV1}")
            set(identity "${ARGV2}")
            message(STATUS "[SMTG] Codesign ${target} with team '${team}' and identity '${identity}")
            set(SMTG_CODESIGN_ATTRIBUTES 
                XCODE_ATTRIBUTE_DEVELOPMENT_TEAM    ${team}
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY  "${identity}"
                XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "${SMTG_XCODE_OTHER_CODE_SIGNING_FLAGS}"
            )
        else()
            message(STATUS "[SMTG] Codesign ${target} for local machine only")
            set(SMTG_CODESIGN_ATTRIBUTES 
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
            )
        endif(ARGC GREATER 2)
        set_target_properties(${target}
            PROPERTIES
                ${SMTG_CODESIGN_ATTRIBUTES}
        )
		if(SMTG_XCODE_MANUAL_CODE_SIGN_STYLE)
			set_target_properties(${target}
				PROPERTIES
					XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
			)
		endif(SMTG_XCODE_MANUAL_CODE_SIGN_STYLE)
		if(SMTG_MAC AND (XCODE_VERSION VERSION_GREATER_EQUAL 12))
			# make sure that the executable is signed before cmake post build commands are run as the
			# Xcode code-sign step is run after the post build commands are run which would prevent
			# using the target output on system where everything needs to be code-signed.
			target_link_options(${target} PRIVATE LINKER:-adhoc_codesign)
		endif()
    endif(XCODE AND (NOT SMTG_DISABLE_CODE_SIGNING))
endfunction(smtg_target_codesign)

#------------------------------------------------------------------------
# Deprecated since 3.7.4 -----------------------------
function(smtg_codesign_target target)
    message(DEPRECATION "[SMTG] Use smtg_target_codesign instead of smtg_codesign_target")
    smtg_target_codesign (${target})
endfunction(smtg_codesign_target)
