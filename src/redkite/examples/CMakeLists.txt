set(RK_EXAMPLE_HELLO ON)
set(RK_EXAMPLE_CHILD ON)
set(RK_EXAMPLE_PAINTER ON)
set(RK_EXAMPLE_ACTION ON)
set(RK_EXAMPLE_TIMER ON)

#set(RK_EXAMPLES_SOURCES_PROPERTY ${RK_EXAMPLES_PATH}/property.cpp)
#set(RK_EXAMPLES_SOURCES_WIDGET_COLORS ${RK_EXAMPLES_PATH}/widget_colors.cpp)
#set(RK_EXAMPLES_SOURCES_DIALOG ${RK_EXAMPLES_PATH}/dialog.cpp)
#set(RK_EXAMPLES_SOURCES_KEYEVENT ${RK_EXAMPLES_PATH}/keyevent.cpp)
#set(RK_EXAMPLES_SOURCES_LINEEDIT ${RK_EXAMPLES_PATH}/lineedit.cpp)

# ------------ Hello example -------

if (RK_EXAMPLE_HELLO)
  set(RK_EXAMPLES_SOURCES ${RK_EXAMPLES_PATH}/hello.cpp)
  add_executable(hello ${RK_EXEC_OPTION}
    ${RK_HEADERS}
    ${RK_EXAMPLES_SOURCES})
  add_dependencies(hello redkite)
  target_link_libraries(hello redkite)
  if (CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(hello "-mwindows -ld3d11 -ld2d1 -ldwrite -lm")
    target_link_libraries(hello "-mwindows -lstdc++ -lpthread -lm")
  else()
    target_link_libraries(hello "-lX11 -lpthread -lcairo -lrt -lm -ldl")
  endif()
endif (RK_EXAMPLE_HELLO)

# ------------ Child Example -------

if (RK_EXAMPLE_CHILD)
  set(RK_EXAMPLES_SOURCES_CHILD ${RK_EXAMPLES_PATH}/child.cpp)
  add_executable(child ${RK_EXEC_OPTION}
    ${RK_HEADERS}
    ${RK_EXAMPLES_SOURCES_CHILD})
  add_dependencies(child redkite)
  target_link_libraries(child redkite)
  if (CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(child "-mwindows -s -static -ld3d11 -ld2d1 -ldwrite -lm")
  else()
    target_link_libraries(child "-lX11 -lcairo -lpthread -lrt -lm -ldl")
  endif()
endif(RK_EXAMPLE_CHILD)

# ------------ Property -------

#add_executable(property
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_PROPERTY})

#add_dependencies(property redkite)
#target_link_libraries(property redkite)
#target_link_libraries(property "-lX11 -lrt -lm -ldl")
#target_link_libraries(property ${RK_GRAPHICS_BACKEND_LINK_LIBS})

# ------------ Label Example -------

if (RK_EXAMPLE_LABEL)
  set(RK_EXAMPLES_SOURCES_LABEL ${RK_EXAMPLES_PATH}/label.cpp)
  add_executable(label
    ${RK_HEADERS}
    ${RK_EXAMPLES_SOURCES_LABEL})
  add_dependencies(label redkite)
  target_link_libraries(label redkite)
  if (CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(label "-mwindows -lstdc++ -lm -lcairo")
  else ()
    target_link_libraries(label "-lX11 -lrt -lm -ldl")
    target_link_libraries(label ${RK_GRAPHICS_BACKEND_LINK_LIBS})
  endif()
endif(RK_EXAMPLE_LABEL)

#------------- Widget Colors --------

#add_executable(widget_colors
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_WIDGET_COLORS})

#add_dependencies(widget_colors redkite)
#target_link_libraries(widget_colors redkite)
#target_link_libraries(widget_colors "-lX11 -lrt -lm -ldl")
#target_link_libraries(widget_colors ${RK_GRAPHICS_BACKEND_LINK_LIBS})

# ------------ Painter Exmaple -------
if (RK_EXAMPLE_PAINTER)
    add_executable(painter
    ${RK_HEADERS}
    ${RK_EXAMPLES_PATH}/painter.cpp)

    add_dependencies(painter redkite)
    target_link_libraries(painter redkite)
    if (CMAKE_SYSTEM_NAME MATCHES Windows)
        target_link_libraries(painter "-mwindows -lstdc++ -lm -lcairo")
    else ()
       target_link_libraries(painter "-lX11 -lcairo -lpthread -lrt -lm -ldl")
    endif()
endif(RK_EXAMPLE_PAINTER)

# ------------ Action Example -------
if (RK_EXAMPLE_ACTION)
   add_executable(action
    ${RK_HEADERS}
    ${RK_EXAMPLES_PATH}/action.cpp)
   add_dependencies(action redkite)
   target_link_libraries(action redkite)
   target_link_libraries(action "-lX11 -lcairo -lpthread -lrt -lm -ldl")
 endif(RK_EXAMPLE_ACTION)


# ------------ Dialog example -------

#add_executable(dialog
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_DIALOG})

#add_dependencies(dialog redkite)
#target_link_libraries(dialog redkite)
#target_link_libraries(dialog "-lX11 -lrt -lm -ldl")
#target_link_libraries(dialog ${RK_GRAPHICS_BACKEND_LINK_LIBS})

# ------------ Timer example -------
if (RK_EXAMPLE_TIMER)
add_executable(timer
  ${RK_HEADERS}
  ${RK_EXAMPLES_PATH}/timer.cpp)
add_dependencies(timer redkite)
target_link_libraries(timer redkite)
target_link_libraries(timer "-lX11 -lcairo -lpthread -lrt -lm -ldl")
endif(RK_EXAMPLE_TIMER)

# ------------ KeyEvent example -------

#add_executable(keyevent
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_KEYEVENT})

#add_dependencies(keyevent redkite)
#target_link_libraries(keyevent redkite)
#target_link_libraries(keyevent "-lX11 -lrt -lm -ldl")
#target_link_libraries(keyevent ${RK_GRAPHICS_BACKEND_LINK_LIBS})

# ------------ RkLineEdit example -------

#add_executable(lineedit
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_LINEEDIT})

#add_dependencies(lineedit redkite)
#target_link_libraries(lineedit redkite)
#target_link_libraries(lineedit "-lX11 -lrt -lm -ldl")
#target_link_libraries(lineedit ${RK_GRAPHICS_BACKEND_LINK_LIBS})
#endif () # ALL EXAMPLES

