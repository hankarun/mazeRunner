project(libMaze)

file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(libMaze STATIC ${SRC_FILES})
target_include_directories(libMaze PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties (libMaze PROPERTIES
    FOLDER lib
)