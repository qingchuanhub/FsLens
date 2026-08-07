# FsLens — 轻量级跨平台命令行文件检索工具

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS-lightgrey)]()

**FsLens** 是一款使用 C++17 编写的**命令行文件检索工具**，专注于快速、灵活的文件名搜索。它原生支持 Windows（Unicode 路径）和 macOS（UTF-8），通过抽象层实现了跨平台。

> 🚀 当前版本：**Beta 1.0.0** – 基础功能稳定，持续迭代中。

---

## ✨ 特性

- **双平台支持**：Windows（`win32.cpp`）+ macOS（`posix.cpp`），一套代码两处运行。
- **完美 Unicode 支持**：Windows 使用宽字符（`wchar_t`），macOS 使用 UTF-8，中文/日文/特殊字符无乱码。
- **灵活的命令行选项**：支持指定搜索目录、文件名包含匹配、递归子目录搜索。
- **轻量快速**：无外部依赖，静态编译生成单个可执行文件，体积小巧。
- **模块化架构**：平台层抽象隔离，便于扩展。

---

## 📦 快速开始

### 前置条件
- **编译器**：MinGW-w64（Windows）或 Clang（macOS，Xcode Command Line Tools）
- **构建工具**：CMake（≥3.10）或直接使用编译器命令行

### 获取源码
```bash
git clone https://github.com/yourusername/FsLens.git
cd FsLens
```

### 使用 CMake 构建（推荐，跨平台统一）
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"   # Windows
# 或 cmake .. -G "Unix Makefiles"  # macOS
make
```

### 各平台手动编译

#### Windows（MinGW-w64）
```bash
g++ -std=c++17 -static-libgcc -static-libstdc++ -municode -DUNICODE -D_UNICODE -Isrc src/FsLens.cpp src/scanner.cpp src/platform/win32.cpp -o FsLens.exe
```

#### macOS（clang++）
```bash
clang++ -std=c++17 -Isrc src/FsLens.cpp src/scanner.cpp src/platform/posix.cpp -o FsLens
```

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
| `-n, --name <模式>` | 文件名包含匹配（例如 `main` 匹配 `main.cpp`） |
| `-r, --recursive`   | 递归搜索所有子目录 |
| `-v, --version`     | 显示版本号并退出 |
| `-s, --size <规则>` | 按文件大小过滤，支持 +10M（大于）、-500k（小于）、100M-500M（范围） |
| `-h, --help`        | 显示帮助信息 |

### 示例

1. **在当前目录搜索所有包含 `.txt` 的文件**：
   ```bash
   FsLens -n .txt
   ```

2. **在 `/Users/xxx/Projects` 中递归搜索含 `config` 的文件**（macOS）：
   ```bash
   FsLens -p /Users/xxx/Projects -n config -r
   ```

3. **查看版本**：
   ```bash
   FsLens -v
   # 输出: Beta 1.0.0
   ```
4. **按文件大小过滤**：
   - 查找大于 10MB 的文件：
     ```bash
     FsLens -s +10M
     ```
   - 查找小于 500KB 的文件：
     ```bash
     FsLens -s -500k
     ```
   - 查找大小在 100MB 到 500MB 之间的文件：
     ```bash
     FsLens -s 100M-500M
     ```

### 输出格式
- 每行显示一个匹配文件的**完整路径**。
- 最后一行输出匹配的文件总数。

---

## 📁 项目结构

```
FsLens/
├── .github/workflows/          # CI 自动构建（预留）
├── src/
│   ├── platform/
│   │   ├── platform.hpp        # 跨平台类型定义与接口声明
│   │   ├── win32.cpp           # Windows 目录遍历实现
│   │   └── posix.cpp           # macOS / POSIX 目录遍历实现
│   ├── cli/
│   │   └── cmdopt.hpp          # 命令行参数解析器（含 -v 版本查询）
│   ├── scanner.hpp             # 文件扫描器接口
│   ├── scanner.cpp             # 递归扫描 + 名称匹配逻辑
│   └── FsLens.cpp              # 程序入口（Windows 用 wmain，macOS 用 main）
├── tests/                      # 单元测试（预留）
├── CMakeLists.txt              # CMake 构建配置（自动选择平台源文件）
├── .gitignore
├── LICENSE                     # MIT 许可证
└── README.md                   # 本文件
```

---

## 🧩 核心设计

### 平台抽象层
- 通过 `FileEntry` 结构体统一文件元数据。
- `traverse_directory` 函数提供跨平台目录遍历接口。
- Windows 使用 `FindFirstFileW` / `FindNextFileW`，macOS 使用 `opendir` / `readdir`。

### 命令行解析
- `cmdopt.hpp` 实现轻量级参数解析，支持短选项和长选项。
- 使用 `FSLENS_TEXT` 宏统一宽字符（Windows）和窄字符（macOS）字面量。

### 版本管理
- 版本号通过 `FSLENS_VERSION` 宏集中定义（`Beta 1.0.0`）。
- `-v` / `--version` 选项独立输出版本信息并退出。

---

## 🔧 扩展开发

### 移植到 Linux（目前“计划中”）
- 理论可行：`posix.cpp` 在 Linux 上同样可以编译。
- 官方暂不提供 Linux 二进制包，欢迎社区贡献。

### 新增过滤条件（如按文件大小）
1. 在 `Options` 结构体中增加字段。
2. 在 `scan_directory` 中添加过滤逻辑。
3. 更新命令行解析和帮助信息。

### 支持正则表达式
- 引入 `<regex>` 标准库，在 `match_pattern` 中使用 `std::regex_search`。

---

## 📜 许可证

本项目采用 [MIT 许可证](LICENSE)，允许自由使用、修改和分发。

---

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！开发前请确保：
1. 代码符合 C++17 标准，编译无警告。
2. 新功能附带相应测试（如可能）。
3. 提交信息清晰明了。

---

## 📧 联系方式

- 作者：Your Name
- 邮箱：your.email@example.com
- 项目主页：[GitHub 链接](https://github.com/yourusername/FsLens)

---

> Made with ❤️ and C++17.
