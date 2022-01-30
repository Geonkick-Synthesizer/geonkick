
set(again_sampleaccurate_sources
	source/agsa_controller.cpp
	source/agsa_factory.cpp
	source/agsa_processor.cpp
	source/agsa.h
	source/tutorial.cpp
	source/tutorial.h
	source/version.h
    ${SDK_ROOT}/public.sdk/source/vst/utility/test/sampleaccuratetest.cpp
    ${SDK_ROOT}/public.sdk/source/vst/utility/test/rttransfertest.cpp
)

set(target again_sampleaccurate)
smtg_add_vst3plugin(${target} ${again_sampleaccurate_sources})
set_target_properties(${target} 
	PROPERTIES
		${SDK_IDE_PLUGIN_EXAMPLES_FOLDER}
)
target_link_libraries(${target}
	PRIVATE
		sdk
		sdk_hosting
)

if(SMTG_MAC)
	smtg_target_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif(SMTG_WIN)
	target_sources(${target}
		PRIVATE
			resource/again_sampleaccurate.rc
	)
endif(SMTG_MAC)

