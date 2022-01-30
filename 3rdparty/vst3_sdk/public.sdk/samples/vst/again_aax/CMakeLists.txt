
if(SMTG_AAX_SDK_PATH AND SMTG_ADD_VSTGUI)
    include(SMTG_AddAAXLibrary)

    set(again_sources
        source/againaax.cpp
        ../again/source/again.cpp
        ../again/source/again.h
        ../again/source/againcids.h
        ../again/source/againcontroller.cpp
        ../again/source/againcontroller.h
        ../again/source/againentry.cpp
        ../again/source/againparamids.h
        ../again/source/againprocess.h
        ../again/source/againsidechain.cpp
        ../again/source/againsidechain.h
        ../again/source/againuimessagecontroller.h
        ../again/source/version.h
    )

    set(target again_aax)
    smtg_add_aaxplugin(${target} ${again_sources})
    set_target_properties(${target}
        PROPERTIES
            ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER}
    )
    target_include_directories(${target}
        PUBLIC
            ${SDK_ROOT}/public.sdk/samples/vst/again/source
    )
    target_link_libraries(${target}
        PRIVATE
            sdk
            vstgui_support
            aaxwrapper
    )
    smtg_target_add_plugin_resources(${target}
        RESOURCES
            ../again/resource/again.uidesc
            ../again/resource/background.png
            ../again/resource/slider_background.png
            ../again/resource/slider_handle.png
            ../again/resource/slider_handle_2.0x.png
            ../again/resource/vu_on.png
            ../again/resource/vu_off.png
    )

    if(SMTG_MAC)
        smtg_target_set_bundle(${target}
        	BUNDLE_IDENTIFIER "com.steinberg.vst3.again-aax"
        	INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/../again/mac/Info.plist" PREPROCESS
        )
    elseif(SMTG_WIN)
        target_sources(${target} 
            PRIVATE 
                ../again/resource/again.rc
        )
        # remove warnings from VST2
        if(NOT MINGW)
            add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        endif(NOT MINGW)
    endif(SMTG_MAC)

endif(SMTG_AAX_SDK_PATH AND SMTG_ADD_VSTGUI)
