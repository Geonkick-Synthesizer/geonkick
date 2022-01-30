
#------------------------------------------------------------------------
macro(smtg_enable_vstgui_support)
    set(VSTGUI_DISABLE_UNITTESTS 1)
    set(VSTGUI_STANDALONE_EXAMPLES OFF)
    set(VSTGUI_STANDALONE ON)
    set(VSTGUI_TOOLS OFF)

    add_subdirectory(${SMTG_VSTGUI_ROOT}/vstgui4/vstgui ${CMAKE_CURRENT_BINARY_DIR}/VSTGUI.build)

#    list(APPEND CMAKE_MODULE_PATH "${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/standalone/cmake/modules")

    set(VST3_VSTGUI_SOURCES
        ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3groupcontroller.cpp
        ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3groupcontroller.h
        ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3padcontroller.cpp
        ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3padcontroller.h
        ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3editor.cpp
        ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3editor.h
        ${SDK_ROOT}/public.sdk/source/vst/vstguieditor.cpp
        )
    add_library(vstgui_support STATIC ${VST3_VSTGUI_SOURCES})
    target_compile_definitions(vstgui_support
        PUBLIC 
            $<$<CONFIG:Debug>:VSTGUI_LIVE_EDITING>
    )
    if(VSTGUI_ENABLE_DEPRECATED_METHODS)
        target_compile_definitions(vstgui_support 
            PUBLIC
                VSTGUI_ENABLE_DEPRECATED_METHODS=1
        )
    else()
        target_compile_definitions(vstgui_support
            PUBLIC
                VSTGUI_ENABLE_DEPRECATED_METHODS=0
        )
    endif(VSTGUI_ENABLE_DEPRECATED_METHODS)
    target_include_directories(vstgui_support
        PUBLIC
            ${SMTG_VSTGUI_ROOT}/vstgui4
    )
    target_link_libraries(vstgui_support 
        PRIVATE
            vstgui_uidescription
        PUBLIC
            base
    )
    smtg_target_setup_universal_binary(vstgui_support)
    smtg_target_setup_universal_binary(vstgui)
    smtg_target_setup_universal_binary(vstgui_uidescription)
    if(SMTG_MAC)
        if(XCODE)
            target_link_libraries(vstgui_support
                PRIVATE
                    "-framework Cocoa"
                    "-framework OpenGL"
                    "-framework Accelerate"
                    "-framework QuartzCore"
                    "-framework Carbon"
            )
        else()
            find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
            find_library(COCOA_FRAMEWORK Cocoa)
            find_library(OPENGL_FRAMEWORK OpenGL)
            find_library(ACCELERATE_FRAMEWORK Accelerate)
            find_library(QUARTZCORE_FRAMEWORK QuartzCore)
            find_library(CARBON_FRAMEWORK Carbon)
            target_link_libraries(vstgui_support
                PRIVATE
                    ${COREFOUNDATION_FRAMEWORK}
                    ${COCOA_FRAMEWORK}
                    ${OPENGL_FRAMEWORK}
                    ${ACCELERATE_FRAMEWORK}
                    ${QUARTZCORE_FRAMEWORK}
                    ${CARBON_FRAMEWORK}
            )
        endif(XCODE)
    endif(SMTG_MAC)
    if(SMTG_WIN AND SMTG_CREATE_BUNDLE_FOR_WINDOWS)
        target_compile_definitions(vstgui_support
            PUBLIC
                SMTG_MODULE_IS_BUNDLE=1
        )
        target_sources(vstgui_support
            PRIVATE
                ${SDK_ROOT}/public.sdk/source/vst/vstgui_win32_bundle_support.cpp
                ${SDK_ROOT}/public.sdk/source/vst/vstgui_win32_bundle_support.h
        )
    endif(SMTG_WIN AND SMTG_CREATE_BUNDLE_FOR_WINDOWS)
    message(STATUS "[SMTG] SMTG_VSTGUI_ROOT is set to: " ${SMTG_VSTGUI_ROOT})
    
    set_target_properties(vstgui
        PROPERTIES
            FOLDER "VSTGUI"
    )
    set_target_properties(vstgui_support
        PROPERTIES
            FOLDER "VSTGUI"
    )
    set_target_properties(vstgui_uidescription
        PROPERTIES
            FOLDER "VSTGUI"
    )
    if(VSTGUI_STANDALONE)
        set_target_properties(vstgui_standalone
            PROPERTIES
                FOLDER "VSTGUI"
        )
    endif(VSTGUI_STANDALONE)
endmacro(smtg_enable_vstgui_support)
