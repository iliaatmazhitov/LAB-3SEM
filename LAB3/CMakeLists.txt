cmake_minimum_required(VERSION 3.28)
project(laboratory3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)


add_executable(laboratory3 main.cpp
        backpack.h
        MemoryManager.h
        MemoryManager.cpp
        MainWindow.h
        backpack.cpp)

target_link_libraries(laboratory3 PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Charts
)

target_include_directories(laboratory3 PRIVATE include)