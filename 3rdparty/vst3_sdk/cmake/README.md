# Welcome to VST 3 SDK cmake

This repository contains Steinberg's [cmake modules](https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html) and template files which can be used to create VST3 Plug-Ins for platforms like Windows, macOS, iOS and Linux.

For more information about cmake visit [cmake.org](https:://www.cmake.org/).

## Dependency map

In order to build a plug-in successfully, it only needs to link to ```sdk``` library. VSTGUI is optional and can be used by linking to ```vstgui_support```.

The following figure shows the dependency map by using AGain, EditorHost and VST3Inspector:

```c
again
+-- sdk
|   +-- sdk_common
|       +-- base
|       +-- pluginterfaces
|
+-- vstgui_support
    +-- base
    +-- vstgui_uidescription
        +-- vstgui
```
```editorhost
editorhost
+-- sdk_hosting
    +-- sdk_common
        +-- base
        +-- pluginterfaces
```

```VST3Inspector
VST3Inspector
+-- vstgui_uidescription 
|   +-- vstgui
+-- vstgui_standalone
    +-- vstgui
```

## Naming conventions and guidelines

Steinberg module, macro, function and variable names have the prefix ```SMTG_``` or ```smtg_``` in order to avoid name clashes. 

### File namings

Steinberg module names are written in camel case and have the prefix ```SMTG_```.

Examples:

```c
SMTG_AddVST3Library.cmake
SMTG_DetectPlatform.cmake
SMTG_ConfigureCmakeGenerator.cmake
```

### Macro and function namings

Steinberg macro and function names are written in lower case and have the prefix ```smtg_``` followed by a verb.

Examples:

```c
smtg_detect_platform
smtg_configure_cmake_generator
smtg_setup_symbol_visibility
smtg_detect_xcode_version
```

### Variable namings

Steinberg variable names are written in upper case and have the prefix ```SMTG_```.

Examples:

```c
SMTG_PLUGIN_BINARY_DIR
SMTG_PLUGIN_EXTENSION
SMTG_PLUGIN_PACKAGE_NAME
```

## License & Usage guidelines

More details can be found at [www.steinberg.net/sdklicenses_vst3](http://www.steinberg.net/sdklicenses_vst3)

----
Return to [VST 3 SDK](https://github.com/steinbergmedia/vst3sdk)