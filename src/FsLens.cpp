// src/FsLens.cpp
#include "cli/cmdopt.hpp"
#include "scanner.hpp"
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#else
int main(int argc, char* argv[]) {
#endif

    Options opts = parse_options(argc, argv);

    if (opts.show_help) {
        print_help();
        return 0;
    }

    if (opts.show_version) {
        print_version();
        return 0;
    }

    std::vector<FileEntry> results;
    scan_directory(
        opts.search_path,
        opts.name_pattern,
        opts.recursive,
        opts.min_size,
        opts.max_size,
        results
    );

    for (const auto& entry : results) {
#ifdef _WIN32
        std::wcout << entry.path << std::endl;
#else
        std::cout << entry.path << std::endl;
#endif
    }

#ifdef _WIN32
    std::wcout << L"\n总计找到 " << results.size() << L" 个匹配文件。" << std::endl;
#else
    std::cout << "\n总计找到 " << results.size() << " 个匹配文件。" << std::endl;
#endif

    return 0;
}