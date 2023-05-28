cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

add_library(raylib STATIC IMPORTED GLOBAL)
target_include_directories(raylib INTERFACE ${CMAKE_SOURCE_DIR}/rlib/include)

project(mazeRunner)

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_property(TARGET raylib PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/rlib/lib/libraylib.a)
    add_compile_definitions(PLATFORM_WEB)
else()
    set_property(TARGET raylib PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/rlib/lib/raylib.lib)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif ()


add_executable(${PROJECT_NAME} 
    src/gamedrawer.h
    src/gamedrawer.cpp
    src/boardgenerator.h
    src/boardgenerator.cpp
    src/boardcolors.h
    src/boardcolors.cpp
    src/boardsolver.h
    src/boardsolver.cpp
    src/game.h
    src/game.cpp
    src/board.h
    src/board.cpp
    src/common.h
    src/common.cpp
    src/cell.h
    src/cell.cpp
    src/raygui.h
	src/main.cpp)
	
target_link_libraries(${PROJECT_NAME} raylib)