cmake_minimum_required(VERSION 4.2)
project(105Assessment)

set(CMAKE_CXX_STANDARD 20)

add_executable(105Assessment src/main.cpp
        src/Menu.cpp
        include/Menu.h
        src/Table.cpp
        include/Table.h
        src/Order.cpp
        include/Order.h
        src/manager.cpp
        include/manager.h
)
