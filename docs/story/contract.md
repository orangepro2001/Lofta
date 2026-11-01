# 跨章接口

七章分头写，靠这份接口对齐。这里写的 ID 和标记一个字都不许改，新增只能在本章内部用。事实依据永远是 `02-truth.md`。

## 文件归属

每章只写自己的文件：

```
content/story/chN.toml          章节计划
content/cards/chN.toml          卡片和配方（配方 id 用 d/i/r/f 加章号开头：d3_x）
content/dialogue/chN/*.dlg      对白，节点名以 chN_ 开头
content/cast/chN.toml           本章新增的说话人和他们的“已死”条件
docs/story/chapters/chN.md      章节脚本
```

## 标记

本章设置，别的章读取。除非注明，标记只在本章设置一次。

| 标记 | 设置 | 含义 |
| --- | --- | --- |
| `ch1_bill_true` `ch1_took_groat` `ch1_found_dead` | 1 | 南那一行的写法 |
| `ch1_blamed_keane` | 1 | 诬告基恩太太 |
| `bowell_dead` | 2 | 鲍尔已死 |
| `bowell_soothed` | 2 | 听到“我没看他的手” |
| `hannah_stone_told` | 2 | 皮姆提过穷人坑边的石头 |
| `ch2_done` `ch3_done` `ch4_done` `ch5_done` `ch6_done` | 各章 | 章末设置，供“已死”条件用 |
| `kitty_alive` | 3 | 救下基蒂 |
| `register_note` | 3 | 抄到看守旁注 |
| `have_almanac` | 3 | 拿到《历书》 |
| `ned_alive` `lucy_alive` `will_alive` | 4 | 救下 |
| `gale_book` | 4 | 拿到盖尔的札记 |
| `ruth_verified` | 4 | 午夜前验明 |
| `gale_dead` | 4 | 盖尔已死（一定设置） |
| `pereira_cleared` | 5 或 6 | 洗清佩雷拉 |
| `pereira_committed` | 5 | 被送审，第六章第一幕可翻案 |
| `pereira_hanged` | 6 | 绞死 |
| `lyle_guilty` | 6 | 爵士定罪 |
| `crane_taken` `crane_dead` | 6 | 二选一 |
| `pym_spoke` `pym_dead` | 6 | 二选一 |
| `holt_found` | 6 | 找到霍尔特 |
| `bottles_tested` | 6 | 化验了露西的瓶子 |
| `exhumed` | 6 | 开棺 |
| `rotterdam_paper` | 6 | 拿到鹿特丹的纸（一定设置） |
| `have_cordial` | 6 | 从岗亭取得蓝瓶 |
| `hannah_stone` | 6 或 7 | 找到墙根的石头 |

变量 `tuck_state`：第六章末设为 1、2 或 3。结局：`a` `b` `c1` `c2` `d` `e` `f1` `f2`。

## 卡片

下面这些卡必须存在，别的章会用。卡片 `chapter` 字段填本章。规律卡要写 `certainty`。

| 章 | id | 种类 | tags | 内容 |
| --- | --- | --- | --- | --- |
| 1 | `c1_stub` | fact | stub | 南的存根 |
| 1 | `c1_scar` | fact | scar | 南的圆疤 |
| 1 | `c1_bottle` | fact | bottle | 南的蓝瓶 |
| 1 | `c1_arsenic` | inference | arsenic | 瓶里有砒霜 |
| 1 | `c1_rule_addington` | rule | | 阿丁顿的铁 |
| 1 | `c1_grey_coat` | testimony，`six = true` | grey_coat | 第一件事 |
| 2 | `c2_bess` | fact | case_dead | 贝丝一例，案例表的行 |
| 2 | `c2_jonas` | fact | case_dead | 乔纳斯一例 |
| 2 | `c2_bowell_body` | fact | no_stub | 鲍尔没有疤和存根 |
| 2 | `c2_rule_list` | rule 或然 | | 这些死者是同一份名单上的人 |
| 2 | `c2_rule_bottle` | rule 或然 | | 这些人都死于蓝瓶 |
| 2 | `c2_rule_bet` | rule 必然 | | 有人为一条命付钱，就有人在这条命上下了注 |
| 2 | `c2_rule_pox` | rule 必然 | | 这种圆疤是人痘疤 |
| 2 | `c2_rule_silence` | rule 或然 | | 知道内情的人也在死 |
| 2 | `c2_note` | fact，source 托马斯的便条 | note_date | 便条 |
| 2 | `c2_lefevre_book` | testimony，`six = true` | | 第二件事 |
| 2 | `c2_society` | fact | society | 友爱长存会 |
| 3 | `c3_rule_ns` | rule 必然 | | 荷兰用新式，差十一天 |
| 3 | `c3_rule_dual` | rule 必然 | | 三月二十五才换年 |
| 3 | `c3_rule_natural` | rule 必然 | | 名义日和自然日 |
| 3 | `c3_rule_eight` | rule 必然 | | 届满前八日验明，旧式 9 月 2 日 |
| 3 | `c3_ten_list` | fact | ten_list | 十处单子，第十一处另有人办 |
| 3 | `c3_visitor` | testimony | | 盖尔：你丈夫是访视人 |
| 3 | `c3_sam` `c3_anne` | fact | unopened | 瓶子没开的两例 |
| 3 | `c3_two_groups` | rule 或然 | | 死亡分成蓝瓶和意外两组 |
| 3 | `c3_cobb_knight` | testimony，`six = true` | cobb_knight | 第三件事 |
| 3 | `c3_sam_share` | testimony，`six = true` | | 第四件事 |
| 3 | `c3_lyle` | inference | lyle | 莱尔爵士雇科布 |
| 3 | `c3_is_17` | fact | is_17 | 她就是 17 号 |
| 3 | `c3_who_sh` | fact | | 未解：S.H. 是谁 |
| 3 | `c3_sleep_line` | 不是卡，是第三章必到台词 | | 老塔克：“睡吧，钟点替你醒着。” |
| 4 | `c4_lawyer` | testimony，`six = true` | | 第五件事 |
| 4 | `c4_letter_lost` | fact | letter_lost | 给内德的信丢了 |
| 4 | `c4_gale_book` | fact | gale_book | 盖尔的札记（`gale_book` 时） |
| 5 | `c5_date_talk` | testimony，`six = true` | | 第六件事 |
| 5 | `c5_anon_letter` | fact | broken_g | 匿名信，缺尾的 g |
| 5 | `c5_clean_wine` | fact | | 马德拉酒无毒，药瓶有毒 |
| 5 | `c5_chaplain` | fact | chaplain | 牧师副署 |
| 5 | `c5_trust` | fact | trust | 信托文书 |
| 6 | `c6_rotterdam` | fact | rotterdam | 鹿特丹的纸 |
| 6 | `c6_tuck` | inference | tuck_is_thomas | 老塔克就是托马斯 |

第 2 章案例表 `t2_dead`，列：`圆疤`、`先令存根`、`蓝瓶`、`三十多岁`、`同一教区`。第 3 章缺席表 `t3_attacks`：行是遭袭的命，列是四名持有人。

## 章节计划字段

`content/story/chN.toml` 见 `docs/story/README.md`，加上：

```toml
[[forced]]            # 时段到了就强制发生，可有 cond
slot = 3
talk = "ch1_rat"
```

第七章计划另有：

```toml
[finale]
flags = [...]         # 求解器穷举的标记
endings = ["a", "b", "c1", "c2", "d", "e", "f1", "f2"]
min_endings = 2
```

## 对质写法

用节点串起来，见 `content/dialogue/ch1/main.dlg` 的 `ch1_inquest`。用 `var:xxx_miss` 记错误次数，3 轮，容错 2 次。

## 金额

凡涉及露丝份额的数字一律先 `~ fund`，再写 `{fund_ruth}`、`{fund_each}`、`{fund_shares}`、`{fund_total}`，不许手写。总数“三万一千镑”可以写。

## 文字量

每章对白 12,000 至 16,000 字。第一章现有样板约 3,600 字，要扩到 9,000 字以上。

## 写作检查

写单章时用 `python3 tools/storyc.py --only N content <临时文件>`，它只编译第 N 章，引用别章的卡和节点先放过。它会拒绝禁用词、过长的行、未登记的说话人和“不是……而是”句式。本章 `content/cast/chN.toml` 要列出本章用到的全部说话人，包括已在别章出现的。日期写成阿拉伯数字加星期，例如“8月13日，星期四”，`tools/story_check.py` 会核对星期。
