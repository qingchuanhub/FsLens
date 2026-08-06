//src/scanner.hpp
#pragma once

#include "platform/platform.hpp"
#include <vector>

void scan_directory(
    const fs_string& path,
    const fs_string& pattern,
    bool recursive,
    std::vector<FileEntry>& results
);