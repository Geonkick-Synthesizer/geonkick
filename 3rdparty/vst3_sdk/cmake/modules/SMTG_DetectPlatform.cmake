
# Detect the platform which cmake builds for.
#
# The detected platform var is stored within the internal cache of cmake in order to make it
# available globally.
set(SMTG_PLATFORM_DETECTION_COMMENT "The platform which was detected by SMTG")

#------------------------------------------------------------------------
# Detect the Platform
#
macro(smtg_detect_platform)
	if(APPLE)
		set(SMTG_MAC TRUE CACHE INTERNAL ${SMTG_PLATFORM_DETECTION_COMMENT})
	elseif(UNIX OR ANDROID_PLATFORM)
		set(SMTG_LINUX TRUE CACHE INTERNAL ${SMTG_PLATFORM_DETECTION_COMMENT})
	elseif(WIN32)
		set(SMTG_WIN TRUE CACHE INTERNAL ${SMTG_PLATFORM_DETECTION_COMMENT})
	endif(APPLE)
endmacro(smtg_detect_platform)

#------------------------------------------------------------------------
# Detect the default Xcode Version
#
macro(smtg_detect_xcode_version)
    if(SMTG_MAC)
        if(NOT DEFINED ENV{XCODE_VERSION})
            # find out Xcode version if not set by cmake
            execute_process(COMMAND xcodebuild -version OUTPUT_VARIABLE XCODE_VERSION ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
            string(REGEX MATCH "Xcode [0-9\\.]+" XCODE_VERSION "${XCODE_VERSION}")
            string(REGEX REPLACE "Xcode ([0-9\\.]+)" "\\1" XCODE_VERSION "${XCODE_VERSION}")
        endif()
        if(XCODE_VERSION VERSION_LESS "9")
            message(FATAL_ERROR "[SMTG] XCode 9 or newer is required")
        endif()
        message(STATUS "[SMTG] Building with Xcode version: ${XCODE_VERSION}")
        # macOS defaults
        if(NOT DEFINED ENV{MACOSX_DEPLOYMENT_TARGET})
            set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "macOS deployment target")
            message(STATUS "[SMTG] macOS Deployment Target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
        endif()
        if(NOT DEFINED ENV{SDKROOT})
            execute_process(COMMAND xcrun --sdk macosx --show-sdk-path OUTPUT_VARIABLE CMAKE_OSX_SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
        endif()
    endif(SMTG_MAC)
endmacro(smtg_detect_xcode_version)
