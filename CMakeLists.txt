cmake_minimum_required(VERSION 3.12)

project(jutils)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    # Enable __cplusplus and __VA_OPT__ macros
    add_compile_options(/Zc:__cplusplus /permissive- /Zc:preprocessor)
endif()

option(JUTILS_FMT_ENABLED "Enable fmt lib even if format header is found" OFF)
option(JUTILS_LOG_ENABLED "Enable log module" ON)

include(CheckIncludeFileCXX)
check_include_file_cxx("format" JUTILS_STD_FORMAT_FOUND)
if(JUTILS_STD_FORMAT_FOUND AND (NOT JUTILS_FMT_ENABLED))
    set(JUTILS_USE_FMT OFF)
else()
    set(JUTILS_USE_FMT ON)
endif()

list(APPEND JUTILS_HEADER_FILES
    include/jutils/base_types.h
    include/jutils/jarray.h
    include/jutils/jasync_task_queue.h
    include/jutils/jdelegate.h
    include/jutils/jdelegate_multicast.h
    include/jutils/jdescriptor_table.h
    include/jutils/format.h
    include/jutils/jdefer.h
    include/jutils/jlist.h
    include/jutils/jmap.h
    include/jutils/jmap_hash.h
    include/jutils/jmemory.h
    include/jutils/jpool.h
    include/jutils/jset.h
    include/jutils/jset_hash.h
    include/jutils/jstring.h
    include/jutils/jstringID.h
    include/jutils/juid.h
    include/jutils/log.h
    include/jutils/type_traits.h

    include/jutils/math/common.h
    include/jutils/math/hash.h
    include/jutils/math/vec_base.h
    include/jutils/math/vec2.h
    include/jutils/math/vec3.h
    include/jutils/math/vec4.h
    include/jutils/math/mat_base.h
    include/jutils/math/mat2x2.h
    include/jutils/math/mat2x3.h
    include/jutils/math/mat2x4.h
    include/jutils/math/mat3x3.h
    include/jutils/math/mat3x2.h
    include/jutils/math/mat3x4.h
    include/jutils/math/mat4x4.h
    include/jutils/math/mat4x2.h
    include/jutils/math/mat4x3.h
)

if(JUTILS_USE_FMT)
    find_package(fmt REQUIRED)
    list(APPEND JUTILS_MACRO_DEFINITIONS JUTILS_USE_FMT)
    list(APPEND JUTILS_LIBS fmt)
endif()
if(NOT JUTILS_LOG_ENABLED)
    list(APPEND JUTILS_MACRO_DEFINITIONS JUTILS_LOG_DISABLED)
endif()

add_library(jutils INTERFACE ${JUTILS_HEADER_FILES})
target_include_directories(jutils INTERFACE include)
target_compile_definitions(jutils INTERFACE ${JUTILS_MACRO_DEFINITIONS})
target_link_libraries(jutils INTERFACE ${JUTILS_LIBS})