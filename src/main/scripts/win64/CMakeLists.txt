cmake_minimum_required(VERSION 2.8.7)
project(retro-installer)

include_directories("${CMAKE_SOURCE_DIR}/libcurl/win64/include")

file(
        GLOB_RECURSE
        source_files
        ${CMAKE_SOURCE_DIR} ./*.cpp
        ${CMAKE_SOURCE_DIR} ./*.c
        ${CMAKE_SOURCE_DIR} ./*.hpp
        ${CMAKE_SOURCE_DIR} ./*.h
)

add_executable(retro-installer main.cpp)

target_link_libraries(retro-installer ${CMAKE_SOURCE_DIR}/libcurl/win64/lib/libcurl.a)

install(TARGETS DESTINATION ${CMAKE_INSTALL_PREFIX})
