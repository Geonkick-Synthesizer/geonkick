
if(SMTG_ADD_VSTGUI)
    set(hostchecker_sources
        resource/hostchecker.uidesc
        source/cids.h
        source/editorsizecontroller.cpp
        source/editorsizecontroller.h
        source/eventlistcheck.cpp
        source/eventlistcheck.h
        source/eventlogdatabrowsersource.cpp
        source/eventlogdatabrowsersource.h
        source/eventlogger.cpp
        source/eventlogger.h
        source/factory.cpp
        source/hostcheck.cpp
        source/hostcheck.h
        source/hostcheckercontroller.cpp
        source/hostcheckercontroller.h
        source/hostcheckerprocessor.cpp
        source/hostcheckerprocessor.h
        source/logevents.h
        source/parameterchangescheck.cpp
        source/parameterchangescheck.h
        source/processcontextcheck.cpp
        source/processcontextcheck.h
        source/processsetupcheck.cpp
        source/processsetupcheck.h
        source/version.h
    )

    set(target hostchecker)
    smtg_add_vst3plugin(${target} ${hostchecker_sources})
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
            resource/hostchecker.uidesc
            resource/bypass.png
            resource/knob.png
    )

    if(SMTG_MAC)
        smtg_target_set_bundle(${target}
        	BUNDLE_IDENTIFIER "com.steinberg.vst3.hostchecker"
        	INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/info.plist" PREPROCESS
        )
    elseif(SMTG_WIN)
        target_sources(${target}
            PRIVATE
                resource/hostchecker.rc
        )
    endif(SMTG_MAC)
endif(SMTG_ADD_VSTGUI)
