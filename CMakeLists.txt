cmake_minimum_required(VERSION 3.16)
project(MinesweeperQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(MinesweeperQt
    main.cpp
    mainwindow.h
    mainwindow.cpp
    cellbutton.h
    cellbutton.cpp
    gameboard.h
    gameboard.cpp
)

target_link_libraries(MinesweeperQt PRIVATE Qt6::Widgets)
