#include "scanner.hpp"
#include "platform/platform.hpp"
#include <vector>

static bool match_pattern(const fs_string& name, const fs_string& pattern) {
    if (pattern.empty()) return true;
    return name.find(pattern) != fs_string::npos;
}

void scan_directory(
    const fs_string& path,
    const fs_string& pattern,
    bool recursive,
    uint64_t min_size,
    uint64_t max_size,
    std::vector<FileEntry>& results
) {
    std::vector<FileEntry> entries;
    if (!traverse_directory(path, entries)) {
        return;
    }

    for (const auto& entry : entries) {
        if (entry.is_directory && recursive) {
            scan_directory(entry.path, pattern, recursive, min_size, max_size, results);
        }
        if (!entry.is_directory && match_pattern(entry.name, pattern)) {
            if (entry.size >= min_size && entry.size <= max_size) {
                results.push_back(entry);
            }
        }
    }
}