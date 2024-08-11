cmake_minimum_required(VERSION 3.14)
project(MindSweeper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Copy configuration files to the build directory
configure_file(files/config.cfg ${CMAKE_BINARY_DIR}/files/config.cfg COPYONLY)

set(SOURCE_FILES main.cpp
        start_screen.cpp
        functions.h
        generate_board.cpp
        game_screen.cpp
)

add_executable(MindSweeper ${SOURCE_FILES})

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(MindSweeper sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
