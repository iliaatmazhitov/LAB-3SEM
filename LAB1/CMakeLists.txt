cmake_minimum_required(VERSION 3.25)
project(dumb_pointer)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0167 NEW)

find_package(Curses REQUIRED)
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)

set(SOURCES
    src/main.cpp
    src/tests.cpp
    src/UI.cpp
    src/timing.cpp
)

add_executable(pointer_menu ${SOURCES})

target_include_directories(pointer_menu PRIVATE
    include
    ${CURSES_INCLUDE_DIR}
)

target_link_libraries(pointer_menu PRIVATE
    ${CURSES_LIBRARIES}
    Boost::iostreams
    Boost::system
    Boost::filesystem
)

message(STATUS "CURSES_LIBRARIES: ${CURSES_LIBRARIES}")
message(STATUS "CURSES_INCLUDE_DIR: ${CURSES_INCLUDE_DIR}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")