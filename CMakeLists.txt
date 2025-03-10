cmake_minimum_required(VERSION 3.12)

project(jutils)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    # Enable __cplusplus and __VA_OPT__ macros
    add_compile_options(/Zc:__cplusplus /permissive- /Zc:preprocessor)
endif()

list(APPEND JUTILS_HEADER_FILES
    include/jutils/core.h
    include/jutils/base_types.h
    include/jutils/type_traits.h
    include/jutils/uid.h
    include/jutils/delegate.h
    include/jutils/multidelegate.h
    include/jutils/format.h
    include/jutils/log.h
    include/jutils/stringID.h

    include/jutils/jasync_task_queue.h
    include/jutils/jdefer.h
    include/jutils/jdescriptor_table.h
    include/jutils/jmemory.h
    include/jutils/jpool.h

    include/jutils/math/math.h
    include/jutils/math/hash.h
    include/jutils/math/format_glm.h

    include/jutils/macro/args_count.h
    include/jutils/macro/if_has_args.h
    include/jutils/macro/wrap.h
)

add_library(jutils INTERFACE ${JUTILS_HEADER_FILES})
target_include_directories(jutils INTERFACE include)