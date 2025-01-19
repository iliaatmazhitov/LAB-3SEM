
cmake_minimum_required(VERSION 3.28)
project(laboratory2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)

add_executable(laboratory2
        src/main.cpp
        include/algorithms/Sorts.h
        include/algorithms/Comparators.h
        include/MainWindow.h
        src/MainWindow.cpp
        include/CSVParser.h
        src/CSVParser.cpp
)

add_custom_command(
        TARGET laboratory2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:laboratory2>/data/
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/data/people.csv
        $<TARGET_FILE_DIR:laboratory2>/data/people.csv
)

target_link_libraries(laboratory2 PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Charts
)

target_include_directories(laboratory2 PRIVATE include)
