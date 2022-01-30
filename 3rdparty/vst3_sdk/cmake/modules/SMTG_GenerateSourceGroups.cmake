
#------------------------------------------------------------------------
# Generate Source Groups
#------------------------------------------------------------------------

#------------------------------------------------------------------------
function(smtg_generate_source_groups root)
    foreach(source ${ARGN})
        set(path ${source})
        string(REGEX REPLACE "${root}/" "" path "${path}")
        string(REGEX REPLACE "[\\\\/][^\\\\/]*$" "" path "${path}")
        string(REGEX REPLACE "^[\\\\/]" "" path "${path}")
        string(REGEX REPLACE "\\.\\.[\\\\/]" "" path "${path}")
        string(REGEX REPLACE "/" "\\\\\\\\" path "${path}")
        source_group(${path} FILES ${source})
    endforeach()
endfunction(smtg_generate_source_groups)

#------------------------------------------------------------------------
function(smtg_target_generate_source_groups target)
	get_target_property(_source_list ${target} SOURCES)

	foreach(_source IN ITEMS ${_source_list})
		get_filename_component(_source_path "${_source}" PATH)
		string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
		source_group("${_source_path_msvc}" FILES "${_source}")
	endforeach()
endfunction(smtg_target_generate_source_groups)
