cmake_minimum_required(VERSION 3.14)
project(Minesweeper-in-C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Copy the entire files directory to the build directory (For sharing on Github)
file(COPY ${CMAKE_SOURCE_DIR}/files DESTINATION ${CMAKE_BINARY_DIR})

set(SOURCE_FILES main.cpp
        start_screen.cpp
        functions.h
        generate_board.cpp
        game_screen.cpp
)

add_executable(Minesweeper-in-C ${SOURCE_FILES})

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(Minesweeper-in-C sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
