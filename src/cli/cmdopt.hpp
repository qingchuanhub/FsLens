// src/cli/cmdopt.hpp
#pragma once

#include "../platform/platform.hpp"
#include <cstring>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
    #define FSLENS_TEXT(x) L##x
#else
    #define FSLENS_TEXT(x) x
#endif

#define FSLENS_VERSION "Beta 1.0.0"

struct Options {
    fs_string search_path = FSLENS_TEXT(".");
    fs_string name_pattern = FSLENS_TEXT("");
    bool recursive = false;
    bool show_help = false;
    bool show_version = false;
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
    std::wcout << L"FsLens - 跨平台文件检索工具 (" << FSLENS_VERSION << ")" << std::endl;
    std::wcout << L"用法: FsLens [选项]" << std::endl;
    std::wcout << L"  -p, --path <路径>    指定搜索目录 (默认: .)" << std::endl;
    std::wcout << L"  -n, --name <模式>    文件名包含匹配 (如: main, .txt)" << std::endl;
    std::wcout << L"  -r, --recursive      递归搜索子目录" << std::endl;
    std::wcout << L"  -v, --version        显示版本信息" << std::endl;
    std::wcout << L"  -h, --help           显示此帮助信息" << std::endl;
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