
if(ANDROID)
    add_subdirectory(adelay)
    return()
endif(ANDROID)

include(SMTG_AddSubDirectories)

smtg_add_subdirectories()
