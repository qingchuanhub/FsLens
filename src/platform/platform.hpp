//src/platform/platform.hpp
#pragma once

#include <string>
#include <vector>
#include <cstdint>

#ifdef _WIN32
    #define FSLENS_WINDOWS 1
    typedef wchar_t fs_char;
    typedef std::wstring fs_string;
#else
    #define FSLENS_POSIX 1
    typedef char fs_char;
    typedef std::string fs_string;
#endif

// 文件条目结构体
struct FileEntry {
    fs_string path;        // 完整路径
    fs_string name;        // 文件名
    bool is_directory;     // 是否为目录
    uint64_t size;         // 文件大小 (字节)
    time_t modified_time;  // 修改时间戳

    FileEntry() : is_directory(false), size(0), modified_time(0) {}
};

// 跨平台遍历目录接口
bool traverse_directory(const fs_string& path, std::vector<FileEntry>& out_entries);