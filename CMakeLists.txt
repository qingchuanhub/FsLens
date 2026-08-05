cmake_minimum_required(VERSION 3.20)
project(FsLens LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Windows 全局定义，兼容CMD、宽字符路径遍历
if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE -DWIN32)
endif()

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 源码目录
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")

add_executable(FsLens ${SRC_FILES})

# MinGW 静态链接，运行不需要附带dll
if(MINGW)
    target_link_options(FsLens PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()

# Release / Debug 编译选项
target_compile_options(FsLens PRIVATE
    $<$<CONFIG:Release>:-O2 -Wall -Wextra>
    $<$<CONFIG:Debug>:-g -Og -Wall -Wextra>
)