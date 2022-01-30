
if(SMTG_ADD_VSTGUI)
    set(noteexpressionsynth_sources
        source/brownnoise.h
        source/factory.cpp
        source/filter.h
        source/note_expression_synth_controller.cpp
        source/note_expression_synth_controller.h
        source/note_expression_synth_processor.cpp
        source/note_expression_synth_processor.h
        source/note_expression_synth_ui.cpp
        source/note_expression_synth_ui.h
        source/note_expression_synth_voice.cpp
        source/note_expression_synth_voice.h
        source/note_touch_controller.cpp
        source/note_touch_controller.h
        source/version.h
        ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/contrib/keyboardview.cpp
        ${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/contrib/keyboardview.h
        resource/note_expression_synth.uidesc
    )

    set(target noteexpressionsynth)

    smtg_add_vst3plugin(${target} ${noteexpressionsynth_sources})
    set_target_properties(${target}
        PROPERTIES
            ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER}
    )
    target_compile_features(${target}
        PUBLIC
            cxx_std_14
    )
    target_link_libraries(${target}
        PRIVATE
            sdk
            vstgui_support
    )
    smtg_target_add_plugin_resources(${target}
        RESOURCES
            resource/note_expression_synth.uidesc
            resource/about.png
            resource/background.png
            resource/groupframe.png
            resource/knob.png
            resource/knob2.png
            "resource/knob big.png"
            resource/vst3_logo_small.png
    )

    smtg_target_add_plugin_snapshots(${target}
        RESOURCES
            resource/41466D9BB0654576B641098F686371B3_snapshot.png
            resource/41466D9BB0654576B641098F686371B3_snapshot_2.0x.png
    )

    if(SMTG_MAC)
        smtg_target_set_bundle(${target}
        	BUNDLE_IDENTIFIER "com.steinberg.vst3.note-expression-synth"
        	INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist"
        	PREPROCESS
        )
    elseif(SMTG_WIN)
        target_sources(${target}
            PRIVATE
                resource/note_expression_synth.rc
        )
    endif(SMTG_MAC)

    if(SMTG_MAC AND XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)
        set(target noteexpressionsynth_ios)
        smtg_add_ios_vst3plugin(${target} "${SMTG_CODE_SIGN_IDENTITY_IOS}" "${target}" "${noteexpressionsynth_sources}")

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
                resource/note_expression_synth.uidesc
                resource/about.png
                resource/background.png
                resource/groupframe.png
                resource/knob.png
                resource/knob2.png
                resource/knob big.png
                resource/vst3_logo_small.png
        )
        smtg_target_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
        target_sources(${target}
            PRIVATE 
                "${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/vstgui_uidescription.cpp"
                "${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/vstgui_ios.mm"
                "${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3editor.cpp"
                "${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3groupcontroller.cpp"
                "${SMTG_VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3padcontroller.cpp"
                "${SDK_ROOT}/public.sdk/source/vst/vstguieditor.cpp"
        )
    endif(SMTG_MAC AND XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)

endif(SMTG_ADD_VSTGUI)
