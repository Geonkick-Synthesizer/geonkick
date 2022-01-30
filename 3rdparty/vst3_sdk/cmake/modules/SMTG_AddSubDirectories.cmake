
#------------------------------------------------------------------------
# add every sub directory of the current source dir if it contains a CMakeLists.txt
function(smtg_add_subdirectories)
    file(GLOB subDirectories RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
    foreach(dir ${subDirectories})
        if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt")
                add_subdirectory(${dir})
            endif()
        endif()
    endforeach(dir)
endfunction(smtg_add_subdirectories)
