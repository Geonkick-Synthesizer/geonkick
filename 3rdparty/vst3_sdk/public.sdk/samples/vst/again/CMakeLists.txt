
if(SMTG_ADD_VSTGUI)
    if (SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION)
        set(again_vst2_sources
            ${SDK_ROOT}/public.sdk/source/vst/vst2wrapper/vst2wrapper.sdk.cpp
            source/againentry_vst2.cpp
        )
    endif(SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION)

    set(again_sources
        source/again.cpp
        source/again.h
        source/againcids.h
        source/againcontroller.cpp
        source/againcontroller.h
        source/againentry.cpp
        source/againparamids.h
        source/againprocess.h
        source/againsidechain.cpp
        source/againsidechain.h
        source/againuimessagecontroller.h
        source/version.h
        resource/again.uidesc
    )

    set(again_simple_sources
        ${SDK_ROOT}/public.sdk/source/vst/vstsinglecomponenteffect.cpp
        ${SDK_ROOT}/public.sdk/source/vst/vstsinglecomponenteffect.h
        source/againparamids.h
        source/againsimple.cpp
        source/againsimple.h
        source/version.h
    )

    # VST2 is only defined for macOS and Windows
    if(SMTG_MAC OR SMTG_WIN)
        if (SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION)
            set(again_sources ${again_sources} ${again_vst2_sources})
        else()
            set(again_sources ${again_sources})
        endif(SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION)
    endif(SMTG_MAC OR SMTG_WIN)

    set(target again)
    smtg_add_vst3plugin(${target} ${again_sources})
    #smtg_add_vst3plugin(${target} PACKAGE_NAME "A Gain" SOURCES_LIST ${again_sources})
    set_target_properties(${target} 
        PROPERTIES
            ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER}
    )
    target_link_libraries(${target}
        PRIVATE
            sdk
            vstgui_support
    )
    smtg_target_add_plugin_resources(${target}
        RESOURCES
            resource/again.uidesc
            resource/background.png
            resource/slider_background.png
            resource/slider_handle.png
            resource/slider_handle_2.0x.png
            resource/vu_on.png
            resource/vu_off.png
    )

    smtg_target_add_plugin_snapshots (${target}
        RESOURCES
            resource/84E8DE5F92554F5396FAE4133C935A18_snapshot.png
            resource/84E8DE5F92554F5396FAE4133C935A18_snapshot_2.0x.png
            resource/41347FD6FED64094AFBB12B7DBA1D441_snapshot.png
            resource/41347FD6FED64094AFBB12B7DBA1D441_snapshot_2.0x.png
    )

    if(SMTG_MAC)
        smtg_target_set_bundle(${target}
            INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/mac/Info.plist" PREPROCESS
        )
    elseif(SMTG_WIN)
        target_sources(${target}
            PRIVATE
                resource/again.rc
        )
    endif(SMTG_MAC)

    if(SMTG_MAC AND XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)
        set(target again_ios)
        smtg_add_ios_vst3plugin(${target} "${SMTG_CODE_SIGN_IDENTITY_IOS}" "${target}" "${again_sources}")
        set_target_properties(${target}
            PROPERTIES
                ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER}
        )
        target_include_directories(${target}
            PUBLIC
                ${SMTG_VSTGUI_ROOT}/vstgui4
        )
        target_link_libraries(${target} 
            PRIVATE
                base_ios
                sdk_ios
                "-framework UIKit"
                "-framework CoreGraphics" 
                "-framework QuartzCore" 
                "-framework CoreText" 
                "-framework Accelerate" 
                "-framework ImageIO" 
                "-framework MobileCoreServices" 
        )

        smtg_target_add_plugin_resources(${target}
            RESOURCES
                resource/again.uidesc
                resource/background.png
                resource/slider_background.png
                resource/slider_handle.png
                resource/slider_handle_2.0x.png
                resource/vu_on.png
                resource/vu_off.png
        )

        smtg_target_set_bundle(${target}
	    	BUNDLE_IDENTIFIER "com.steinberg.vst3.again"
        	INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/mac/Info.plist" PREPROCESS
        )
        target_sources(${target} 
            PRIVATE 
                ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/vstgui_uidescription.cpp
                ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/vstgui_ios.mm
                ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3editor.cpp
                ${SDK_ROOT}/public.sdk/source/vst/vstguieditor.cpp
        )
    endif(SMTG_MAC AND XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)

    set(targetsimple againsimple)
    smtg_add_vst3plugin(${targetsimple} ${again_simple_sources})
    set_target_properties(${targetsimple}
        PROPERTIES
            ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
    target_link_libraries(${targetsimple}
        PUBLIC
            sdk
            vstgui_support
    )

    smtg_target_add_plugin_resources(${targetsimple}
        RESOURCES
            resource/again.uidesc
            resource/background.png
            resource/slider_background.png
            resource/slider_handle.png
            resource/slider_handle_2.0x.png
            resource/vu_on.png
            resource/vu_off.png
    )

    if(SMTG_MAC)
        smtg_target_set_bundle(${targetsimple}
	        BUNDLE_IDENTIFIER "com.steinberg.vst3.again-simple"
        	INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/mac/Info.plist" PREPROCESS
        )
    elseif(SMTG_WIN)
        target_sources(${targetsimple}
            PRIVATE
                resource/again.rc
        )
    endif(SMTG_MAC)

    # do not build AGain VST 2 by default
    option(SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION "Legacy: Create the VST2 version of the Sample Plug-in AGain, be sure that you have copied the VST2 interfaces into the folder VST_SDK/vst3sdk/pluginterfaces/vst2.x" OFF)
    if(SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION)
        message(STATUS "[SMTG] SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION is set. A VST 2 version of the plug-in will be created (just rename the generated file from .vst3 to .dll (win) or .vst (mac)).")
        if(XCODE)
            # fix missing VSTPluginMain symbol when also building VST 2 version
            set_target_properties(${target}
                PROPERTIES
                    XCODE_ATTRIBUTE_EXPORTED_SYMBOLS_FILE ""
            )
        endif(XCODE)
        if(SMTG_WIN)
            if(NOT MINGW)
                add_definitions(-D_CRT_SECURE_NO_WARNINGS)
            endif(NOT MINGW)
        endif(SMTG_WIN)
    endif(SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION)
endif(SMTG_ADD_VSTGUI)
