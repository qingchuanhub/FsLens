# FsLens — 轻量级跨平台文件检索工具

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)]()

FsLens 是一款使用 **C++17** 开发的命令行文件检索工具，支持**模糊匹配**、**正则表达式**（预留）、**大小/时间过滤**（预留）和**递归搜索**。项目优先适配 Windows CMD，同时通过抽象层保留了对 Linux/macOS 的扩展能力。

> 🚀 项目状态：基础功能已完成(Windows CMD&PowerShell)，持续迭代中。

---

## ✨ 特性

- **跨平台设计**：核心逻辑与平台 API 隔离，目前已实现 Windows（`win32.cpp`），Linux/macOS 可轻松接入。
- **Unicode 完美支持**：使用宽字符（`wchar_t`）处理路径和文件名，中文无乱码。
- **命令行交互**：支持 `-p`（路径）、`-n`（名称模式）、`-r`（递归）等选项，简洁高效。
- **可扩展架构**：模块化代码，便于添加新过滤器（大小、时间、正则等）。
- **图形界面（可选）**：提供基于 Win32 API 的简洁 GUI 版本（`FsLensGUI.exe`），操作更直观。

---

## 📦 快速开始

### 前置条件
- **编译器**：MinGW-w64（`g++` 支持 C++17）或 MSVC
- **构建工具**：CMake（推荐）或直接使用 Makefile
- **（可选）** 若要构建 GUI 版本，需链接 `comctl32` 和 `shell32`（MinGW 默认已包含）

### 获取源码
```bash
git clone https://github.com/yourusername/FsLens.git
cd FsLens
```

### 使用 CMake 构建（推荐）
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"   # Windows
# 或 cmake .. -G "Unix Makefiles"  # Linux/macOS
make
```
构建完成后，可执行文件位于 `build/` 目录下。

### 直接使用 g++ 编译（命令行版）
```bash
g++ -std=c++17 -static-libgcc -static-libstdc++ -municode -DUNICODE -D_UNICODE -Isrc src/FsLens.cpp src/scanner.cpp src/platform/win32.cpp -o FsLens.exe
```

### 编译 GUI 版本（Windows）
```bash
g++ -std=c++17 -static-libgcc -static-libstdc++ -mwindows -municode -DUNICODE -D_UNICODE -Isrc src/FsLensGUI.cpp src/scanner.cpp src/platform/win32.cpp -o FsLensGUI.exe
```

---

## 🖥️ 使用方法

### 命令行版
```bash
FsLens.exe [选项]
```
| 选项 | 说明 |
|------|------|
| `-p, --path <目录>` | 指定搜索目录（默认当前目录 `.`） |
| `-n, --name <模式>` | 文件名包含匹配（如 `main`、`.txt`） |
| `-r, --recursive`   | 递归搜索所有子目录 |
| `-h, --help`        | 显示帮助信息 |

**示例**：
```bash
FsLens -p C:\Projects -n .cpp -r
```
在 `C:\Projects` 及其子目录中搜索所有包含 `.cpp` 的文件。

### GUI 版
双击 `FsLensGUI.exe` 打开窗口：
- 在“搜索目录”输入框键入路径，或点击“浏览”按钮选择。
- 在“名称模式”输入文件名关键字（留空则匹配全部）。
- 勾选“递归子目录”以深入搜索。
- 点击“搜索”按钮，结果将以表格形式展示（文件名、路径、大小、修改时间）。
- 底部状态栏显示匹配的文件总数。

---

## 📁 项目结构

```
FsLens/
├── .github/workflows/          # CI 自动构建（预留）
├── src/
│   ├── platform/
│   │   ├── platform.hpp        # 跨平台类型定义
│   │   ├── win32.cpp           # Windows 目录遍历实现
│   │   └── posix.cpp           # (预留) Linux/macOS 实现
│   ├── cli/
│   │   └── cmdopt.hpp          # 命令行参数解析
│   ├── scanner.hpp             # 文件扫描器接口
│   ├── scanner.cpp             # 递归扫描 + 名称匹配
│   ├── FsLens.cpp              # 命令行入口 (wmain)
│   └── FsLensGUI.cpp           # GUI 入口 (WinMain)
├── tests/                      # 单元测试（预留）
├── CMakeLists.txt              # CMake 构建配置
├── .gitignore
├── LICENSE                     # MIT 许可证
└── README.md                   # 本文件
```

---

## 🧩 核心设计

### 平台抽象层
- 通过 `FileEntry` 结构体统一文件元数据。
- `traverse_directory` 函数提供跨平台目录遍历接口。
- 业务逻辑（`scanner.cpp`）完全与平台无关。

### 命令行解析
- `cmdopt.hpp` 实现轻量级参数解析，支持短选项（`-p`）和长选项（`--path`）。
- 默认值友好，未指定路径时使用当前目录。

### 搜索算法
- 递归遍历目录树（受 `-r` 控制）。
- 当前使用**包含匹配**（子串查找），未来可扩展为正则表达式或通配符。

---

## 🔧 扩展开发

### 添加新的过滤条件
修改 `scanner.cpp` 中的 `match_pattern` 函数，或新增过滤函数（如按大小、时间），在 `scan_directory` 中调用即可。

### 支持 Linux/macOS
实现 `src/platform/posix.cpp`，使用 `opendir`/`readdir` 等 POSIX API，并确保 `platform.hpp` 中 `FSLENS_POSIX` 分支正确编译。

### 增加 GUI 跨平台能力
可替换为 **Qt** 或 **FLTK** 等跨平台框架，当前 Win32 实现可作为原型参考。

---

## 📜 许可证

本项目采用 [MIT 许可证](LICENSE)，允许自由使用、修改和分发。

---

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！在开发前请确保：
1. 代码符合 C++17 标准，无警告。
2. 新功能附带相应测试（如有）。
3. 提交信息清晰明了。

---

## 📧 联系方式

- 作者：qingchuanhub & Greenchannel
- 邮箱：qingchuanyj@agent.qq.com
- 项目主页：[GitHub 链接](https://github.com/qingchuanhub/FsLens)

---

> Made with ❤️ and C++17.

---
