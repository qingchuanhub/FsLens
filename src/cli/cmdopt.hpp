// src/cli/cmdopt.hpp
#pragma once

#include "../platform/platform.hpp"
#include <cstring>
#include <iostream>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
    #define FSLENS_TEXT(x) L##x
#else
    #define FSLENS_TEXT(x) x
#endif

#define FSLENS_VERSION "1.0.0"

inline bool parse_size_filter(const fs_string& input, uint64_t& out_max) {
    if(input.empty()) {
        out_min = 0;
        out_max = UINT64_MAX;
        return true;
    }
    fs_string str = input;
    bool is_min = true;
    bool is_exact = false;

    //检查前缀
    if(str[0] == FSLENS_TEXT('+')) {
        is_min = true;
        str.erase(0, 1);
    } else if(str[0] == FSLENS_TEXT('-')) {
        is_min = false;
        str.erase(0, 1);
    } else {
        is_exact = true;//没有前缀，表示精确匹配
    }

    //解析数字和后缀
    double num = 0;
    int suffix_multiplier = 1; //默认单位为字节
    size_t i = 0;
    while (i < str.size() && (std::isdigit(str[i]) || str[i] == FSLENS_TEXT('.'))) {
        i++;
    }

    //转换数字部分
    fs_string num_part = str.substr(0, i);
    #ifdef _WIN32
        num = std::stod(num_part);
    #else
        num = std::stod(num_part);
    #endif

    //解析后缀
    fs_string suffix = str.substr(i);
    if (suffix == FSLENS_TEXT("B") || suffix.empty()) {
        suffix_multiplier = 1;
    } else if (suffix == FSLENS_TEXT("K") || suffix == FSLENS_TEXT("k")) {
        suffix_multiplier = 1024;
    } else if (suffix == FSLENS_TEXT("M") || suffix == FSLENS_TEXT("m")) {
        suffix_multiplier = 1024 * 1024;
    } else if (suffix == FSLENS_TEXT("G") || suffix == FSLENS_TEXT("g")) {
        suffix_multiplier = 1024 * 1024 * 1024;
    } else {
        return false; //不支持的 suffix
    }

    uint64_t bytes = static_cast<uint64_t>(num * suffix_multiplier);

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

struct Options {
    fs_string search_path = FSLENS_TEXT(".");
    fs_string name_pattern = FSLENS_TEXT("");
    bool recursive = false;
    bool show_help = false;
    bool show_version = false;
    fs_string size_filter = FSLENS_TEXT("");
    uint64_t size_min = 0;
    uint64_t size_max = UINT64_MAX;
};

inline Options parse_options(int argc, wchar_t* argv[]) {
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
                if (!parse_size_filter(opts.size_filter, opts.size_max)) {
                    std::cerr << "错误: 无效的大小过滤参数 '" << opts.size_filter << "'" << std::endl;
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
            L"  -s, --size <大小>    文件大小过滤 (如: +10M, -500K, 1.5G)\n",
            L"                       支持 B, K, M, G 后缀\n",
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
    std::cout << "  -s, --size <大小>    文件大小过滤 (如: +10M, -500K, 1.5G)" << std::endl;
    std::cout << "                       支持 B, K, M, G 后缀" << std::endl;
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
    std::wcout << FSLENS_VERSION << std::endl;
}