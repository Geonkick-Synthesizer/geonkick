
if(SMTG_ADD_VSTGUI)
    set(pitchnames_sources
        source/factory.cpp
        source/pitchnames.cpp
        source/pitchnames.h
        source/pitchnamesdatabrowsersource.cpp
        source/pitchnamesdatabrowsersource.h
        source/version.h
    )

    set(target pitchnames)
    smtg_add_vst3plugin(${target} ${pitchnames_sources})
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
            resource/pitchnames.uidesc
    )

    if(SMTG_MAC)
        smtg_target_set_bundle(${target}
        	BUNDLE_IDENTIFIER "com.steinberg.vst3.pitchnames"
        	INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS
        )
    elseif(SMTG_WIN)
        target_sources(${target}
            PRIVATE
                resource/pitchnames.rc
        )
    endif(SMTG_MAC)

endif(SMTG_ADD_VSTGUI)
