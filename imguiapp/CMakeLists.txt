project(imguiApp)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(imguiApp ${SRC_FILES})
target_link_libraries(imguiApp raylib imgui rimgui libMaze mazeDrawer)

set_target_properties (imguiApp PROPERTIES
    FOLDER app
)