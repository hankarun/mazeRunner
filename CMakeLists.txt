cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(mazeRunner)

add_library(raylib STATIC IMPORTED GLOBAL)
target_include_directories(raylib INTERFACE ${CMAKE_SOURCE_DIR}/rlib/include)
set_property(TARGET raylib PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/rlib/lib/raylib.lib)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
add_executable(${PROJECT_NAME} 
	main.cpp)
	
target_link_libraries(${PROJECT_NAME} raylib)