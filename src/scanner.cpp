//src/scanner.cpp
#include "scanner.hpp"
#include "platform/platform.hpp"
#include <vector>

// 判断文件名是否匹配（当前为包含匹配，例如输入 "main" 匹配 "main.cpp"）
static bool match_pattern(const fs_string& name, const fs_string& pattern) {
    if (pattern.empty()) return true;
    return name.find(pattern) != fs_string::npos;
}

void scan_directory(
    const fs_string& path,
    const fs_string& pattern,
    bool recursive,
    std::vector<FileEntry>& results
) {
    std::vector<FileEntry> entries;
    if (!traverse_directory(path, entries)) {
        return;
    }

    for (const auto& entry : entries) {
        // 如果是目录且允许递归，则深入扫描
        if (entry.is_directory && recursive) {
            scan_directory(entry.path, pattern, recursive, results);
        }

        // 如果是文件（或我们想同时列目录），进行名称匹配
        // 这里仅匹配非目录，如果你想显示目录，可以去掉 !entry.is_directory 条件
        if (!entry.is_directory && match_pattern(entry.name, pattern)) {
            results.push_back(entry);
        }
    }
}