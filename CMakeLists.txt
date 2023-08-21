cmake_minimum_required(VERSION 3.12)

project(jutils)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(JUTILS_INSTALL "Disable this if you want to use jutils as submodule" ON)
option(JUTILS_LOG_ENABLED "Enable log module" ON)
option(JUTILS_CONFIG_ENABLED "Enable JSON and INI configs modules" ON)

include(CheckIncludeFileCXX)
check_include_file_cxx("format" JUTILS_STD_FORMAT_FOUND)
if(${JUTILS_STD_FORMAT_FOUND})
    set(JUTILS_USE_FMT OFF)
else()
    set(JUTILS_USE_FMT ON)
endif()

if(${JUTILS_USE_FMT})
    find_package(fmt CONFIG REQUIRED)
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
    endif()

    if(${JUTILS_USE_FMT})
        target_link_libraries(jutils INTERFACE fmt)
    endif()
endif()
if(NOT ${JUTILS_INSTALL})
    target_include_directories(jutils INTERFACE include)
endif()

if(${JUTILS_INSTALL})
    set(JUTILS_CONFIG_INSTALL_DIR "lib/cmake/${PROJECT_NAME}")
    set(JUTILS_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
    set(JUTILS_VERSION_CONFIG "${JUTILS_GENERATED_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
    set(JUTILS_PROJECT_CONFIG "${JUTILS_GENERATED_DIR}/${PROJECT_NAME}Config.cmake")
    set(JUTILS_TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${JUTILS_VERSION_CONFIG}"
        VERSION "1.0.0"
        COMPATIBILITY SameMajorVersion
    )
    configure_package_config_file(
        "cmake/config.cmake.in" 
        "${JUTILS_PROJECT_CONFIG}" 
        INSTALL_DESTINATION "${JUTILS_CONFIG_INSTALL_DIR}"
    )

    if(${JUTILS_CONFIG_ENABLED})
        install(DIRECTORY include/jutils/ DESTINATION include/jutils FILES_MATCHING PATTERN "*.h")
    else()
        install(DIRECTORY include/jutils/ DESTINATION include/jutils FILES_MATCHING
            PATTERN "include/jutils/configs" EXCLUDE 
            PATTERN "*.h"
        )
    endif()
    install(
        FILES "${JUTILS_PROJECT_CONFIG}" "${JUTILS_VERSION_CONFIG}"
        DESTINATION "${JUTILS_CONFIG_INSTALL_DIR}"
    )
    install(
        TARGETS jutils
        EXPORT "${JUTILS_TARGETS_EXPORT_NAME}"
        LIBRARY DESTINATION lib/
    )
    install(
        EXPORT "${JUTILS_TARGETS_EXPORT_NAME}"
        DESTINATION "${JUTILS_CONFIG_INSTALL_DIR}"
    )
endif()