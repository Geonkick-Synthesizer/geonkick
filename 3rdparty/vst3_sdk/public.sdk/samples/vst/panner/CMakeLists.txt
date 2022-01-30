
if(SMTG_ADD_VSTGUI)
    set(plug_sources
        include/plugcontroller.h
        include/plugids.h
        include/plugprocessor.h
        include/version.h
        source/plugfactory.cpp
        source/plugcontroller.cpp
        source/plugprocessor.cpp
        resource/plug.uidesc
    )

    set(target panner)

    smtg_add_vst3plugin(${target} ${plug_sources})
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
            resource/plug.uidesc
            resource/background.png
            resource/animation_knob.png
            resource/background_2x.png
            resource/background_3x.png
            resource/animation_knob_3x.png
    )

    smtg_target_add_plugin_snapshots(${target}
        RESOURCES
            resource/A2EAF7DB320640F48EDE380DDF89562C_snapshot.png
            resource/A2EAF7DB320640F48EDE380DDF89562C_snapshot_2.0x.png
    )

    if(SMTG_MAC)
        smtg_target_set_bundle(${target}
        	BUNDLE_IDENTIFIER "com.steinberg.vst3.panner"
        	INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS
        )
    elseif(SMTG_WIN)
        target_sources(${target}
            PRIVATE
                resource/plug.rc
        )
    endif(SMTG_MAC)

endif(SMTG_ADD_VSTGUI)
