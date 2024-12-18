## Redkite

A lightweight graphics widget toolkit.

Author: Iurie Nistor

Version: 2.0.0

License: GNU General Public License v3.0

## Overview

Redkite is a small [free software](https://gnu.org/philosophy/free-sw.en.html) GUI toolkit. It offers graphics widget in a single system window. It is developed in C++ and inspired from other well known GUI toolkits. Currently supports GNU/Linux and Windows.

## Goals

* Small and lightweight
* Many instances of the same or different versions of the toolkit must be able to run without problems in the same process address space.
* Can be easily linked statically. Shared libraries that are developed using the toolkit and loaded dynamically must not cause conflicts due to the toolkit.   
* Not to add dependencies that breaks the above points. Maximum to relay on dependencies for runtime that are provided by default by the system.  
* A simple interface

## Use Cases

The toolkit can be used for embeddable UIs, such as audio plugins. It can also be used to develop standalone applications.

## Main features

* Object tree ownership
* Widgets (as graphics widgets)
* Popup and dialog widget (limited functionality)
* Graphics (with Cairo backend)
* Painter (paints RkImage and widgets)
* Events (windows system events, post events)
* Actions (a binding mechanism between objects)
* XLib and WinAPI
* rkpng2c (a tool that converts PNG images to C array encoded in ARGB32)
* RkTimer (a timer class)
* Widget container used for vertical and horizontal layout
* Shortcuts
* Support to scale the UI (useful for HiDPI cases)

## Interface

* RkAction
* RkButton
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
* RkSpinBox
* RkTimer
* RkTransition
* RkVariant
* RkWidget

## Used by

* Geonkick Synthesizer - https://geonkick.org
