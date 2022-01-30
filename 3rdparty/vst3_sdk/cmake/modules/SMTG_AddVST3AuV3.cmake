
#------------------------------------------------------------------------
# Includes
#------------------------------------------------------------------------
include(SMTG_CodeSign)

if(SMTG_MAC AND SMTG_ADD_VSTGUI)
    if(XCODE)
        set(SMTG_AUV3_FOLDER FOLDER "AudioUnit V3")

        set(auv3wrappermacos_sources
            ${SDK_ROOT}/public.sdk/source/vst/auv3wrapper/AUv3WrappermacOS/main.mm
        )

        set(auv3wrappermacosextension_sources
            ${SDK_ROOT}/public.sdk/source/vst/auv3wrapper/Shared/AUv3WrapperFactory.mm
        )

        set(auv3wrapperios_sources
        )

        set(auv3wrapperiosextension_sources
            ${SDK_ROOT}/public.sdk/source/vst/auv3wrapper/Shared/AUv3WrapperFactory.mm
        )

        #------------------------------------------------------------------------
        # Add a AudioUnit Version 3 App target for macOS
        # @param target                 target name
        # @param BuildTarget            must be either "macOS" or "iOS"
        # @param OutputName             Output App name
        # @param BundleID               Bundle Identifier
        # @param AudioUnitConfigHeader  configuration header
        # @param EntitlementFile        Code sign entitlements file
        # @param AppSources             Application Sources
        # @param AppUIResources         Application UI Resources
        # @param AppInfoPlist           Info.plist for the application
        # @param ExtensionInfoPlist     Info.plist for the app-extension
        # @param vst3_plugin_target     the vst3 plugin target
        #------------------------------------------------------------------------
        function(smtg_add_auv3_app target BuildTarget OutputName BundleID AudioUnitConfigHeader EntitlementFile AppSources AppUIResources AppInfoPlist ExtensionInfoPlist vst3_plugin_target)
            if(BuildTarget STREQUAL macOS)
                set(macOS TRUE)
            elseif(BuildTarget STREQUAL iOS)
                set(iOS TRUE)
            else()
                message(FATAL_ERROR "[SMTG] BuildTarget must be macOS or iOS not ${BuildTarget}")
            endif()
            
            if(macOS)
                set(link_frameworks
                    "-framework Accelerate" 
                    "-framework AppKit" 
                    "-framework AudioToolbox" 
                    "-framework AVFoundation" 
                    "-framework CoreAudioKit" 
                    "-framework OpenGL" 
                    "-framework QuartzCore"
                    "-framework CoreMIDI"
                )
                set(app-extension-target ${target}_appextension_macos)
                set(app-target ${target})
                set(app-extension-sources ${auv3wrappermacosextension_sources})
                set(auwrapper-sources ${auv3wrappermacos_sources} ${AppSources})
                set(auwrapper-lib auv3wrappermacos)
            else()
                set(link_frameworks
                    "-framework UIKit" 
                    "-framework CoreAudioKit" 
                    "-framework AudioToolbox" 
                    "-framework CoreGraphics" 
                    "-framework QuartzCore" 
                    "-framework CoreText" 
                    "-framework Accelerate" 
                    "-framework AVFoundation" 
                    "-framework ImageIO" 
                    "-framework MobileCoreServices" 
                    "-framework MediaPlayer"
                    "-framework CoreMIDI"
                )
                set(app-extension-target ${target}_appextension_ios)
                set(app-target ${target})
                set(app-extension-sources ${auv3wrapperiosextension_sources})
                set(auwrapper-sources ${AppSources})
                set(auwrapper-lib auv3wrapperios)
            endif(macOS)

            set(auwrapper-xib-resources ${AppUIResources})

            get_filename_component(AudioUnitConfig ${AudioUnitConfigHeader} ABSOLUTE)
            get_filename_component(AppInfoPlist ${AppInfoPlist} ABSOLUTE)
            get_filename_component(ExtensionInfoPlist ${ExtensionInfoPlist} ABSOLUTE)
            get_filename_component(EntitlementFile ${EntitlementFile} ABSOLUTE)

            # app-extension
            add_executable(${app-extension-target} ${app-extension-sources})
            target_link_libraries(${app-extension-target}
                PRIVATE
                    ${auwrapper-lib} 
                    ${link_frameworks}
            )
            add_dependencies(${app-extension-target} "${vst3_plugin_target}")
            set_target_properties(${app-extension-target}
                PROPERTIES
                    # BUNDLE YES
                    # MACOSX_BUNDLE TRUE
                    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${BundleID}.appex
                    XCODE_ATTRIBUTE_INFOPLIST_PREFIX_HEADER ${AudioUnitConfig}
                    XCODE_ATTRIBUTE_GCC_PREFIX_HEADER ${AudioUnitConfig}
                    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
                    XCODE_ATTRIBUTE_WRAPPER_EXTENSION appex
                    XCODE_PRODUCT_TYPE com.apple.product-type.app-extension
                    ${SMTG_AUV3_FOLDER}
            )
            smtg_target_set_bundle(${app-extension-target} INFOPLIST "${ExtensionInfoPlist}" PREPROCESS)
            
            # application
            add_executable(${app-target} ${auwrapper-sources} ${auwrapper-xib-resources})
            target_link_libraries(${app-target}
                PRIVATE
                    ${auwrapper-lib}
                    ${link_frameworks}
            )
            add_dependencies(${app-target} "${app-extension-target}")
            set_target_properties(${app-target}
                PROPERTIES
                    RESOURCE "${auwrapper-xib-resources}"
                    MACOSX_BUNDLE TRUE
                    OUTPUT_NAME "${OutputName}"
                    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${BundleID}
                    XCODE_ATTRIBUTE_INFOPLIST_PREFIX_HEADER ${AudioUnitConfig}
                    XCODE_ATTRIBUTE_GCC_PREFIX_HEADER ${AudioUnitConfig}
                    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
                    ${SMTG_AUV3_FOLDER}
            )
            smtg_target_set_bundle(${app-target} INFOPLIST "${AppInfoPlist}" PREPROCESS)

            get_target_property(PLUGIN_PACKAGE_PATH ${vst3_plugin_target} SMTG_PLUGIN_PACKAGE_PATH)

            if(macOS)
                add_custom_command(
                    TARGET ${app-extension-target} POST_BUILD
                    COMMENT "[SMTG] Copy VST3 plugin into App-Extension"
                    COMMAND rm -Rf
                        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/$<TARGET_FILE_NAME:${app-extension-target}>.appex/Contents/PlugIns/plugin.vst3"
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/$<TARGET_FILE_NAME:${app-extension-target}>.appex/Contents/PlugIns/plugin.vst3"
                    COMMAND /bin/cp -a -v
                        "${PLUGIN_PACKAGE_PATH}/"
                        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/$<TARGET_FILE_NAME:${app-extension-target}>.appex/Contents/PlugIns/plugin.vst3/"
                )
                add_custom_command(
                    TARGET ${app-target} POST_BUILD
                    COMMENT "[SMTG] Copy App-Extension into Application"
                    COMMAND rm -Rf
                        "$<TARGET_BUNDLE_CONTENT_DIR:${app-target}>/PlugIns/auv3.appex"
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        "$<TARGET_BUNDLE_CONTENT_DIR:${app-target}>/PlugIns/auv3.appex"
                    COMMAND /bin/cp -a -v
                        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/$<TARGET_FILE_NAME:${app-extension-target}>.appex/"
                        "$<TARGET_BUNDLE_CONTENT_DIR:${app-target}>/PlugIns/auv3.appex/"
                )
                smtg_target_codesign(${app-target} ${SMTG_IOS_DEVELOPMENT_TEAM} "${SMTG_CODE_SIGN_IDENTITY_MAC}")
                smtg_target_codesign(${app-extension-target} ${SMTG_IOS_DEVELOPMENT_TEAM} "${SMTG_CODE_SIGN_IDENTITY_MAC}")
                set_target_properties(${app-extension-target}
                    PROPERTIES
                        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${EntitlementFile}"
                )
            else()
                add_custom_command(
                    TARGET ${app-extension-target} POST_BUILD
                    COMMENT "[SMTG] Copy VST3 plugin into App-Extension"
                    COMMAND rm -Rf
                        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/$<TARGET_FILE_NAME:${app-extension-target}>.appex/PlugIns/plugin.vst3"
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/$<TARGET_FILE_NAME:${app-extension-target}>.appex/PlugIns/plugin.vst3"
                    COMMAND /bin/cp -a -v
                        "${PLUGIN_PACKAGE_PATH}/"
                        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/$<TARGET_FILE_NAME:${app-extension-target}>.appex/PlugIns/plugin.vst3/"
                )
                add_custom_command(
                    TARGET ${app-target} POST_BUILD
                    COMMENT "[SMTG] Copy App-Extension into Application"
                    COMMAND rm -Rf
                        "$<TARGET_BUNDLE_DIR:${app-target}>/PlugIns/auv3.appex"
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        "$<TARGET_BUNDLE_DIR:${app-target}>/PlugIns/auv3.appex"
                    COMMAND /bin/cp -a -v
                        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/$<TARGET_FILE_NAME:${app-extension-target}>.appex/"
                        "$<TARGET_BUNDLE_DIR:${app-target}>/PlugIns/auv3.appex/"
                )

                smtg_target_set_platform_ios(${app-target})
                smtg_target_set_platform_ios(${app-extension-target})
                smtg_target_codesign(${app-target} ${SMTG_IOS_DEVELOPMENT_TEAM} "${SMTG_IOS_DEVELOPMENT_TEAM}")
                smtg_target_codesign(${app-extension-target} ${SMTG_IOS_DEVELOPMENT_TEAM} "${SMTG_IOS_DEVELOPMENT_TEAM}")

                set_target_properties(${app-target} 
                    PROPERTIES 
                        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
                )
                set_target_properties(${app-extension-target}
                    PROPERTIES 
                        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${EntitlementFile}"
                        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
                )
            endif(macOS)

            set(AUV3_APP_TARGET ${app-target} PARENT_SCOPE)
            set(AUV3_EXTENSION_TARGET ${app-extension-target} PARENT_SCOPE)

        endfunction(smtg_add_auv3_app)      

        # Deprecated since 3.7.4 -----------------------------
        function(smtg_add_auv3 target BuildTarget OutputName BundleID AudioUnitConfigHeader EntitlementFile AppSources AppUIResources AppInfoPlist ExtensionInfoPlist vst3_plugin_target)
            message(DEPRECATION "[SMTG] Use smtg_add_auv3_app instead of smtg_add_auv3")
            smtg_add_auv3_app (${target} ${BuildTarget} ${OutputName} ${BundleID} ${AudioUnitConfigHeader} ${EntitlementFile} ${AppSources} ${AppUIResources} ${AppInfoPlist} ${ExtensionInfoPlist} ${vst3_plugin_target})
        endfunction(smtg_add_auv3)

    endif(XCODE)
endif(SMTG_MAC AND SMTG_ADD_VSTGUI)
