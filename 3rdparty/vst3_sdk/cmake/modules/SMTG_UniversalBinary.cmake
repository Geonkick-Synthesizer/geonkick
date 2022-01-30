
if(SMTG_MAC)
    if(XCODE_VERSION VERSION_GREATER_EQUAL 12)
        option(SMTG_BUILD_UNIVERSAL_BINARY "Build universal binary (x86_64 & arm64)" ON)
    elseif(XCODE_VERSION VERSION_LESS 10)
        option(SMTG_BUILD_UNIVERSAL_BINARY "Build universal binary (32 & 64 bit)" OFF)
    else()
        # Xcode 10 and 11 can only build x86_64 for macOS
        set(SMTG_BUILD_UNIVERSAL_BINARY 0)
    endif()
endif(SMTG_MAC)

#------------------------------------------------------------------------
# smtg_target_setup_universal_binary
#------------------------------------------------------------------------
function(smtg_target_setup_universal_binary target)
    if(SMTG_MAC)
        if(SMTG_BUILD_UNIVERSAL_BINARY)
            if(XCODE_VERSION VERSION_GREATER_EQUAL 12)
                set_target_properties(${target}
                    PROPERTIES
                        XCODE_ATTRIBUTE_OSX_ARCHITECTURES   "x86_64;arm64;arm64e"
                        XCODE_ATTRIBUTE_ARCHS               "$(ARCHS_STANDARD_64_BIT)"
                )
            else()
                set_target_properties(${target}
                    PROPERTIES
                        XCODE_ATTRIBUTE_OSX_ARCHITECTURES   "x86_64;i386"
                        XCODE_ATTRIBUTE_ARCHS               "$(ARCHS_STANDARD_32_64_BIT)"
                )
            endif()
            set_target_properties(${target}
                PROPERTIES
                    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH        "$<$<CONFIG:Debug>:YES>$<$<CONFIG:Release>:NO>"
            )
        endif(SMTG_BUILD_UNIVERSAL_BINARY)
    endif(SMTG_MAC)
endfunction(smtg_target_setup_universal_binary)

#------------------------------------------------------------------------
# Deprecated since 3.7.4 -----------------------------
function(smtg_setup_universal_binary target)
    message(DEPRECATION "[SMTG] Use smtg_target_setup_universal_binary instead of smtg_setup_universal_binary")
    smtg_target_setup_universal_binary (${target})
endfunction(smtg_setup_universal_binary)
