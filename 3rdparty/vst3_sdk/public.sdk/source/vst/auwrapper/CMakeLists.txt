if(SMTG_MAC)
    if (XCODE AND SMTG_COREAUDIO_SDK_PATH)
        set(target auwrapper)
        set(${target}_sources
            aucarbonview.mm
            aucarbonview.h
            aucocoaview.mm
            aucocoaview.h
            ausdk.mm
            auwrapper.mm
            auwrapper.h
            NSDataIBStream.mm
            NSDataIBStream.h
        )
        add_library(${target}
            STATIC
                ${${target}_sources}
        )
        smtg_target_setup_universal_binary(${target})
        set_target_properties(${target}
            PROPERTIES
                ${SDK_IDE_LIBS_FOLDER}
        )

        target_link_libraries(${target} 
            PRIVATE 
                sdk_hosting 
                "-framework AudioUnit" "-framework CoreMIDI"
                "-framework AudioToolbox"
                "-framework CoreFoundation"
                "-framework Carbon" 
                "-framework Cocoa" 
                "-framework CoreAudio"
        )
        target_include_directories(${target}
            PRIVATE 
                "${SMTG_COREAUDIO_SDK_PATH}/**"
        )
        add_custom_command(TARGET ${target}
            PRE_BUILD COMMAND 
                /usr/bin/ruby "./generateCocoaClassNamePrefix.rb" "${CMAKE_CURRENT_LIST_DIR}" WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
    else()
        message("[SMTG] * To enable building the AudioUnit wrapper, you need to use the Xcode generator and set SMTG_COREAUDIO_SDK_PATH to the path of your installation of the CoreAudio SDK!")
    endif(XCODE AND SMTG_COREAUDIO_SDK_PATH)
endif(SMTG_MAC)