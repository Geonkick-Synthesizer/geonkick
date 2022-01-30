
if(SMTG_WIN)
    option(SMTG_USE_STATIC_CRT "use static CRuntime on Windows (option /MT)" OFF)
endif(SMTG_WIN)

#------------------------------------------------------------------------
# setup the platform toolset (compiler options)
macro(smtg_setup_platform_toolset)
    # deprecated
    if(SMTG_RENAME_ASSERT)
        add_compile_options(-DSMTG_RENAME_ASSERT=1)
    endif()

    # Add colors to clang output when using Ninja
    # See: https://github.com/ninja-build/ninja/wiki/FAQ
    if(UNIX AND CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_GENERATOR STREQUAL "Ninja")
        add_compile_options(-fcolor-diagnostics)
    endif()

    #------------
    option(SMTG_ENABLE_ADDRESS_SANITIZER "Enable Address Sanitizer" OFF)
       
    if(SMTG_LINUX)
        if(SMTG_ENABLE_ADDRESS_SANITIZER)
            set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES};ASan")
            add_compile_options($<$<CONFIG:ASan>:-DDEVELOPMENT=1>)
            add_compile_options($<$<CONFIG:ASan>:-fsanitize=address>)
            add_compile_options($<$<CONFIG:ASan>:-DVSTGUI_LIVE_EDITING=1>)
            add_compile_options($<$<CONFIG:ASan>:-g>)
            add_compile_options($<$<CONFIG:ASan>:-O0>)
            set(ASAN_LIBRARY asan)
            link_libraries($<$<CONFIG:ASan>:${ASAN_LIBRARY}>)
        endif()

        set(common_linker_flags "-Wl,--no-undefined")
        set(CMAKE_MODULE_LINKER_FLAGS "${common_linker_flags}" CACHE STRING "Module Library Linker Flags")
        set(CMAKE_SHARED_LINKER_FLAGS "${common_linker_flags}" CACHE STRING "Shared Library Linker Flags")
    endif(SMTG_LINUX)

    #------------
    if(UNIX)
        if(XCODE)
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
            add_compile_options(-Winconsistent-missing-override) # Suggest override when missing 
            add_compile_options(-Werror=return-type)
            add_compile_options(-ffast-math -ffp-contract=fast)
            if(SMTG_ENABLE_ADDRESS_SANITIZER)
                set(CMAKE_XCODE_SCHEME_ADDRESS_SANITIZER ON)
                set(CMAKE_XCODE_GENERATE_SCHEME ON)
            endif(SMTG_ENABLE_ADDRESS_SANITIZER)
        else()
            set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
            add_compile_options(-Wsuggest-override)     # Suggest override when missing 
            if(SMTG_MAC)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
                link_libraries(c++)
            else()
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")
                if(ANDROID)
                    set(CMAKE_ANDROID_STL_TYPE c++_static)
                    link_libraries(dl)
                 else()
                    link_libraries(stdc++fs pthread dl)
                endif(ANDROID)
            endif(SMTG_MAC)
        endif(XCODE)
    #------------
    elseif(SMTG_WIN)
        if(MINGW)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")
            set(common_linker_flags "-Wl,--no-undefined")
            set(CMAKE_MODULE_LINKER_FLAGS "${common_linker_flags}" CACHE STRING "Module Library Linker Flags")
            set(CMAKE_SHARED_LINKER_FLAGS "${common_linker_flags}" CACHE STRING "Shared Library Linker Flags")
        else()
            add_definitions(-D_UNICODE)
            add_compile_options(/fp:fast)                   # Floating Point Model
            add_compile_options($<$<CONFIG:Release>:/Oi>)   # Enable Intrinsic Functions (Yes)
            add_compile_options($<$<CONFIG:Release>:/Ot>)   # Favor Size Or Speed (Favor fast code)
            add_compile_options($<$<CONFIG:Release>:/GF>)   # Enable String Pooling
            add_compile_options($<$<CONFIG:Release>:/EHa>)  # Enable C++ Exceptions
            add_compile_options($<$<CONFIG:Release>:/Oy>)   # Omit Frame Pointers
            #add_compile_options($<$<CONFIG:Release>:/Ox>)  # Optimization (/O2: Maximise Speed /0x: Full Optimization)
            add_compile_options(/MP)                        # Multi-processor Compilation
            add_compile_options(/wd6031)                    # Return value ignored
            add_compile_options(/wd6308)                    # Realloc may return null pointer
            add_compile_options(/wd6330)                    # Incorrect type passed as parameter in call to function
            add_compile_options(/wd6385)                    # Invalid data
            add_compile_options(/wd6386)                    # Buffer overrun
            add_compile_options(/wd28125)                   # The function must be called from within a try/except block
            add_compile_options(/wd28251)                   # Inconsistent annotation for function
            #add_definitions("/analyze")                     # Enable Code Analyze

            set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
            set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
            if(DEFINED CMAKE_GENERATOR_PLATFORM AND CMAKE_GENERATOR_PLATFORM)
                string(TOLOWER ${CMAKE_GENERATOR_PLATFORM} GENERATOR_PLATFORM)
            endif()
            if(${GENERATOR_PLATFORM} MATCHES "arm*")
                add_compile_options($<$<CONFIG:Debug>:/Zi>)     # Program Database
                add_compile_options($<$<CONFIG:Debug>:/EHsc>)   # Enable C++ Exceptions
                add_compile_options(/wd4103)                    # Alignment changed after including header
            else()
                if(SMTG_ENABLE_ADDRESS_SANITIZER)
                    add_compile_options($<$<CONFIG:Debug>:/fsanitize=address>) # Enable Address Sanitizer
                    add_link_options($<$<CONFIG:Debug>:/INCREMENTAL:NO>) # Enable Incremental Linking
                else()
                    add_compile_options($<$<CONFIG:Debug>:/ZI>)  # Program Database for Edit And Continue
                endif()
            endif()
            if(SMTG_USE_STATIC_CRT)
                add_compile_options($<$<CONFIG:Debug>:/MTd>)    # Runtime Library: /MTd = MultiThreaded Debug Runtime
                add_compile_options($<$<CONFIG:Release>:/MT>)   # Runtime Library: /MT  = MultiThreaded Runtime
            else()
                add_compile_options($<$<CONFIG:Debug>:/MDd>)    # Runtime Library: /MDd = MultiThreadedDLL Debug Runtime
                add_compile_options($<$<CONFIG:Release>:/MD>)   # Runtime Library: /MD  = MultiThreadedDLL Runtime
            endif(SMTG_USE_STATIC_CRT)
        endif(MINGW)
    endif(UNIX)
endmacro(smtg_setup_platform_toolset)
