cmake_minimum_required(VERSION 3.16)
project(ApproximationPlot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(ApproximationPlot
    main.cpp
    plotwidget.h plotwidget.cpp
    approximator.h approximator.cpp
    func.h func.cpp
    task1.h task1.cpp
    error.h error.cpp
    task2.h
    task2.cpp  # если 
)

target_link_libraries(ApproximationPlot PRIVATE Qt5::Widgets)