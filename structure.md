FsLens项目结构说明
```plaintext
FsLens/
├── .vscode/              # VSCode配置（上面所有json）
├── src/
│   ├── main.cpp          # 程序入口
│   ├── file_scanner.hpp  # 文件检索核心
│   ├── cmd_parser.hpp    # 命令行参数解析（兼容CMD参数风格）
│   └── utils.hpp
├── bin/                  # 编译输出 FsLens.exe
├── build/                # CMake临时构建目录（加入.gitignore）
├── CMakeLists.txt
├── README.md             # Github仓库介绍文档
└── .gitignore