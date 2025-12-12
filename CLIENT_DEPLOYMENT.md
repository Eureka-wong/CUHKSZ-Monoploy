# 客户端部署指南

## 如何创建可移植的客户端包

### 方法 1：使用打包脚本（推荐）

1. **在开发电脑上运行打包脚本**：
   ```powershell
   .\package_client.ps1
   ```

   这个脚本会：
   - 自动构建项目（如果需要）
   - 收集所有必要的 DLL 和依赖文件
   - 创建一个 `Client_Portable` 文件夹
   - 包含所有运行所需的文件

2. **打包完成后**：
   - 找到 `Client_Portable` 文件夹
   - 将这个文件夹压缩成 ZIP 文件
   - 或者直接复制整个文件夹到其他电脑

3. **在其他电脑上运行**：
   - 解压或复制 `Client_Portable` 文件夹
   - 双击 `CUHKSZ_Monopoly_Online.exe` 即可运行
   - 不需要安装 Qt、Visual Studio 或其他依赖

### 方法 2：手动打包

如果打包脚本不可用，可以手动操作：

1. **构建项目**：
   ```powershell
   .\run.ps1
   ```

2. **复制文件**：
   - 复制 `build\Windows-Release\Release` 文件夹中的所有内容
   - 包括：
     - `CUHKSZ_Monopoly_Online.exe`
     - 所有 `.dll` 文件
     - 所有子文件夹（`platforms`, `imageformats`, `translations` 等）

3. **验证依赖**（可选）：
   ```powershell
   cd "C:\Qt\6.10.1\msvc2022_64\bin"
   .\windeployqt.exe --release --compiler-runtime "路径\到\CUHKSZ_Monopoly_Online.exe"
   ```

## 文件结构

打包后的文件夹应该包含：

```
Client_Portable/
├── CUHKSZ_Monopoly_Online.exe  (主程序)
├── Qt6Core.dll                  (Qt 核心库)
├── Qt6Gui.dll                   (Qt GUI 库)
├── Qt6Widgets.dll               (Qt Widgets 库)
├── Qt6WebSockets.dll            (Qt WebSocket 库)
├── Qt6Network.dll               (Qt 网络库)
├── Qt6Svg.dll                   (Qt SVG 库)
├── platforms/                   (平台插件)
│   └── qwindows.dll
├── imageformats/                (图片格式插件)
│   ├── qgif.dll
│   ├── qico.dll
│   ├── qjpeg.dll
│   └── qsvg.dll
├── translations/                (翻译文件)
├── styles/                      (样式插件)
├── tls/                         (TLS 插件)
├── networkinformation/          (网络信息插件)
├── iconengines/                 (图标引擎)
├── generic/                     (通用插件)
├── README.txt                   (说明文件)
└── Run_Client.bat               (启动脚本)
```

## 系统要求

- **操作系统**：Windows 10 或更高版本
- **架构**：64 位 (x64)
- **网络**：需要能够连接到服务器（同一局域网或互联网）

## 常见问题

### Q: 程序无法启动，提示缺少 DLL
A: 确保所有 DLL 文件都在同一文件夹中，不要删除任何文件。

### Q: 无法连接到服务器
A: 
1. 检查服务器是否正在运行
2. 检查服务器 IP 地址是否正确
3. 检查防火墙设置
4. 确保客户端和服务器在同一网络

### Q: 程序启动后立即关闭
A: 
1. 检查是否有错误日志
2. 尝试在命令行中运行，查看错误信息
3. 确保所有依赖文件完整

### Q: 如何更新客户端
A: 只需替换 `CUHKSZ_Monopoly_Online.exe` 文件，其他 DLL 文件通常不需要更新。

## 分发建议

1. **压缩包**：将 `Client_Portable` 文件夹压缩成 ZIP 文件分发
2. **文件大小**：通常约 50-100 MB（取决于 Qt 版本）
3. **版本信息**：建议在文件夹名称中包含版本号，如 `Client_Portable_v1.0`

## 注意事项

- 不要修改文件夹结构
- 不要删除任何 DLL 文件
- 确保所有文件在同一文件夹中
- 如果使用杀毒软件，可能需要将整个文件夹添加到白名单


