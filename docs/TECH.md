# 技术方案

架构是 **C 核心 + 内嵌 Lua 5.4 + raylib 宿主**，沿用 survivalv2 验证过的做法。核心是软件绘制的索引色帧缓冲，raylib 只负责窗口、输入和把帧放大到屏幕，所以无头版和发布版跑同一份 C 和 Lua，画面哈希可以跨平台比较。

## 分层

| 层 | 内容 | 位置 |
| --- | --- | --- |
| C 核心 | 等距投影、场景网格、深度排序、寻路、角色移动、绘图、中文排版、调色板、随机数 | `src/core/` |
| 宿主 | 窗口、输入、呈现、存档文件、Lua 绑定 | `src/platform/` |
| 更新 | 协议、验签、下载、自替换 | `src/update/` |
| Lua | 屏幕栈、对话机、线索合成、叙事状态、存档编排 | `scripts/` |
| 内容 | 对白、线索、配方、地图 | `content/`，编译为 `build/gen/content_data.lua` |

规则放 C 的只有高频且必须确定的部分（移动、寻路、绘制）。叙事规则变化多，放 Lua，但状态只有字符串和整数，条件和效果都是编译后的数据。

## 画面

- 逻辑分辨率 640×360，每像素 1 字节的索引色。1080p 放大 3 倍，1440p 放大 4 倍，4K 放大 6 倍。选 640×360 而不是更低，是因为中文对白需要一行放得下约 50 字，12px 字体在这个分辨率上刚好。
- 调色板约 95 色，分界面固定色和世界色。昼夜靠每帧重算世界色，不改像素；灯火单独成段，天黑后保持亮度。
- 等距投影 2:1，格子 32×16。世界是方格，只有绘制时投影。
- 绘制分两遍：先铺全部地面，再按 `x+y` 对角线从远到近画墙、物件和人物。人物站在两格之间时不会被后画的地面切掉。
- 墙有高度（每级 8 像素）。走进一个区域（`region`）时，该区域的墙被切到 1 级，屋子敞开露出室内。这是 2.5D 的关键手感，放在核心里而不是脚本里。
- 文字用 Fusion Pixel 12px（OFL）。`tools/gen_font.py` 只抽取脚本和内容实际用到的字形，字库里没有的字会让构建失败。

## 确定性

- 核心状态是一个没有指针的结构体 `CoreState`，存档就是拷贝它，哈希就是哈希它。
- 叙事状态 `story.s` 只有字符串和整数，`scripts/codec.lua` 按排序后的键写出，相同状态得到相同字节。沙箱去掉了 `load`，存档不可能执行代码。
- `tests/scripts/determinism.lua` 跑固定输入，`tools/win_check.py crosscheck` 比较 Linux 和真实 Windows 的状态哈希和帧哈希。

## 内容管线

```
content/dialogue/**/*.dlg ─┐
content/cards/*.toml       ├─ tools/storyc.py ─> build/gen/content_data.lua ─> 内嵌进程序
content/maps/*.toml        ┘                      └─> tools/gen_font.py 抽字形
```

`.dlg` 的语法和检查规则见 `docs/story/README.md`。对话编译成带跳转的扁平操作表，所以对话进度就是“节点 + 程序计数器”，能直接存档。

## 无头测试

`build/lofta_headless <脚本> <截图目录>` 用真实入口跑 Lua 场景脚本，开 ASan 和 UBSan。脚本通过 `tests/lib/testlib.lua` 的 `tap`、`key`、`ticks` 操作，和玩家用的是同一批入口，按钮只有画出来才能点到。

## 编译环境

| 位置 | 用途 |
| --- | --- |
| egg `~/dev/game-dev/Lofta` | 全部编译和 Linux 测试，gcc 13、mingw-w64 13 |
| 本机 WSL | 写代码、同步、运行真实 Windows 程序 |
| AI | relhub 服务和发布令牌 |

raylib 的静态库由 `tools/build_raylib.sh` 在 egg 上用官方 6.2 源码编译，因为同机项目的预编译库和 egg 的 mingw 不兼容（缺 `stat64i32`）。依赖来源见 `third_party/PROVENANCE.md`。

## 已知问题

**Windows Defender 误报。** `Lofta.exe` 会被判成 `Program:Win32/Contebrew.A!ml` 并隔离（survivalv2 的 `snow.exe` 也遇到过同样的判定）。它是机器学习启发式，同样的源码换一次构建结果就可能不同：加上图标和版本信息后的 dev 版能跑，带版本号的版本和后续重编的版本又被拦。没有动 Defender 的设置，也不去试探它的判定。`lofta_headless.exe` 和 `update_client.exe` 不受影响，跨平台和更新验证用它们。详情和待决定事项见 `docs/RELEASE.md`。
