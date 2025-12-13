cmake_minimum_required(VERSION 3.15)
project(TimerApp)

set(CMAKE_CXX_STANDARD 20)

# My own app
add_executable(TimerApp src/main.cpp)

target_include_directories(TimerApp PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

find_package(raylib CONFIG REQUIRED)
target_link_libraries(TimerApp PUBLIC
    raylib
)