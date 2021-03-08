# Redkite

#### Description

Redkite is a small free software GUI toolkit.
It is developed in C++ and inspired from other
well known GUI toolkits.

Version: 1.3.1

License: GPLv3

#### The problem

An application that is developed in a particular GUI toolkit is loading
in its address space GUI plugins that are developed with different versions
of the same or different GUI toolkit. Using well known GUI toolkits for this
purpose can cause a lot of issues. For example, often these conflicts arise
in music software when a Digital Audio Workstation as a host is loading GUI
audio plugins. The Redkite GUI toolkit is developed to address
this issue, and provide a way to develop easily self-sufficient
GUI plugins that can be embedded the host GUI.

### Goals

 * Generally to have the same basic properties as common GUI toolkits.

 * Many instances of the same or different versions of the toolkit must be able
   to run without problems in the same process address space.

 * Can be easily linked statically. Shared libraries that are developed
   using the toolkit and loaded dynamically must not cause conflicts.

 * Not to add dependencies that breaks the above points. Also, maximum
   to relay on dependencies for runtime that are provided by default
   by the system. Avoid requiring to install additional dependencies
   for runtime that may cause version conflicts.

 * Interface to be simple

### Main features

* Object tree ownership
* Widgets
* Popup and dialog widget
* Graphics (with Cairo backend)
* Painter (paints RkImage and widgets)
* Events
* Actions (a binding mechanism between objects)
* GNU/Linux (XLib/X11)
* rkpng2c (a tool that converts PNG images to C array encoded in ARGB32)
* RkTimer (a timer class)
* Widget container used for vertical and horizontal layout
* Shortcuts
* Support to scale the UI (useful for HiDPI cases)

### Interface

* RkAction
* RkButton
* RkCanvas
* RkColor
* RkContainer
* RkContainerItem
* RkContainerWidgetItem
* RkEvent
* RkEventQueue
* RkFont
* Rk
* RkImage
* RkLabel
* RkLineEdit
* RkList
* RkLog
* RkMain
* RkModel
* RkObject
* RkPainter
* RkPen
* RkPlatform
* RkPoint
* RkProgressBar
* RkRealPoint
* RkRect
* RkSize
* RkTimer
* RkTransition
* RkVariant
* RkWidget
* RkString

### Technical limitations

* It is not cross-plafrom, supports only GNU/Linux (X11/Xlib)
* RkLineEdit is limited, doesn't implement all standard features
* Doesn't have many standard widgets:
     combo box, popup menus, file dialog,
     tree view, grid view, multi-line edit etc.
* Supports only Cairo graphics backend
* Doesn't support copy/paste
* Doesn't support unicode

### For what Redkite is recommended

* Standalone applications with relative simple UI
* GUI apps/plugins with relative simple UI that can be embedded into the host GUI

### Build & Install Redkite

In order to build Redkite there is a need to install the following development packages:

* Cairo

On Debian, Ubuntu, Ubuntu Studio install:

    apt-get install build-essential
    apt-get install cmake
    apt-get install libcairo2-dev

Clone the code repository, compile and install.

Note: it is recommended to use release versions (see repository tags)

        git clone https://gitlab.com/iurie-sw/redkite.git
        mkdir redkite/build
        cd redkite/build
        cmake ../
        make
        make install

### What applications were developed with Redkite?

* [Geonkick](https://gitlab.com/iurie-sw/geonkick) - a percussion synthesizer.
