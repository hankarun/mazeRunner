project(cameraApp)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(cameraApp ${SRC_FILES})
target_link_libraries(cameraApp raylib imgui rimgui)

set_target_properties (cameraApp PROPERTIES
    FOLDER app
)

if (EMSCRIPTEN)
target_link_options(cameraApp
PUBLIC
    "SHELL:-s GL_ENABLE_GET_PROC_ADDRESS=1"
)
endif()