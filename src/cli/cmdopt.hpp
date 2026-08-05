#pragma once

#include "../platform/platform.hpp"
#include <cstring>
#include <iostream>

struct Options {
    fs_string search_path = L".";   // 默认当前目录
    fs_string name_pattern = L"";   // 空表示匹配所有
    bool recursive = false;
    bool show_help = false;
};

inline Options parse_options(int argc, wchar_t* argv[]) {
    Options opts;

    for (int i = 1; i < argc; ++i) {
        fs_string arg = argv[i];

        if (arg == L"-h" || arg == L"--help") {
            opts.show_help = true;
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
    std::wcout << L"FsLens - 跨平台文件检索工具 (v1.0)" << std::endl;
    std::wcout << L"用法: FsLens [选项]" << std::endl;
    std::wcout << L"  -p, --path <路径>    指定搜索目录 (默认: .)" << std::endl;
    std::wcout << L"  -n, --name <模式>    文件名包含匹配 (如: main, .txt)" << std::endl;
    std::wcout << L"  -r, --recursive      递归搜索子目录" << std::endl;
    std::wcout << L"  -h, --help           显示此帮助信息" << std::endl;
}