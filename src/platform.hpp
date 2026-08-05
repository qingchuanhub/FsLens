#pragma once
#include <string>

#ifdef _WIN32
#define FSLENS_WINDOWS
#include <windows.h>
// Windows 宽字符兼容
using fs_char = wchar_t;
using fs_string = std::wstring;
#define FS_TEXT(x) L##x
#else
#define FSLENS_LINUX
using fs_char = char;
using fs_string = std::string;
#define FS_TEXT(x) x
#endif

namespace fslens {
// 简易字符串转换函数，Windows wstring <-> string
#ifdef FSLENS_WINDOWS
std::wstring to_wstring(const std::string& utf8);
std::string to_utf8(const std::wstring& wstr);
#endif
}