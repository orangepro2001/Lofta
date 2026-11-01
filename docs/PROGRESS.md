# 进度

接手先读这里。最后更新：2026-11-01（第三轮红队修订中途，为迁机收敛到稳定基线）。

## 闸门

用户规定：框架和剧情都完成、红队测试通过之后，才进入正式开发。

| 条件 | 状态 |
| --- | --- |
| 框架，最小无头测试，egg 编译 | 通过 |
| relhub 对接 | 通过，线上首发待 Defender 问题解决 |
| 史料考证 | 完成 |
| 剧情大纲（第三版） | 完成，经两轮红队 |
| 章节脚本和全部对白 | 七章交稿，合计约 11.4 万字 |
| 机器求解 | 通过，见下 |
| 第三轮红队 | 致命 5 / 严重 31 已改完；一般项约完成一半 |
| 正式开发 | **未开始**，等闸门 |

## 当前稳定基线

```
python3 tools/storyc.py content build/gen/content_data.lua
  → 473 nodes, 5805 lines, 113850 chars, 620 options, 130 cards, 47 recipes

python3 tools/story_check.py
  → 20000 runs, 65536 finale combinations, 8 endings all reachable, 4 problems
```

那 4 条问题全是 `demo.toml` 的占位配方（`i_demo_stub`、`r_demo_stub` 不可达 + 机器人没走到），与七章无关，改动前后一模一样。换句话说：**编译和求解都是干净基线**，随时可以接着改。

卡与配方都在预算上限内（新卡不含谬误卡）：

| 章 | 新卡 | 上限 | 配方 | 上限 |
| --- | --- | --- | --- | --- |
| 1 | 18 | 18 | 7 | 8 |
| 2 | 18 | 18 | 8 | 8 |
| 3 | 20 | 20 | 6 | 9 |
| 4 | 12 | 12 | 4 | 5 |
| 5 | 14 | 14 | 5 | 7 |
| 6 | 16 | 16 | 9 | 9 |
| 7 | 8 | 8 | 4 | 4 |

第一、二章的卡、第六、七章的配方已经顶到上限。再加东西就必须先合并或删。

## 第三轮红队

报告：`docs/story/redteam/R3-continuity.md`（致命 2、严重 13、一般 50）、`R3-logic.md`（致命 3、严重 18、一般 10）。
处理表：`docs/story/redteam/R3-response.md`，含撞车条目怎么取舍，以及剩余每一项的具体改法。

剩余工作集中在一般项，全是“改一两句”的活，不动主线。下一件事按 `R3-response.md` 第四节的表往下走即可。

## 迁机注意

**这个目录不是 git 仓库**（`git status` 报 not a git repository）。所有状态都在文件里，没有提交可以回滚，迁机就是整棵树拷过去。

- 要拷：`content/`、`docs/`、`scripts/`、`src/`、`tests/`、`third_party/`、`tools/`、`Makefile`、`VERSION`、`README.md`、`CLAUDE.md`、`.gitignore`
- 不用拷：`build/`（19 MB 产物，含 `build/gen/content_data.lua`，会在新机重新生成）、`tools/__pycache__/`
- 编译全在 egg 上：`tools/egg.sh test` / `shots` / `win`。新机要先把 `tools/egg.sh` 里的主机和路径改过来，代码本身不依赖本机路径。
- 本次会话改过的文件：`content/` 下的 `cards/ch1..ch7.toml`、`cast/ch4.toml`、`cast/ch5.toml`、`dialogue/ch1/main.dlg`、`ch2/{bowell,town}.dlg`、`ch3/{law,register,tuck}.dlg`、`ch4/{day,fourteenth,night}.dlg`、`ch5/{act1,act2,act3,term}.dlg`、`ch6/{act1,act2,act3}.dlg`、`ch7/{coda,endings,finale,main}.dlg`；`scripts/cards.lua`、`scripts/screens/board.lua`；`tools/{storyc,story_check}.py`；`docs/story/redteam/R3-response.md`、本文件。

## 下一步

1. 按 `R3-response.md` 第四节把剩余一般项改完，每批之后跑 `storyc.py` + `story_check.py`。
2. 同步 `02-truth.md` 和 `04-systems.md`（`R3-response.md` 第五节）。
3. 一般项清零后，跑 `tools/egg.sh test`、`tools/egg.sh win`、`win_check.py crosscheck`，再导 PNG 自查。
4. 全部通过后，第三轮红队收尾，闸门的“剧情”一项才算过。
