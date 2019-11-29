cmake_minimum_required(VERSION 2.8.7)
project(retro-installer)

file(
        GLOB_RECURSE
        source_files
        ${CMAKE_SOURCE_DIR} ./*.cpp
        ${CMAKE_SOURCE_DIR} ./*.c
        ${CMAKE_SOURCE_DIR} ./*.hpp
        ${CMAKE_SOURCE_DIR} ./*.h
)

add_executable(retro-installer main.cpp)
