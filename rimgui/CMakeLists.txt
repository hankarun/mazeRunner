project(rimgui)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(rimgui STATIC ${SRC_FILES})
target_link_libraries(rimgui imgui raylib)
target_include_directories(rimgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
