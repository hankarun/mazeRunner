project(cameraApp)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(cameraApp ${SRC_FILES})
target_link_libraries(cameraApp raylib imgui rimgui mazeDrawer)

set_target_properties (cameraApp PROPERTIES
    FOLDER app
)