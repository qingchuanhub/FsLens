#pragma once
#include <string>
#include <vector>
#include "platform.hpp"

namespace fslens {

struct RunOptions {
    fs_string search_path;
    fs_string name_pattern;
    bool recursive{false};
    bool show_help{false};
};

// 解析命令行参数
RunOptions parse_args(int argc, fs_char** argv);
// 打印帮助信息
void print_help();

}