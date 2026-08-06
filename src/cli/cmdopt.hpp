#pragma once

#include "../platform/platform.hpp"
#include <cstring>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

#define FSLENS_VERSION L"Beta 1.0.0"

struct Options {
    fs_string search_path = L".";   // 默认当前目录
    fs_string name_pattern = L"";   // 空表示匹配所有
    bool recursive = false;
    bool show_help = false;
    bool show_version = false;
};

inline Options parse_options(int argc, wchar_t* argv[]) {
    Options opts;

    for (int i = 1; i < argc; ++i) {
        fs_string arg = argv[i];

        if (arg == L"-h" || arg == L"--help") {
            opts.show_help = true;
            return opts;
        }
        else if (arg == L"-v" || arg == L"--version") {
            opts.show_version = true;
            return opts;
        }
        else if (arg == L"-p" || arg == L"--path") {
            if (i + 1 < argc) {
                opts.search_path = argv[++i];
            }
        }
        else if (arg == L"-n" || arg == L"--name") {
            if (i + 1 < argc) {
                opts.name_pattern = argv[++i];
            }
        }
        else if (arg == L"-r" || arg == L"--recursive") {
            opts.recursive = true;
        }
        else {
            // 未识别的参数，当作路径处理（简单兼容）
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
            L"FsLens - 跨平台文件检索工具 (v1.0)\n",
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
    // 回退到标准输出（非Windows或控制台不可用时）
    std::wcout << L"FsLens - 跨平台文件检索工具 (v1.0)" << std::endl;
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
        const wchar_t* msg = L"FsLens Beta 1.0.0\n";
        WriteConsoleW(hConsole, msg, wcslen(msg), &written, NULL);
        return;
    }
#endif
    std::wcout << L"FsLens Beta 1.0.0" << std::endl;
}