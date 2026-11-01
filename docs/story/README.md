# 剧情文档

| 文件 | 内容 |
| --- | --- |
| `01-bible.md` | 世界观、人物、语言风格、史料用法 |
| `02-truth.md` | 真相：实际发生了什么，人物知道什么。唯一事实来源 |
| `03-outline.md` | 七章大纲、选择后果总表、结局 |
| `04-systems.md` | 线索卡、合成、时间、提示、结局规则，可机器检查的不变式 |
| `chapters/ch1.md` … `ch7.md` | 章节脚本：场景、线索、配方、分支 |
| `redteam/` | 红队报告和处理结果 |

玩家实际看到的文字在 `content/`，不在这里。

## 内容文件

```
content/cast.toml              允许开口的人名，以及每个人的“已死”条件
content/story/ch<N>.toml       章节计划：预算、可去的地点、必需结论
content/cards/ch<N>.toml       线索卡和合成配方
content/dialogue/ch<N>/*.dlg   对白
```

### 对白语法（.dlg）

```
# 注释
=== ch1_rudd_first              节点名，全局唯一，小写字母、数字、下划线
拉德大妈: 她不喝酒，这点我敢赌咒。   名字: 台词。名字必须在 cast.toml 里
旁白: 煤气味混着杜松子味。           “旁白”不显示名字
~ card c1_never_drank           效果
if has:c1_no_spirit and not flag:ch1_rudd_angry
  露丝: 那她嘴里为什么闻不到酒气？
elif flag:ch1_rudd_angry
  拉德大妈: 滚。
else
  露丝: ……
end
* 问她南最近吃过什么               选项
  拉德大妈: 面包和咸鱼。
  -> ch1_rudd_menu
* (once) 只能问一次的问题           (once) 选过就消失
  ...
* (has:c1_bottle) 把蓝瓶给她看      (条件) 满足才出现
  ...
* (once, flag:x and not flag:y) 同时用  逗号分隔 once 和一个条件
  ...
* 离开
  -> END
```

选项正文比 `*` 多缩进两格。选项块之后的语句是“汇合点”，选项正文没有跳转时落到这里。`if` 的正文缩进两格，以 `end` 收尾。

**效果**

| 写法 | 含义 |
| --- | --- |
| `~ card c1_x` | 给一张卡 |
| `~ flag x` / `~ clear x` | 设置、清除标记 |
| `~ add v 1` / `~ set v 3` | 改整数变量 |
| `~ time 1` | 花掉时段 |
| `~ chapter 2` | 进入下一章 |
| `~ ending a` | 定下结局 |

**条件**：`has:卡`、`flag:标记`、`seen:节点`、`made:配方`、`var:名字>=2`，用 `and`、`or`、`not` 和括号组合。

### 卡片和配方（cards/*.toml）

```toml
[[card]]
id = "c1_no_spirit"
kind = "fact"            # fact testimony rule inference fallacy
title = "没有酒气"
text = "南的嘴里和衣襟上都闻不到酒气。"
source = "亲眼所见"       # 必填
chapter = 1
# 规律卡必填：certainty = "必然" 或 "或然"

[[recipe]]
id = "d1_not_drunk"
logic = "deduction"      # deduction induction rebut fallacy
inputs = ["c1_rule_spirit", "c1_no_spirit"]
output = "c1_not_drunk"
explain = "大前提：……小前提：……结论：……"
# 可选：fault = "肯定后件：……"（谬误配方必填，“查验”时显示）
# 可选：cond、effects、consumes
```

### 章节计划（story/ch*.toml）

```toml
n = 1
title = "周报"
dates = "1752 年 8 月 1 日至 4 日"
slots = 8
start = "ch1_open"
climax = "ch1_climax"
required = ["c1_not_drunk"]     # 必需结论，至少两条独立路径

[[spot]]
id = "ch1_body"
name = "南的尸体"
place = "迪奥特街酒窖"
talk = "ch1_body_menu"
cond = "flag:x"                  # 可选
once = true                      # 可选
```

高潮节点必须以 `~ chapter N` 收尾，最后一章以 `~ ending x` 收尾。

## 检查

```bash
python3 tools/storyc.py content build/gen/content_data.lua   # 编译，失败即语法或引用错误
python3 tools/story_check.py                                 # 求解器
python3 tools/calendar_facts.py                              # 日期和星期
```
