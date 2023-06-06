project(mazeBuilder)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(mazeBuilder ${SRC_FILES})
target_link_libraries(mazeBuilder raylib imgui libMaze mazeDrawer)

set_target_properties (mazeBuilder PROPERTIES
    FOLDER app
)