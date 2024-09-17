if (GKICK_PLUGIN_LV2)
  add_subdirectory(lv2)
endif(GKICK_PLUGIN_LV2)

if (GKICK_PLUGIN_VST)
  add_subdirectory(vst)
endif(GKICK_PLUGIN_VST)
