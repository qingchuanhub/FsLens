cmake_minimum_required(VERSION 3.10)
project(FsLens VERSION 1.0.0 LANGUAGES CXX)

# 启用 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 针对 Windows/MinGW 的特定配置
if (MINGW OR WIN32)
    # 定义 Unicode 宏
    add_definitions(-DUNICODE -D_UNICODE)
    # 静态链接运行时库，使 exe 可独立运行
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    # 禁用控制台窗口的 DEP 警告（可选）
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mconsole")
endif()

# 收集源文件
set(SOURCES
    src/FsLens.cpp
    src/scanner.cpp
    src/platform/win32.cpp
)

# 添加头文件搜索路径
include_directories(src)

# 生成可执行文件
add_executable(FsLens ${SOURCES})