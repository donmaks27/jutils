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
    include/jutils/jpool_simple.h
    include/jutils/jset.h
    include/jutils/jset_hash.h
    include/jutils/jstring.h
    include/jutils/jstringID.h
    include/jutils/juid.h
    include/jutils/log.h
    include/jutils/type_traits.h

    include/jutils/math/math.h
    include/jutils/math/hash.h
    include/jutils/math/vector2.h
    include/jutils/math/vector3.h
    include/jutils/math/vector4.h
    include/jutils/math/box2.h
    include/jutils/math/box3.h
    include/jutils/math/math_matrix.h
    include/jutils/math/matrix2.h
    include/jutils/math/matrix2x3.h
    include/jutils/math/matrix2x4.h
    include/jutils/math/matrix3.h
    include/jutils/math/matrix3x2.h
    include/jutils/math/matrix3x4.h
    include/jutils/math/matrix4.h
    include/jutils/math/matrix4x2.h
    include/jutils/math/matrix4x3.h

    include/jutils/math/base_types/vector_base.h
    include/jutils/math/base_types/vector_operations.h
    include/jutils/math/base_types/vector2_base.h
    include/jutils/math/base_types/vector3_base.h
    include/jutils/math/base_types/vector4_base.h
    include/jutils/math/base_types/box_base.h
    include/jutils/math/base_types/box_operations.h
    include/jutils/math/base_types/box2_base.h
    include/jutils/math/base_types/box3_base.h
    include/jutils/math/base_types/matrix_base.h
    include/jutils/math/base_types/matrix_operations.h
    include/jutils/math/base_types/matrix2_base.h
    include/jutils/math/base_types/matrix2x2_base.h
    include/jutils/math/base_types/matrix2x3_base.h
    include/jutils/math/base_types/matrix2x4_base.h
    include/jutils/math/base_types/matrix3_base.h
    include/jutils/math/base_types/matrix3x2_base.h
    include/jutils/math/base_types/matrix3x3_base.h
    include/jutils/math/base_types/matrix3x4_base.h
    include/jutils/math/base_types/matrix4_base.h
    include/jutils/math/base_types/matrix4x2_base.h
    include/jutils/math/base_types/matrix4x3_base.h
    include/jutils/math/base_types/matrix4x4_base.h

    include/jutils/configs/ini.h
    include/jutils/configs/ini_parser.h
    include/jutils/configs/json.h
    include/jutils/configs/json_parser.h
)
list(APPEND JUTILS_SOURCE_FILES
    src/configs/ini_parser.cpp
    src/configs/json_parser.cpp
)

if(JUTILS_USE_FMT)
    find_package(fmt REQUIRED)
    list(APPEND JUTILS_MACRO_DEFINITIONS JUTILS_USE_FMT)
    list(APPEND JUTILS_LIBS fmt)
endif()
if(NOT JUTILS_LOG_ENABLED)
    list(APPEND JUTILS_MACRO_DEFINITIONS JUTILS_LOG_DISABLED)
elseif(JUTILS_LOG_COLOR_ENABLED)
    list(APPEND JUTILS_MACRO_DEFINITIONS JUTILS_LOG_COLOR_ENABLED)
endif()

if(JUTILS_CONFIG_ENABLED)
    add_library(jutils STATIC ${JUTILS_HEADER_FILES} ${JUTILS_SOURCE_FILES})
    target_include_directories(jutils PUBLIC include PRIVATE
        third_party/mattiasgustavsson/include
        third_party/nlohmann/include
    )
    target_compile_definitions(jutils PUBLIC ${JUTILS_MACRO_DEFINITIONS})
    target_link_libraries(jutils PUBLIC ${JUTILS_LIBS})
else()
    add_library(jutils INTERFACE ${JUTILS_HEADER_FILES})
    target_include_directories(jutils INTERFACE include)
    target_compile_definitions(jutils INTERFACE ${JUTILS_MACRO_DEFINITIONS})
    target_link_libraries(jutils INTERFACE ${JUTILS_LIBS})
endif()