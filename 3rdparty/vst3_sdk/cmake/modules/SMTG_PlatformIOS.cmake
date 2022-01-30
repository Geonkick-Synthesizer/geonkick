
#------------------------------------------------------------------------
# Platform
#------------------------------------------------------------------------

#------------------------------------------------------------------------
function(smtg_target_set_platform_ios target)
    if(NOT SMTG_MAC)
        message(FATAL_ERROR "[SMTG] smtg_target_set_platform_ios only works on macOS, use it in an if(SMTG_MAC) block")
    endif(NOT SMTG_MAC)
    set_target_properties(${target}
        PROPERTIES
            XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET  "9.0"
            XCODE_ATTRIBUTE_SDKROOT                     "iphoneos"
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY      "1,2"
            XCODE_ATTRIBUTE_SUPPORTS_MACCATALYST        "NO"
    )
endfunction(smtg_target_set_platform_ios)

#------------------------------------------------------------------------
# Deprecated since 3.7.4 -----------------------------
function(smtg_set_platform_ios target)
    message(DEPRECATION "[SMTG] Use smtg_target_set_platform_ios instead of smtg_set_platform_ios")
    smtg_target_set_platform_ios (${target})
endfunction(smtg_set_platform_ios)
