project(mazeBuilder)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(mazeBuilder ${SRC_FILES})
target_link_libraries(mazeBuilder raylib imgui libMaze)