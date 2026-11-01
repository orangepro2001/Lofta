# 《遗失的十一日》（Lofta）

1752 年伦敦的等距 2D 推理游戏，目标平台 Windows，游戏语言中文。用来积累游戏开发经验。

接手先读 `docs/PROGRESS.md`。文档入口是 `docs/README.md`。

**闸门：** 框架、剧情三级设计和红队测试全部通过前，不开始正式开发。闸门状态写在 `docs/PROGRESS.md`。

## 命令

编译都在 egg 上做，本机只同步和取回产物。

```
tools/egg.sh test           # 层检查、脚本规则、单元测试、全部 Lua 场景，开 ASan 和 UBSan
tools/egg.sh shots          # 导出 PNG，取回到 build/egg/shots/
tools/egg.sh win            # 开发版 build/egg/dev/Lofta.exe（版本码 0，不检查更新）
tools/egg.sh build/lofta_headless.exe
python3 tools/win_check.py crosscheck      # 真实 Windows 跑全部场景，哈希必须和 Linux 一致
python3 tools/win_check.py smoke <exe>     # 真实窗口开在隐藏桌面上，不弹到用户屏幕
python3 tools/story_check.py               # 剧情静态检查与求解，见 docs/story/README.md
tools/release.sh --channel beta            # 测试、构建、发布，见 docs/RELEASE.md
```

不要在用户桌面上弹窗口：跑 Windows 图形版一律走 `win_check.py smoke`。

## 层边界

- `src/core/` 是 C 核心：世界、移动、寻路、绘制、文字。对外只有 `core_api.h`，其余头文件私有。
- `src/platform/` 是宿主：`lua_host`、`lua_bind`、`savefile` 共用，`raylib/` 和 `headless/` 各一份入口。
- `src/update/` 是更新：`relhub.c` 可移植，`update_win.c` 只在 Windows。
- `scripts/` 是 Lua：界面流程、对话机、线索合成、叙事状态。
- `content/` 是内容：对白 `.dlg`，线索和地图 `.toml`，由 `tools/storyc.py` 编译。
- 每次请求只改一个层，必须跨层时先说明原因。

## 代码约定

- C99，禁止 C++ 和 VLA，选项 `-std=c99 -Wall -Wextra -Werror -Wvla -O2`。
- 核心不用 `stdio.h`、`stdlib.h`、`time.h`，不 `malloc`，不用浮点，不调平台 API，全部 ASCII。`tools/check_layers.sh` 强制检查。
- 所有随机数走 `rng.c`，分世界、剧情、特效三条流。
- 新功能必须能只靠点击完成，键盘只是快捷方式。
- 绘制不改变状态。可存档的状态都在 `state.h` 的 `CoreState` 或 Lua 的 `story.s` 里，不存指针。
- 脚本不用 `math.random`、`os`、`io`、`load`；`pairs` 只用于随后排序的场合，行尾标 `-- sorted below`。
- 叙事状态只有字符串和整数，影响结果的计算只用整数。
- 所有玩家可见的剧情文字写在 `content/`，界面文字写在 `scripts/screens/`。
- 代码注释用英文，文档用中文。

## 完成标准

- 每次改动通过 `tools/egg.sh test` 和 `tools/egg.sh win`。
- 改内容后通过 `tools/story_check.py`。
- 涉及画面的改动要导出 PNG 并亲眼检查。
- 改 `src/core/`、`scripts/` 后跑 `win_check.py crosscheck`；改宿主后再跑 `win_check.py smoke`。
- 改 `src/update/` 后跑 `tools/relhub_e2e.py`。
- 每个里程碑开工前写 `docs/M<N>.md`，完工后写 `docs/handoff-M<N>.md`。
