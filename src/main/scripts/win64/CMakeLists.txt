cmake_minimum_required(VERSION 2.8.7)
project(retro-installer)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCURL_STATICLIB -static-libstdc++ -static-libgcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCURL_STATICLIB -static-libstdc++ -static-libgcc")

set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive") 

include_directories("${CMAKE_SOURCE_DIR}/curl/win64/include")
include_directories("${CMAKE_SOURCE_DIR}/libarchive/win64/include")

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
target_link_libraries(retro-installer ${CMAKE_SOURCE_DIR}/libarchive/win64/lib/libarchive_static.a)
target_link_libraries(retro-installer ${CMAKE_SOURCE_DIR}/zlib/win64/lib/libzlibstatic.a)
target_link_libraries(retro-installer wsock32)
target_link_libraries(retro-installer ws2_32)
target_link_libraries(retro-installer pthread)

