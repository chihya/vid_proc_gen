cmake_minimum_required(VERSION 3.6)
project(vid_proc_gen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if ("${CYGWIN}" OR "${MINGW}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${TARGET_ARCH}/${CMAKE_BUILD_TYPE})

set(SOURCE_FILES main.cpp)
add_executable(vid_proc_gen ${SOURCE_FILES})

if ("${CMAKE_COMPILER_IS_GNUC_PLUS_PLUS}" OR "${CMAKE_COMPILER_IS_GNUC}" OR "${CYGWIN}" OR "${MINGW}")
    set(GNU_COMPILER 1)
else ()
    set(GNU_COMPILER 0)
endif ()

if (${GNU_COMPILER})
    add_custom_command(TARGET vid_proc_gen
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:vid_proc_gen>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif ()
