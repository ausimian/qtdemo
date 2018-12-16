cmake_minimum_required(VERSION 3.10)

project(qttest)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets CONFIG REQUIRED)

add_executable(${PROJECT_NAME} 
    main.cc
    main_widget.cc
    main_widget.h
    my_model.cc
    my_model.h
    )

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)