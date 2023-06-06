project(imgui)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(imgui STATIC ${SRC_FILES})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties (imgui PROPERTIES
    FOLDER external
)