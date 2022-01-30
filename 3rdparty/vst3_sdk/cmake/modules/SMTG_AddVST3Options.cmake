
#-------------------------------------------------------------------------------
# Includes
#-------------------------------------------------------------------------------
include(SMTG_SetupVST3LibraryDefaultPath)
include(SMTG_AddCommonOptions)
include(SMTG_Platform_Windows)

if(SMTG_MAC)
    set(SMTG_CODE_SIGN_IDENTITY_MAC "Mac Developer" CACHE STRING "macOS Code Sign Identity")
    set(SMTG_CODE_SIGN_IDENTITY_IOS "iPhone Developer" CACHE STRING "iOS Code Sign Identity")
endif(SMTG_MAC)
