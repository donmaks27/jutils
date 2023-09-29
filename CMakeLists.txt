cmake_minimum_required(VERSION 3.12)

project(jutils)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(JUTILS_FMT_ENABLED "Enable fmt lib even if format module not found" OFF)
option(JUTILS_LOG_ENABLED "Enable log module" ON)
option(JUTILS_LOG_COLOR_ENABLED "Enable colored logs (works only with fmt lib)" ON)
option(JUTILS_CONFIG_ENABLED "Enable JSON and INI configs modules" ON)

include(CheckIncludeFileCXX)
check_include_file_cxx("format" JUTILS_STD_FORMAT_FOUND)
if((${JUTILS_STD_FORMAT_FOUND}) AND (NOT ${JUTILS_FMT_ENABLED}))
    set(JUTILS_USE_FMT OFF)
else()
    set(JUTILS_USE_FMT ON)
endif()
if(${JUTILS_USE_FMT})
    find_package(fmt REQUIRED)
endif()

if(${JUTILS_CONFIG_ENABLED})
    add_library(jutils STATIC 
        src/configs/json_parser.cpp 
        src/configs/ini_parser.cpp
    )
    target_include_directories(jutils PRIVATE 
        include
        third_party/mattiasgustavsson/include
        third_party/nlohmann/include
    )

    if(${JUTILS_USE_FMT})
        target_compile_definitions(jutils PUBLIC JUTILS_USE_FMT)
    endif()
    if(NOT ${JUTILS_LOG_ENABLED})
        target_compile_definitions(jutils PUBLIC JUTILS_LOG_DISABLED)
    elseif(${JUTILS_LOG_COLOR_ENABLED})
        target_compile_definitions(jutils PUBLIC JUTILS_LOG_COLOR_ENABLED)
    endif()

    if(${JUTILS_USE_FMT})
        target_link_libraries(jutils PRIVATE fmt)
    endif()
else()
    add_library(jutils INTERFACE)

    if(${JUTILS_USE_FMT})
        target_compile_definitions(jutils INTERFACE JUTILS_USE_FMT)
    endif()
    if(NOT ${JUTILS_LOG_ENABLED})
        target_compile_definitions(jutils INTERFACE JUTILS_LOG_DISABLED)
    elseif(${JUTILS_LOG_COLOR_ENABLED})
        target_compile_definitions(jutils INTERFACE JUTILS_LOG_COLOR_ENABLED)
    endif()

    if(${JUTILS_USE_FMT})
        target_link_libraries(jutils INTERFACE fmt)
    endif()
endif()
target_include_directories(jutils INTERFACE include)