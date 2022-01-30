
#------------------------------------------------------------------------
# Create a given Directory with Admin permission on Windows
function(smtg_create_directory_as_admin_win directory_name)
    if(NOT SMTG_WIN)
        message(FATAL_ERROR "[SMTG] smtg_create_directory_as_admin only works on Windows, use it in an if(SMTG_WIN) block")
    endif()
    set(TMPDIR "$ENV{TEMP}") 
    if(NOT EXISTS ${TMPDIR})
        set(TMPDIR "$ENV{TMPDIR}") 
    endif()
    message(STATUS "[SMTG] TMPDIR is set to: " ${TMPDIR})
    if(NOT EXISTS ${TMPDIR})
        message(FATAL_ERROR "[SMTG] smtg_create_directory_as_admin does find TEMP Folder!")
    endif()

    # create the bat creating the Directory
    set(TMPFILE ${TMPDIR}\\smtg_mkdir_windows_as_admin.bat)
    file(WRITE ${TMPFILE} "mkdir \"${directory_name}\"")

    execute_process(
        COMMAND
		   msg * /time:60 /w VST 3 SDK: In order to create a subDirectory you will need to provide Administrator permission!
    )
	# execute with powershell the bat file
    execute_process(
        COMMAND
            powershell.exe Start-Process ${TMPFILE} -Verb runAs
    )
endfunction(smtg_create_directory_as_admin_win)
