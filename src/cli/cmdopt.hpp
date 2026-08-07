// src/cli/cmdopt.hpp
#pragma once

#include "../platform/platform.hpp"
#include <cstring>
#include <iostream>
#include <cwctype>      // 用于 iswdigit

#ifdef _WIN32
#include <windows.h>
#endif

// 统一字符串字面量宏
#ifdef _WIN32
    #define FSLENS_TEXT(x) L##x
    #define FSLENS_VERSION L"release version 1.0.0"   // 宽字符串
#else
    #define FSLENS_TEXT(x) x
    #define FSLENS_VERSION "release version 1.0.0"
#endif

struct Options {
    fs_string search_path = FSLENS_TEXT(".");
    fs_string name_pattern = FSLENS_TEXT("");
    bool recursive = false;
    bool show_help = false;
    bool show_version = false;
    fs_string size_filter = FSLENS_TEXT("");
    uint64_t min_size = 0;
    uint64_t max_size = UINT64_MAX;
};

// 解析大小过滤字符串
inline bool parse_size_filter(const fs_string& input, uint64_t& out_min, uint64_t& out_max) {
    if (input.empty()) {
        out_min = 0;
        out_max = UINT64_MAX;
        return true;
    }

    fs_string str = input;
    bool is_min = true;
    bool is_exact = false;
    bool is_range = false;
    fs_string min_str, max_str;

    // 检查是否为范围格式 "100M-500M"
    size_t dash_pos = str.find(FSLENS_TEXT('-'));
    if (dash_pos != fs_string::npos && dash_pos > 0 && 
        str[0] != FSLENS_TEXT('+') && str[0] != FSLENS_TEXT('-')) {
        is_range = true;
        min_str = str.substr(0, dash_pos);
        max_str = str.substr(dash_pos + 1);
    } else {
        // 处理 +N, -N, N
        if (str[0] == FSLENS_TEXT('+')) {
            is_min = true;
            str.erase(0, 1);
        } else if (str[0] == FSLENS_TEXT('-')) {
            is_min = false;
            str.erase(0, 1);
        } else {
            is_exact = true;
        }
        min_str = str;
        max_str = str;
    }

    auto parse_single = [&](const fs_string& s) -> uint64_t {
        if (s.empty()) return 0;
        size_t i = 0;
        while (i < s.size() && (std::iswdigit(s[i]) || s[i] == FSLENS_TEXT('.'))) i++;
        double num = std::stod(s.substr(0, i));
        fs_string suffix = s.substr(i);
        int mult = 1;
        if (suffix == FSLENS_TEXT("B") || suffix.empty()) mult = 1;
        else if (suffix == FSLENS_TEXT("K") || suffix == FSLENS_TEXT("k")) mult = 1024;
        else if (suffix == FSLENS_TEXT("M") || suffix == FSLENS_TEXT("m")) mult = 1024 * 1024;
        else if (suffix == FSLENS_TEXT("G") || suffix == FSLENS_TEXT("g")) mult = 1024 * 1024 * 1024;
        return static_cast<uint64_t>(num * mult);
    };

    if (is_range) {
        out_min = parse_single(min_str);
        out_max = parse_single(max_str);
        if (out_min > out_max) std::swap(out_min, out_max);
        return true;
    }

    uint64_t bytes = parse_single(min_str);
    if (is_exact) {
        out_min = bytes;
        out_max = bytes;
    } else if (is_min) {
        out_min = bytes;
        out_max = UINT64_MAX;
    } else {
        out_min = 0;
        out_max = bytes;
    }
    return true;
}

inline Options parse_options(int argc, fs_char* argv[]) {
    Options opts;

    for (int i = 1; i < argc; ++i) {
        fs_string arg = argv[i];

        if (arg == FSLENS_TEXT("-h") || arg == FSLENS_TEXT("--help")) {
            opts.show_help = true;
            return opts;
        }
        else if (arg == FSLENS_TEXT("-v") || arg == FSLENS_TEXT("--version")) {
            opts.show_version = true;
            return opts;
        }
        else if (arg == FSLENS_TEXT("-s") || arg == FSLENS_TEXT("--size")) {
            if (i + 1 < argc) {
                opts.size_filter = argv[++i];
                if (!parse_size_filter(opts.size_filter, opts.min_size, opts.max_size)) {
                    std::wcerr << L"错误: 无效的大小过滤参数 '" << opts.size_filter << L"'" << std::endl;
                    opts.show_help = true;
                    return opts;
                }
            }
        }
        else if (arg == FSLENS_TEXT("-p") || arg == FSLENS_TEXT("--path")) {
            if (i + 1 < argc) {
                opts.search_path = argv[++i];
            }
        }
        else if (arg == FSLENS_TEXT("-n") || arg == FSLENS_TEXT("--name")) {
            if (i + 1 < argc) {
                opts.name_pattern = argv[++i];
            }
        }
        else if (arg == FSLENS_TEXT("-r") || arg == FSLENS_TEXT("--recursive")) {
            opts.recursive = true;
        }
        else {
            opts.search_path = arg;
        }
    }
    return opts;
}

inline void print_help() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        DWORD written;
        const wchar_t* lines[] = {
            L"FsLens - 跨平台文件检索工具 (",
            FSLENS_VERSION,
            L")\n",
            L"用法: FsLens [选项]\n",
            L"  -p, --path <路径>    指定搜索目录 (默认: .)\n",
            L"  -n, --name <模式>    文件名包含匹配 (如: main, .txt)\n",
            L"  -s, --size <+N|-N>  按文件大小过滤 (如: +10M, -500k, 1M-10M)\n",
            L"                      支持 B, K, M, G 后缀\n",
            L"  -r, --recursive      递归搜索子目录\n",
            L"  -v, --version        显示版本信息\n",
            L"  -h, --help           显示此帮助信息\n"
        };
        for (const auto& line : lines) {
            WriteConsoleW(hConsole, line, wcslen(line), &written, NULL);
        }
        return;
    }
#endif
    std::cout << "FsLens - 跨平台文件检索工具 (" << FSLENS_VERSION << ")" << std::endl;
    std::cout << "用法: FsLens [选项]" << std::endl;
    std::cout << "  -p, --path <路径>    指定搜索目录 (默认: .)" << std::endl;
    std::cout << "  -n, --name <模式>    文件名包含匹配 (如: main, .txt)" << std::endl;
    std::cout << "  -s, --size <+N|-N>  按文件大小过滤 (如: +10M, -500k, 1M-10M)" << std::endl;
    std::cout << "                      支持 B, K, M, G 后缀" << std::endl;
    std::cout << "  -r, --recursive      递归搜索子目录" << std::endl;
    std::cout << "  -v, --version        显示版本信息" << std::endl;
    std::cout << "  -h, --help           显示此帮助信息" << std::endl;
}

inline void print_version() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        DWORD written;
        const wchar_t* msg = FSLENS_VERSION L"\n";
        WriteConsoleW(hConsole, msg, wcslen(msg), &written, NULL);
        return;
    }
#endif
    std::cout << FSLENS_VERSION << std::endl;
}