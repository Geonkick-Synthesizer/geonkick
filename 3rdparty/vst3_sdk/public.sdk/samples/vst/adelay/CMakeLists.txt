
set(adelay_sources
    source/adelaycontroller.cpp
    source/adelaycontroller.h
    source/adelayids.h
    source/adelayprocessor.cpp
    source/adelayprocessor.h
    source/exampletest.cpp
    source/factory.cpp
    source/version.h
    ${SDK_ROOT}/public.sdk/source/vst/utility/test/ringbuffertest.cpp
    ${SDK_ROOT}/public.sdk/source/vst/utility/test/versionparsertest.cpp
)

set(target adelay)
smtg_add_vst3plugin(${target} ${adelay_sources})
set_target_properties(${target}
    PROPERTIES 
        ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER}
)
target_compile_features(${target}
    PUBLIC
        cxx_std_17
)
target_link_libraries(${target}
    PRIVATE
        sdk
)
if(SMTG_MAC)
    smtg_target_set_bundle(${target}
    	BUNDLE_IDENTIFIER "com.steinberg.vst3.adelay"
    	INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS
    )
elseif(SMTG_WIN)
    target_sources(${target}
        PRIVATE 
            resource/adelay.rc
    )
endif(SMTG_MAC)
