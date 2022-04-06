cmake_minimum_required(VERSION 3.13)
project(command_dispatcher)
set(CMAKE_CXX_STANDARD 14)

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES 
    src/main.cpp
)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(command_dispatcher ${SOURCE_FILES})

target_include_directories(command_dispatcher PUBLIC include)
