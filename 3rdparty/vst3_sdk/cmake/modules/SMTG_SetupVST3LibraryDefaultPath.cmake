
if(SMTG_WIN)
    option(SMTG_PLUGIN_TARGET_USER_PROGRAM_FILES_COMMON "use FOLDERID_UserProgramFilesCommon as VST3 target path" OFF)
endif(SMTG_WIN)

#------------------------------------------------------------------------
# VST3 Folder (following the specification)
function(smtg_get_default_vst3_path)
    if(SMTG_WIN)
        set(win64Found "1")
        if(NOT ${CMAKE_GENERATOR_PLATFORM} STREQUAL "")
            string(FIND ${CMAKE_GENERATOR_PLATFORM} x64 win64Found)
            if(${win64Found} EQUAL -1)
                string(FIND ${CMAKE_GENERATOR_PLATFORM} ARM64 win64Found)
            endif()
        elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017" OR 
               ${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015" OR 
               ${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013" OR 
               ${CMAKE_GENERATOR} STREQUAL "Visual Studio 11 2012" OR 
               ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10 2010" OR 
               ${CMAKE_GENERATOR} STREQUAL "Visual Studio 9 2008")
                    set(win64Found "-1")
        else()
            string(FIND ${CMAKE_GENERATOR} Win32 win32Found)
            if(NOT ${win32Found} EQUAL -1)
                set(win64Found "-1")
            endif()
        endif()
        if(${win64Found} EQUAL -1) # for 32bits OS
            if(EXISTS "$ENV{PROGRAMFILES} (x86)")
                set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{PROGRAMFILES} (x86)\\Common Files\\VST3" PARENT_SCOPE)
            else()
                set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{PROGRAMFILES}\\Common Files\\VST3" PARENT_SCOPE)
            endif()
        else()  # for 64bits OS
            #FOLDERID_UserProgramFilesCommon
            if(SMTG_PLUGIN_TARGET_USER_PROGRAM_FILES_COMMON)
                set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{LOCALAPPDATA}\\Programs\\Common\\VST3" PARENT_SCOPE)
            else()
                set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{CommonProgramW6432}\\VST3" PARENT_SCOPE)
            endif(SMTG_PLUGIN_TARGET_USER_PROGRAM_FILES_COMMON)
        endif()
    elseif(SMTG_MAC)
        set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{HOME}/Library/Audio/Plug-Ins/VST3" PARENT_SCOPE)
    elseif(SMTG_LINUX)
        set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{HOME}/.vst3" PARENT_SCOPE)
    else()
        message(FATAL_ERROR "[SMTG] unknown platform")
    endif(SMTG_WIN)
endfunction(smtg_get_default_vst3_path)
