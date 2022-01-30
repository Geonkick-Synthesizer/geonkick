
# use by default SMTG_ as prefix for ASSERT,...
option(SMTG_RENAME_ASSERT "Rename ASSERT to SMTG_ASSERT" ON)

# Logging
option(SMTG_ENABLE_TARGET_VARS_LOG "Enable Target variables Logging" OFF)

# Create Bundle on Windows for the Plug-ins
if(SMTG_WIN)
    option(SMTG_CREATE_BUNDLE_FOR_WINDOWS "Create Bundle on Windows for the Plug-ins (New since VST 3.6.10!)" ON)
endif(SMTG_WIN)

# Create Symbolic Link for the Plug-ins
if(SMTG_WIN)
    set(DEF_OPT_LINK ON) # be sure to start visual with admin right when enabling this
else()
    set(DEF_OPT_LINK ON)
endif(SMTG_WIN)
option(SMTG_CREATE_PLUGIN_LINK "Create symbolic link for each Plug-in (you need to have the Administrator right on Windows! or change the Local Group Policy to allow create symbolic links)" ${DEF_OPT_LINK})
