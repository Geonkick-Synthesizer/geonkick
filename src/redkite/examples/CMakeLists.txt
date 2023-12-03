#set(RK_EXAMPLES_SOURCES ${RK_EXAMPLES_PATH}/hello.cpp)
#set(RK_EXAMPLES_SOURCES_CHILD ${RK_EXAMPLES_PATH}/child.cpp)
#set(RK_EXAMPLES_SOURCES_PROPERTY ${RK_EXAMPLES_PATH}/property.cpp)
set(RK_EXAMPLES_SOURCES_LABEL ${RK_EXAMPLES_PATH}/label.cpp)
#set(RK_EXAMPLES_SOURCES_WIDGET_COLORS ${RK_EXAMPLES_PATH}/widget_colors.cpp)
set(RK_EXAMPLES_SOURCES_PAINTER ${RK_EXAMPLES_PATH}/painter.cpp)
#set(RK_EXAMPLES_SOURCES_ACTION ${RK_EXAMPLES_PATH}/action.cpp)
#set(RK_EXAMPLES_SOURCES_DIALOG ${RK_EXAMPLES_PATH}/dialog.cpp)
#set(RK_EXAMPLES_SOURCES_TIMER ${RK_EXAMPLES_PATH}/timer.cpp)
#set(RK_EXAMPLES_SOURCES_KEYEVENT ${RK_EXAMPLES_PATH}/keyevent.cpp)
#set(RK_EXAMPLES_SOURCES_LINEEDIT ${RK_EXAMPLES_PATH}/lineedit.cpp)

#if (RK_GRAPHICS_BACKEND MATCHES Cairo)
#set(RK_GRAPHICS_BACKEND_LINK_LIBS "-lcairo")
#elseif(RK_GRAPHICS_BACKEND MATCHES Direct2D)
#set(CMAKE_CXX_FLAGS "-ld2d1")
#endif()

# ------------ Hello exmaple -------

#add_executable(hello ${RK_EXEC_OPTION}
#	${RK_HEADERS}
#	${RK_EXAMPLES_SOURCES})

#add_dependencies(hello redkite)
#target_link_libraries(hello redkite)
#if (CMAKE_SYSTEM_NAME MATCHES Windows)
  #target_link_libraries(hello "-mwindows -ld3d11 -ld2d1 -ldwrite -lm")
#  target_link_libraries(hello "-mwindows -lstdc++ -lpthread -lm")
#else()
#  target_link_libraries(hello "-lX11 -lpthread -lrt -lm -ldl")
#  target_link_libraries(hello ${RK_GRAPHICS_BACKEND_LINK_LIBS})
#endif()

#if (RK_ALL_EXMAPLES)
# ------------ Child Example -------

#add_executable(child ${RK_EXEC_OPTION}
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_CHILD})
#add_dependencies(child redkite)
#target_link_libraries(child redkite)
#if (CMAKE_SYSTEM_NAME MATCHES Windows)
#  target_link_libraries(child "-mwindows -s -static -ld3d11 -ld2d1 -ldwrite -lm")
#else()
#  target_link_libraries(child "-lX11 -lpthread -lrt -lm -ldl")
#  target_link_libraries(child ${RK_GRAPHICS_BACKEND_LINK_LIBS})
#endif()

# ------------ Property -------

#add_executable(property
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_PROPERTY})

#add_dependencies(property redkite)
#target_link_libraries(property redkite)
#target_link_libraries(property "-lX11 -lrt -lm -ldl")
#target_link_libraries(property ${RK_GRAPHICS_BACKEND_LINK_LIBS})

# ------------ Label Example -------

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

#------------- Widget Colors --------

#add_executable(widget_colors
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_WIDGET_COLORS})

#add_dependencies(widget_colors redkite)
#target_link_libraries(widget_colors redkite)
#target_link_libraries(widget_colors "-lX11 -lrt -lm -ldl")
#target_link_libraries(widget_colors ${RK_GRAPHICS_BACKEND_LINK_LIBS})

# ------------ Painter Exmaple -------
add_executable(painter
  ${RK_HEADERS}
  ${RK_EXAMPLES_SOURCES_PAINTER})

add_dependencies(painter redkite)
target_link_libraries(painter redkite)
if (CMAKE_SYSTEM_NAME MATCHES Windows)
 target_link_libraries(painter "-mwindows -lstdc++ -lm -lcairo")
else ()
 target_link_libraries(painter ${RK_LINK_}"-lX11 -lrt -lm -ldl")
 target_link_libraries(painter ${RK_GRAPHICS_BACKEND_LINK_LIBS})
endif()
# ------------ Action Exmaple -------

#add_executable(action
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_ACTION})

#add_dependencies(action redkite)
#target_link_libraries(action redkite)
#target_link_libraries(action "-lX11 -lrt -lm -ldl")
#target_link_libraries(action ${RK_GRAPHICS_BACKEND_LINK_LIBS})


# ------------ Dialog example -------

#add_executable(dialog
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_DIALOG})

#add_dependencies(dialog redkite)
#target_link_libraries(dialog redkite)
#target_link_libraries(dialog "-lX11 -lrt -lm -ldl")
#target_link_libraries(dialog ${RK_GRAPHICS_BACKEND_LINK_LIBS})

# ------------ Timer example -------

#add_executable(timer
#  ${RK_HEADERS}
#  ${RK_EXAMPLES_SOURCES_TIMER})

#add_dependencies(timer redkite)
#target_link_libraries(timer redkite)
#target_link_libraries(timer "-lX11 -lrt -lm -ldl")
#target_link_libraries(timer ${RK_GRAPHICS_BACKEND_LINK_LIBS})

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

