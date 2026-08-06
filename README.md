# FsLens — 轻量级跨平台命令行文件检索工具

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)]()

**FsLens** 是一款使用 C++17 编写的**命令行文件检索工具**，专注于快速、灵活的文件名搜索。它原生支持 Windows（Unicode 路径），并通过抽象层预留了对 Linux/macOS 的扩展能力。

> 🚀 当前版本：Beta v1.0.0 – 基础功能稳定，持续迭代中。

---

## 平台支持
- ✅ Windows
- ✅ macOS
- 🚧 Linux（开发中，欢迎贡献）

## ✨ 特性

- **跨平台设计**：核心逻辑与平台 API 隔离，目前已提供 Windows 实现（`win32.cpp`），未来可轻松移植到 POSIX 系统。
- **完美 Unicode 支持**：使用宽字符（`wchar_t`）处理路径和文件名，中文、日文、特殊字符无乱码。
- **灵活的命令行选项**：支持指定搜索目录、文件名包含匹配、递归子目录搜索。
- **轻量快速**：无外部依赖，静态编译生成单个 `.exe`，体积小巧，启动迅速。
- **模块化架构**：代码组织清晰，便于添加新功能（如正则表达式、大小/时间过滤）。

---

## 📦 快速开始

### 前置条件
- **编译器**：MinGW-w64（`g++` 支持 C++17）或 MSVC / Clang
- **构建工具**：CMake（≥3.10）或直接使用 `g++` 命令行

### 获取源码
```bash
git clone https://github.com/qingchuanhub/FsLens.git
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

### 直接使用 g++ 编译（Windows 示例）
```bash
g++ -std=c++17 -static-libgcc -static-libstdc++ -municode -DUNICODE -D_UNICODE -Isrc src/FsLens.cpp src/scanner.cpp src/platform/win32.cpp -o FsLens.exe
```
> **说明**：`-municode` 选项用于启用宽字符入口点 `wmain`，这是正确处理中文路径的关键。

---

## 🖥️ 使用方法

### 命令格式
```bash
FsLens [选项]
```

### 选项说明
| 选项 | 说明 |
|------|------|
| `-p, --path <目录>` | 指定搜索的根目录（默认当前目录 `.`） |
| `-n, --name <模式>` | 文件名包含匹配（例如 `main` 会匹配 `main.cpp`、`main.h`） |
| `-r, --recursive`   | 递归搜索所有子目录 |
| `-h, --help`        | 显示帮助信息 |

### 示例

1. **在当前目录搜索所有包含 `.txt` 的文件**（不递归）：
   ```bash
   FsLens -n .txt
   ```

2. **在 `D:\Projects` 中递归搜索含 `config` 的文件**：
   ```bash
   FsLens -p D:\Projects -n config -r
   ```

3. **仅列出当前目录下所有文件（不递归，不过滤名称）**：
   ```bash
   FsLens -p .
   ```

4. **查看帮助**：
   ```bash
   FsLens -h
   ```

### 输出格式
- 每行显示一个匹配文件的**完整路径**。
- 最后一行输出匹配的文件总数，例如：  
  `总计找到 42 个匹配文件。`

---

## 📁 项目结构

```
FsLens/
├── .github/workflows/          # CI 自动构建（预留）
├── src/
│   ├── platform/
│   │   ├── platform.hpp        # 跨平台类型定义与接口声明
│   │   ├── win32.cpp           # Windows 目录遍历实现
│   │   └── posix.cpp           # (预留) Linux/macOS 实现
│   ├── cli/
│   │   └── cmdopt.hpp          # 命令行参数解析器
│   ├── scanner.hpp             # 文件扫描器接口
│   ├── scanner.cpp             # 递归扫描 + 名称匹配逻辑
│   └── FsLens.cpp              # 程序入口（wmain）
├── tests/                      # 单元测试（预留）
├── CMakeLists.txt              # CMake 构建配置
├── .gitignore
├── LICENSE                     # MIT 许可证
└── README.md                   # 本文件
```

---

## 🧩 核心设计

### 平台抽象层
- 通过 `FileEntry` 结构体统一文件元数据（路径、名称、大小、修改时间）。
- `traverse_directory` 函数提供跨平台目录遍历接口，业务代码无需关心底层 API。
- 使用宏 `FSLENS_WINDOWS` / `FSLENS_POSIX` 选择具体实现。

### 命令行解析
- `cmdopt.hpp` 实现轻量级参数解析，支持短选项（`-p`）和长选项（`--path`）。
- 解析结果存入 `Options` 结构体，默认值友好。

### 搜索算法
- 递归遍历目录树（由 `-r` 控制）。
- 当前采用**包含匹配**（子串查找），后续可扩展为正则表达式或通配符匹配。

---

## 🔧 扩展开发

### 添加新的过滤条件（如按文件大小）
1. 在 `Options` 结构体中增加相应字段（`cmdopt.hpp`）。
2. 在 `scan_directory` 中添加过滤逻辑（`scanner.cpp`）。
3. 更新命令行解析和帮助信息。

### 移植到 Linux/macOS
- 创建 `src/platform/posix.cpp`，使用 `opendir` / `readdir` 实现 `traverse_directory`。
- 在 `platform.hpp` 中启用 `FSLENS_POSIX` 分支，将 `fs_char` 定义为 `char`（UTF-8）。
- 修改 CMakeLists.txt 根据系统选择源文件。

### 支持正则表达式
- 引入 `<regex>` 标准库。
- 在 `match_pattern` 函数中使用 `std::regex_search` 进行匹配。
- 增加 `-e, --regex` 命令行开关以切换模式。

---

## 📜 许可证

本项目采用 [MIT 许可证](LICENSE)，允许自由使用、修改和分发。

---

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！开发前请确保：
1. 代码符合 C++17 标准，编译无警告。
2. 新功能附带相应测试（如有）。
3. 提交信息清晰明了，遵循约定式提交（如 `feat: add size filter`）。

---

## 📧 联系方式

- 作者：
  - qingchuanhub 
  - Greenchannel
- 邮箱：
  - qingchuanyj@agent.qq.com
  - cute486@agent.qq.com
- 项目主页：[GitHub 链接](https://github.com/qingchuanhub/FsLens)

---

> Made with ❤️ and C++17.