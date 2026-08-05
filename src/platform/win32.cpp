#include "platform/platform.hpp"
#include <windows.h>
#include <string>
#include <vector>

bool traverse_directory(const fs_string& path, std::vector<FileEntry>& out_entries) {
    fs_string search_path = path;
    if (!search_path.empty() && search_path.back() != L'\\' && search_path.back() != L'/') {
        search_path += L'\\';
    }
    search_path += L'*';

    WIN32_FIND_DATAW find_data;
    HANDLE hFind = FindFirstFileW(search_path.c_str(), &find_data);
    if (hFind == INVALID_HANDLE_VALUE) {
        return false; // 目录无法打开或为空
    }

    do {
        // 忽略 "." 和 ".."
        if (wcscmp(find_data.cFileName, L".") == 0 || wcscmp(find_data.cFileName, L"..") == 0) {
            continue;
        }

        FileEntry entry;
        // 构建完整路径
        entry.path = path;
        if (!entry.path.empty() && entry.path.back() != L'\\' && entry.path.back() != L'/') {
            entry.path += L'\\';
        }
        entry.path += find_data.cFileName;

        entry.name = find_data.cFileName;
        entry.is_directory = (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        // 计算文件大小
        entry.size = (static_cast<uint64_t>(find_data.nFileSizeHigh) << 32) + find_data.nFileSizeLow;

        // 转换时间 (FILETIME -> time_t)
        ULARGE_INTEGER uli;
        uli.LowPart = find_data.ftLastWriteTime.dwLowDateTime;
        uli.HighPart = find_data.ftLastWriteTime.dwHighDateTime;
        // 1601-01-01 到 1970-01-01 的偏移 (单位: 100纳秒)
        entry.modified_time = static_cast<time_t>((uli.QuadPart - 116444736000000000ULL) / 10000000ULL);

        out_entries.push_back(entry);
    } while (FindNextFileW(hFind, &find_data) != 0);

    FindClose(hFind);
    return true;
}