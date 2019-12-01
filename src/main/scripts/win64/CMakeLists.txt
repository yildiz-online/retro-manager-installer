cmake_minimum_required(VERSION 2.8.7)
project(retro-installer)

include_directories("${CMAKE_SOURCE_DIR}/curl/win64/include")

file(
        GLOB_RECURSE
        source_files
        ${CMAKE_SOURCE_DIR} ../../c++/*.cpp
        ${CMAKE_SOURCE_DIR} ../../c++/*.c
        ${CMAKE_SOURCE_DIR} ../../c++/*.hpp
        ${CMAKE_SOURCE_DIR} ../../c++/*.h
)

add_executable(retro-installer ../../c++/main.cpp)

target_link_libraries(retro-installer ${CMAKE_SOURCE_DIR}/curl/win64/lib/libcurl.a)

