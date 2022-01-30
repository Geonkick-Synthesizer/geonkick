
if(SMTG_ADD_VSTGUI)
    set(noteexpressiontext_sources
        source/plug.cpp
        source/plug.h
        source/plugcids.h
        source/plugcontroller.cpp
        source/plugcontroller.h
        source/plugentry.cpp
        source/plugparamids.h
        source/version.h
        resource/plug.uidesc
    )

    set(target noteexpressiontext)
    smtg_add_vst3plugin(${target} ${noteexpressiontext_sources})
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
    )

    if(SMTG_MAC)
        smtg_target_set_bundle(${target}
        	BUNDLE_IDENTIFIER "com.steinberg.vst3.note-expression-text"
        	INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS
        )
    elseif(SMTG_WIN)
        target_sources(${target}
            PRIVATE 
                resource/plug.rc
        )
    endif(SMTG_MAC)
endif(SMTG_ADD_VSTGUI)
