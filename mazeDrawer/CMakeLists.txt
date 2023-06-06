project(mazeDrawer)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(mazeDrawer STATIC ${SRC_FILES})
target_link_libraries(mazeDrawer raylib libMaze)
target_include_directories(mazeDrawer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties (mazeDrawer PROPERTIES
    FOLDER lib
)