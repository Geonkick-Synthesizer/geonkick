
#------------------------------------------------------------------------
# Setup symbol visibility
#
# Puts symbol visibility to default hidden. 
macro(smtg_setup_symbol_visibility)
    set(CMAKE_C_VISIBILITY_PRESET hidden)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endmacro(smtg_setup_symbol_visibility)

#------------------------------------------------------------------------
# Set exported symbols
#
# Specifies the exported symbols by file.
function(smtg_target_set_exported_symbols target exported_symbols_file)
    if(MSVC)
        set_target_properties(${target}
            PROPERTIES
                LINK_FLAGS "/DEF:\"${exported_symbols_file}\""
        )
    elseif(XCODE)
        set_target_properties(${target}
            PROPERTIES
                XCODE_ATTRIBUTE_EXPORTED_SYMBOLS_FILE "${exported_symbols_file}"
        )
    elseif(NOT MINGW)
        set_target_properties(${target}
            PROPERTIES
                LINK_FLAGS "-exported_symbols_list \"${exported_symbols_file}\""
        )
    endif(MSVC)
endfunction(smtg_target_set_exported_symbols)

#------------------------------------------------------------------------
# Deprecated since 3.7.4 -----------------------------
function(smtg_set_exported_symbols target exported_symbols_file)
    message(DEPRECATION "[SMTG] Use smtg_target_set_exported_symbols instead of smtg_set_exported_symbols")
    smtg_target_set_exported_symbols (${target} ${exported_symbols_file})
endfunction(smtg_set_exported_symbols)
