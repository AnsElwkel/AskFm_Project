cmake_minimum_required(VERSION 3.29)
project(AskFm_Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(AskFm_Project main.cpp
        AskFm_Site.cpp
        systemOperation.cpp
        userInterface.cpp
        headers/AskFm_Site.h
        headers/userInterface.h
        headers/systemOperation.h
        Menu.cpp
        headers/Menu.h
        headers/Headers.h
        headers/FILE_MANAGER.h
        FILE_MANAGER.cpp)
