cmake_minimum_required(VERSION 3.12)

project(jutils)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(jutils STATIC src/json/json_parser.cpp)
target_include_directories(jutils INTERFACE include)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(jutils INTERFACE JDEBUG)
else()
    target_compile_definitions(jutils INTERFACE JUTILS_LOG_DISABLED)
endif()

target_link_libraries(jutils PRIVATE fmt)

install(DIRECTORY include/jutils/ DESTINATION include/jutils FILES_MATCHING PATTERN "*.h")