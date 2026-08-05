#include "cli/cmdopt.hpp"
#include "scanner.hpp"
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

int wmain(int argc, wchar_t* argv[]) {
    // Windows 下设置控制台为 UTF-8 编码，防止中文乱码
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    Options opts = parse_options(argc, argv);

    if (opts.show_help) {
        print_help();
        return 0;
    }

    std::vector<FileEntry> results;
    scan_directory(opts.search_path, opts.name_pattern, opts.recursive, results);

    // 输出结果
    for (const auto& entry : results) {
        std::wcout << entry.path << std::endl;
    }

    // 简单的统计信息
    std::wcout << L"\n总计找到 " << results.size() << L" 个匹配文件。" << std::endl;

    return 0;
}