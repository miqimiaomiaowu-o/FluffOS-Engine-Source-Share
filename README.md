# Sharing the Source Code of the FluffOS Engine
## 项目简介
本仓库分享 FluffOS LPC 虚拟引擎完整源码，基于 BSD 3-Clause 开源协议，专为文字MUD、回合制网游服务端二次开发打造。
FluffOS 使用 C++ 底层驱动 + LPC 脚本逻辑分层架构，原生支持 MySQL、WebSocket、加密组件、异常捕获防护，适配 Linux 服务端部署。

## 核心特性
1. 底层内核：C++ 跨平台驱动，内置 try-catch 全局异常捕获，避免战斗/数值逻辑崩溃
2. 脚本语言：LPC 解释器，热更新脚本无需重启服务进程
3. 内置组件
   - MySQL 数据库原生绑定
   - WebSocket 网页客户端通讯
   - OpenSSL 加密、ECC 密钥校验工具
   - Jemalloc 内存优化，高并发在线稳定
4. 配套游戏框架：附带回合制游戏基础LIB（道具、战斗、NPC、灵魄、仙器等逻辑脚本）
5. 编译支持：CMake 一键编译，Ubuntu / CentOS 通用编译脚本

## 环境依赖
### Linux (Ubuntu 22.04 / CentOS 7)
```bash
sudo apt install cmake gcc g++ libjemalloc-dev libicu-dev libmysqlclient-dev libssl-dev libpcre3-dev libevent-dev bison

# 1. 拉取源码
git clone shturl.cc/LKFaLPKW8pTpHBYqQ5p5E40FlCxOH
cd fluffos-share

# 2. 创建编译目录
mkdir build && cd build

# 3. CMake 配置（开启数据库、加密、WebSocket）
cmake .. -DPACKAGE_DB=ON -DPACKAGE_CRYPTO=ON -DPACKAGE_WEBSOCKET=ON

# 4. 编译
make -j$(nproc)

编译完成后二进制文件输出至 ./bin/driver
许可证说明
本项目底层 FluffOS 内核采用 BSD 3 条款 "修订" 许可证
✅ 允许商用、闭源二次开发，无 GPL 传染约束
✅ 可自由修改、分发底层源码与上层 LPC 游戏脚本
⚠️ 修改分发时需保留原始版权声明，禁止使用原作者名义做产品宣传


fluffos-share/
├─ driver/        # C++底层驱动内核
├─ src/            # LPC标准内置函数efun
├─ arch/           # 跨平台系统适配层
├─ cmake/          # 编译配置脚本
├─ game_lib/       # 回合制游戏配套LPC逻辑（道具/战斗/宠物/仙器）
├─ single/tests/   # LPC功能测试用例
└─ README.md       # 项目自述文件

适配场景
文字 MUD 文字游戏开发
问道类回合制网游服务端二次开发
自定义 LPC 脚本服务、后台 GM 管理系统
plaintext

---
## 极简短版（适合小型仓库，简洁清爽）
```markdown
# FluffOS Engine Source Code Sharing
FluffOS LPC virtual engine full source code repository, based on BSD 3-Clause License.

### Main Features
- C++ high-performance driver with global try-catch exception protection
- Native MySQL, WebSocket & OpenSSL support
- Complete turn-based game LPC logic library included
- Cross-platform compile by CMake, support Linux server deployment

### Compile Command
```bash
mkdir build && cd build
cmake .. -DPACKAGE_DB=ON -DPACKAGE_CRYPTO=ON
make -j$(nproc)
License: BSD 3-Clause
plaintext
