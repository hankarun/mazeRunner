cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(raylib STATIC IMPORTED GLOBAL)
target_include_directories(raylib INTERFACE ${CMAKE_SOURCE_DIR}/rlib/include)

project(mazeRunner)

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_property(TARGET raylib PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/rlib/lib/libraylib.a)
    add_compile_definitions(PLATFORM_WEB)
    add_compile_definitions(GL_ENABLE_GET_PROC_ADDRESS=1)
else()
    set_property(TARGET raylib PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/rlib/lib/raylib.lib)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif ()

add_subdirectory(imgui)
add_subdirectory(libMaze)
add_subdirectory(rimgui)
add_subdirectory(mazeDrawer)
#add_subdirectory(cameraApp)
#add_subdirectory(imguiapp)
add_subdirectory(mazeBuilder)
