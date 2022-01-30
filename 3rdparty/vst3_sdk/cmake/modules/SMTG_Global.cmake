
#------------------------------------------------------------------------
# Includes
#------------------------------------------------------------------------
include(SMTG_DetectPlatform)
include(SMTG_ConfigureCmakeGenerator)
include(SMTG_ExportedSymbols)

# Check for in-source build directory.
#
# In order not to pollute the source directory, using a separate build directory is mandatory.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR OR EXISTS "${CMAKE_BINARY_DIR}/CMakeLists.txt")
    message(SEND_ERROR "[SMTG] In-source builds are not allowed. Please create a separate build directory and run 'cmake /path/to/folder [options]' there.")
    message(FATAL_ERROR "[SMTG] You can remove the file \"CMakeCache.txt\" and directory \"CMakeFiles\" in ${CMAKE_SOURCE_DIR}.")
endif()

#------------------------------------------------------------------------
# Global setup functions/macros
#
# Call macros/functions which setup the SDK's basics.
smtg_detect_platform()
smtg_configure_cmake_generator()
smtg_detect_xcode_version()

#------------------------------------------------------------------------
# Enable VST3 SDK
#
# This macro should be called from a 3rd party's plug-in CMakeLists.txt file.
macro(smtg_enable_vst3_sdk)
    smtg_configure_cmake_generator()
    smtg_setup_platform_toolset()
    smtg_setup_symbol_visibility()
    
    # Needs to bet set for 'smtg_add_library_main' function.
    if(DEFINED vst3sdk_SOURCE_DIR)
        set(public_sdk_SOURCE_DIR ${vst3sdk_SOURCE_DIR}/public.sdk)   
    else ()
        get_target_property(public_sdk_SOURCE_DIR sdk SOURCE_DIR)
    endif()
endmacro(smtg_enable_vst3_sdk)
