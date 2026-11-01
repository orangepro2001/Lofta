# 第四章 十一日

1752 年 8 月 31 日星期一至 9 月 14 日星期四。主问题：这一夜救谁？新动词：路线。新历法知识：法院按名义日，契约按自然日。

## 日期与时段预算

白天 4 时段，夜里 8 格，9 月 14 日街景 3 时段。计划文件 `slots = 7`（4 + 3），夜格不占时段，用 `var:night` 记。预算表“4 + 8 夜格”的上限是 12，`slots = 7` 不超。

| 时段 | 日期 | 强制场景 |
| --- | --- | --- |
| 开场 | 8 月 31 日 星期一 上午 | `ch4_open` |
| 0 | 8 月 31 日 下午 | `ch4_bow_first` 弓街，自带 `~ time 1` |
| 1 | 8 月 31 日 夜里 | `ch4_lawyer` 三封信，第五件事 |
| 1 至 2 | 9 月 1 日 星期二 | 自由一个时段 |
| 2 | 9 月 1 日 中午 | `ch4_gale_letter`；`ch4_kitty_letter`（`kitty_alive`） |
| 2 至 3 | 9 月 1 日 下午 | 自由一个时段 |
| 3 | 9 月 2 日 星期三 白天 | `ch4_last_day`，自由一个时段 |
| 4 | 9 月 2 日 18 点起 | `ch4_dusk` 一直走到 `ch4_ambush` |
| 4 | 9 月 14 日 星期四 上午 | `ch4_morning` |
| 4 至 7 | 9 月 14 日 | 街景，三处花时段的探访 |
| 5 | 9 月 14 日 下午 | `ch4_timeline` 时间线 |
| 7 | 9 月 14 日 傍晚至夜里 | 高潮节点 `ch4_end` |

必需线 1 时段（弓街），占白天 25%。自由 3 时段。说服韦尔奇要用满这 3 时段：问渡口习惯 1、算潮水 1、说服 1，所以想拿“内德被截下”，别的花时段的事都做不了。本章不可逆限时事件就是这一夜，第 2 章起反复预告，星期一开场再说一次。

夜格：一格一个钟头，18 点起。第 6 格结束是午夜。第 7 格往回走，第 8 格在教堂巷伏击。最后一站做到第 7 格时，第 8 格才到家，老塔克报“两点过了”。

## 场景表

| 节点 | 地点 | 人物 | 目的 | 花费 |
| --- | --- | --- | --- | --- |
| `ch4_open` | 教堂巷口 | 霍奇、菲普斯太太 | 周报页、杂志短讯（按 `kitty_alive`）、伦敦人算账、她是 17 号 | 0 |
| `ch4_bow_first` | 弓街 | 亨利、约翰 | 带名字，约翰要证据；只给韦尔奇；发 `c4_rule_act`；选择说不说 17 号 | 1 时段 |
| `ch4_lawyer` | 教堂巷岗亭 | 老塔克 | 三封信交他，第五件事 `c4_lawyer`，留灯 | 0 |
| `ch4_gale_letter` | 她屋里 | 盖尔的信 | 我守到午夜，十点落闩，`c4_gale_letter` | 0 |
| `ch4_kitty_letter` | 她屋里 | 基蒂的信 | 巴斯，莫莉 | 0 |
| `ch4_last_day` | | | 328 人，下一期 9 月 19 日 | 0 |
| `ch4_swan` `ch4_swan_first` `ch4_swan_menu` | 老天鹅码头 | 渡工 | 渡口习惯、潮水牌；内德跑格雷夫森德，星期三傍晚回 | 1 时段 |
| `ch4_wapping` `ch4_wapping_ask` | 沃平 | 奥克斯太太 | 同一规律的第二条路，诬告基恩太太的玩家走这里 | 1 时段 |
| `ch4_maddy` `ch4_maddy_menu` `ch4_maddy_route` | 斯皮塔佛德 | 玛德莱娜、勒费弗尔太太 | 算潮水；摊路线（免费）；见母亲 | 1 时段 / 0 / 1 时段 |
| `ch4_welch` `ch4_welch_first` `ch4_welch_menu` | 霍尔本拘留屋 | 韦尔奇 | 闲聊，发起说服 | 0 |
| `ch4_welch_one` `_two` `_three` `_verdict` | 霍尔本拘留屋 | 韦尔奇 | 三轮对质：为什么星期三、哪级台阶几点、怎么知道是科布本人 | 1 时段 |
| `ch4_bow` `ch4_bow_menu` | 弓街 | 约翰、亨利 | 菜单 | 0 |
| `ch4_elias` `ch4_elias_menu` | 亨特讲堂后门 | 以利亚 | 菜单 | 0 |
| `ch4_flea` `ch4_flea_menu` `ch4_flea_cold` | 布罗德街 | 跳蚤 | 卖日子；十二便士；诬告时冷脸 | 0 |
| `ch4_pym` `ch4_pym_menu` | 法衣室 | 皮姆 | 周报跳号；存活证明当众清点（排除皮姆的伏笔） | 0 |
| `ch4_keane` `ch4_keane_cold` | 迪奥特街 | 基恩太太 | 罗马历；诬告时可赔不是 | 0 或 1 时段 |
| `ch4_tuck_night` | 岗亭 | 老塔克 | 邻居的话 | 0 |
| `ch4_dusk` | 教堂巷 | 老塔克 | 先去伯利恒和弗利特；路线单；“我救不完。” | 0 |
| `ch4_assign` `ch4_assign_elias` | 巷口 | 跳蚤、以利亚 | 派遣 | 0 |
| `ch4_route` `ch4_route_menu` `ch4_after_scene` | 路上 | 路人 | 算门，显示赶得上和赶不上，三段路上轻场景 | |
| `ch4_go_lucy` | 伯利恒 | 门房、莫莉、看护、露西 | 杯子、童谣、肥皂、值夜簿 | 路 1，场 2（基蒂 1） |
| `ch4_go_will` `ch4_will` `ch4_will_b` | 弗利特 | 看守、威尔、欠债人 | 楼梯、推和摔的区别、债、十一道 | 路 1，场 2（旁注 1） |
| `ch4_go_ned` `ch4_ned` `ch4_ned_cobb` `ch4_ned_end` | 老天鹅 | 内德、科布 | 信没到、墨、市长、等水走 | 路 1（诬告 2），场 1 |
| `ch4_go_gale` | 法官巷 | 门房、菲奇、盖尔 | 验明、札记、同一只手 | 路 1，场 1 |
| `ch4_no_more` | | | 走不动了，转午夜 | |
| `ch4_midnight` `ch4_bell_*` `ch4_bell_count` `ch4_after_*` `ch4_bell_back` | 所在之处 | 随地点 | 午夜五个版本：黑马酒馆、伯利恒、弗利特、老天鹅、盖尔榻前 | |
| `ch4_return` | 法衣室窗外 | 皮姆（远景） | 十几人看他清点 | 第 7 格 |
| `ch4_ambush` | 教堂巷 | 科布、老塔克、韦尔奇 | 伏击，棍子，手套，手臂 | 第 8 格 |
| `ch4_morning` | 她屋里 | 以利亚、跳蚤、内德 | 战果，`gale_dead`，信的后备发现，校样缝里写名字 | 0 |
| `ch4_timeline` | 弓街 | 约翰 | 摊开时间线，`c4_timeline` | 0 |
| `ch4_st_sleeper` `ch4_st_maid` `ch4_st_boy` `ch4_st_crank` `ch4_st_hodge` `ch4_st_flea` `ch4_st_elias` `ch4_st_pym` | 街景 | 路人 | 七小时、讨工钱、算生日、二十二位、算了一个月、十二便士 | 0 |
| `ch4_st_lucy` `ch4_st_will` `ch4_st_ned` `ch4_st_maddy` | 各处 | 幸存者或空屋 | 活着的话，死了的动作 | 各 1 时段 |
| `ch4_st_home` | 教堂巷 | | 结束这一天 | 用完时段 |
| `ch4_end` | 七日晷、窗边 | 酒客、拉德大妈、老塔克 | 全城庆祝，没有人问蓝瓶，还剩七天 | 高潮 |

对白 14,983 字，下限 12,000、上限 15,000。玩家一次通关只读到其中一部分：午夜五选一，四处现场按路线取舍，死活各一版。

## 卡片表

非谬误 12 张，正好到预算上限。

| id | 种类 | tags | 得到方式 |
| --- | --- | --- | --- |
| `c4_lawyer` | testimony，six，source 老塔克 | last_night | `ch4_lawyer`，强制 |
| `c4_rule_act` | rule 必然 | rule_act | `ch4_bow_first`，强制 |
| `c4_gale_letter` | fact | last_night term_day | `ch4_gale_letter`，强制 |
| `c4_cobb_water` | rule 或然 | cobb_water | 渡工或奥克斯太太 |
| `c4_wapping_tide` | fact | tide_wapping | 同上，桥下头的潮 |
| `c4_tide_table` | fact | tide | 玛德莱娜算的，桥上头的潮 |
| `c4_tide` | inference | | `d4_tide` |
| `c4_letter_lost` | fact | letter_lost | 见到内德时；否则 `ch4_morning`，一定拿到 |
| `c4_gale_book` | fact | gale_book | 赶上盖尔 |
| `c4_timeline` | fact | cobb_timeline | `ch4_timeline`，强制 |
| `c4_seven` | inference | | `d4_seven` |
| `c4_no_owner` | inference | | `d4_no_owner` |
| `c4_tide_wrong` | fallacy，不计入预算 | | `f4_tide` |

`required`：`c4_lawyer`、`c4_rule_act`、`c4_letter_lost`。前两张来自强制场景；`c4_letter_lost` 有三个来源，`ch4_morning` 是强制场景，兜底。

## 配方表

4 条，预算 5 条。

| id | 大前提 | 小前提标签 | 结论 | 逻辑形式 |
| --- | --- | --- | --- | --- |
| `d4_tide` | `c4_cobb_water` | tide | `c4_tide` | 凡科布落水之事，必在落潮过半；今夜此地落潮过半在此时；所以他此时在此地 |
| `f4_tide` | `c4_cobb_water` | tide_wapping | `c4_tide_wrong` | 同一形式，小前提是彼地的钟点（此地当各地） |
| `d4_seven` | `c4_rule_act` | term_day | `c4_seven` | 凡契约按自然日；此约届满在旧式 9 月 10 日；所以届满在新式 9 月 21 日 |
| `d4_no_owner` | `c3_two_groups` | cobb_timeline | `c4_no_owner` | 死亡分作两组；此人只碰过其中一组；所以另一组另有其人 |

算术：老天鹅七点满潮，一点见底，落潮六个钟头，过大半取四个钟头，敲过十一点。谬误版用桥下头的六点和十二点，得十点，差一个钟头。

谬误 `f4_tide` 是“此地当各地”在全游戏的第一次出现，后果轻：韦尔奇当场拆穿，记一错，这一轮重来。玛德莱娜也说破一次，并把它和荷兰日子连起来，给第五章铺路。

对质 `ch4_welch_*`：三轮，容错到 2 错不成立。第二轮必须出示 `c4_tide`（`welch_tide`），否则再少错也不成立，因为他要的是台阶和钟点。第一轮可以出示 `c4_lawyer`，韦尔奇说“我记下了”：这条推理的一条腿架在老塔克的话上，第六章回看时有分量。

## 标记

读取前章：`ch1_bill_true`（亨利的称呼）、`ch1_took_groat`（伯利恒看护的价钱）、`ch1_blamed_keane`（跳蚤不来、渡工不载、基恩太太）、`kitty_alive`、`register_note`、`have_almanac`、`ch3_fitch_bribed`（菲奇的话）。读取前章的卡：`c3_lyle`、`c3_rule_eight`、`c3_cobb_knight`、`c1_grey_coat`、`c3_two_groups`。每种取值都有内容，见分支。

对外设置：`lucy_alive`、`will_alive`、`ned_alive`、`ruth_verified`、`gale_book`、`gale_dead`（一定设置）、`ch4_done`。

本章内部：`welch_won`、`welch_tide`、`ch4_told_john`、`ch4_keane_sorry`、`ch4_saw_violette`、`ch4_set_arm`、`ch4_stepped`、`ch4_night_over`、`home_late`、`help_*`、`flea_*`、`elias_*`、`can_*`、`late_*`、`*_done`、`bell_done`、`road_*`。变量 `night`、`at`、`stops`、`walled_first`、`bell_back`、`welch_miss`。

`[mortal]`：盖尔 `flag:gale_dead`；露西、威尔、内德 `flag:ch4_done and not flag:<x>_alive`。

## 分支和后果

**选择一：白天三个自由时段。** 走渡口习惯、潮水、说服韦尔奇，换 `welch_won`，内德不用她去，满分路线才存在。花在别处的选项：见勒费弗尔太太（她的律师，乔纳斯的妹妹，第 5 章冻结基金的前声）；向基恩太太赔不是（诬告路线，跳蚤和渡工不回头，只有基恩太太那句“我还在学”，和 9 月 14 日跳蚤的一句话）。

**选择二：星期一告不告诉约翰 17 号。** 说了，他当场问疤，9 月 14 日说“科布不知道您是 17 号”。不说，9 月 14 日他说“您星期一停的那一下，我现在听懂了”；没验明时她自己开口。第五章约翰已知她是 17 号，所以两条路在这里合流。

**选择三：帮手。** 一人一扇门，门上写明改到第几格。放错了门等于没放。

**选择四：路线次序。** 老塔克说先去伯利恒和弗利特。照做得露西和威尔，丢盖尔、丢札记、丢验明，内德也来不及。他的话是真的，只是经过挑选。`walled_first>=2` 时玛德莱娜问一句，第六章回看。

**选择五：科布的手臂。** 摆正、不动、踩上去。后果在约翰的话里：别人怎么看她。

| 目标 | 救下 | 没救下 |
| --- | --- | --- |
| 露西 | `lucy_alive`，值夜簿有一行，9 月 14 日她笑了一下 | 第二天没醒，杯子洗过了，值夜簿写“无事” |
| 威尔 | `will_alive`，划掉十一道 | 楼梯，手掌没有伤；她替他划掉 |
| 内德 | `ned_alive`，字写上了 | 比林斯门台阶；她去写“市长” |
| 内德（韦尔奇） | `ned_alive`，关了一夜要告状，9 月 14 日才写字 | |
| 盖尔 | `ruth_verified`、`gale_book`、`c4_gale_book` | 天亮才发现，札记归菲奇，17 号那一行空着 |

`gale_dead` 在 `ch4_morning` 设置，所有路线都经过。

## 路线穷举

规则。目标四个，路程、现场、最迟：露西 1/2/5，威尔 1/2/6，内德 1/1/6，盖尔 1/1/4。`kitty_alive` 露西现场减 1，`register_note` 威尔现场减 1，`welch_won` 内德不用她去，`ch1_blamed_keane` 内德路程加 1 且跳蚤不来。帮手让所在目标的最迟晚 1 格。

落地时的两条约束，也就是对帮手效果的调整：

1. **一扇门一个人，不叠加。** 两个帮手不能放在同一处。
2. **最后一站最多做到第 7 格。** 要留一格回家，这条由最迟格本身保证：帮手最多把内德和威尔推到第 7 格。

放开这两条，韦尔奇单独一条推理就能满分，基蒂加旁注不要韦尔奇也能满分，潮水推理就可有可无。保留这两条，满分要韦尔奇再加至少一条捷径。见下面“放开时”一行，W 韦尔奇，K 基蒂，R 旁注，等号后是最多救几个。

另一条写法上的约束：只要还有赶得上的目标，她就必须继续走，不给“提前回家”的选项，赶不上的目标显示“赶不上了”但不能选。所以最差路线也救下 2 个。

穷举结果。“最差”“最好”都算韦尔奇截下的内德，“满分分配”是能走出 4 个目标的帮手分配数。

| 韦尔奇 | 基蒂 | 看守名 | 诬告 | 目标数上限 | 帮手分配数 | 路线总数 | 最差 | 最好 | 不派帮手最好 | 满分分配 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 否 | 否 | 否 | 否 | 4 | 12 | 126 | 2 | 3 | 2 | 0 |
| 否 | 否 | 否 | 是 | 4 | 4 | 33 | 2 | 2 | 2 | 0 |
| 否 | 否 | 是 | 否 | 4 | 12 | 140 | 2 | 3 | 3 | 0 |
| 否 | 否 | 是 | 是 | 4 | 4 | 39 | 2 | 3 | 2 | 0 |
| 否 | 是 | 否 | 否 | 4 | 12 | 154 | 2 | 3 | 3 | 0 |
| 否 | 是 | 否 | 是 | 4 | 4 | 42 | 2 | 3 | 2 | 0 |
| 否 | 是 | 是 | 否 | 4 | 12 | 192 | 2 | 3 | 3 | 0 |
| 否 | 是 | 是 | 是 | 4 | 4 | 47 | 2 | 3 | 3 | 0 |
| 是 | 否 | 否 | 否 | 4 | 6 | 30 | 3 | 3 | 3 | 0 |
| 是 | 否 | 否 | 是 | 4 | 3 | 13 | 2 | 3 | 3 | 0 |
| 是 | 否 | 是 | 否 | 4 | 6 | 34 | 3 | 4 | 3 | 4 |
| 是 | 否 | 是 | 是 | 4 | 3 | 16 | 3 | 4 | 3 | 1 |
| 是 | 是 | 否 | 否 | 4 | 6 | 34 | 3 | 4 | 3 | 4 |
| 是 | 是 | 否 | 是 | 4 | 3 | 16 | 3 | 4 | 3 | 1 |
| 是 | 是 | 是 | 否 | 4 | 6 | 36 | 3 | 4 | 4 | 6 |
| 是 | 是 | 是 | 是 | 4 | 3 | 18 | 3 | 4 | 4 | 3 |

放开时（三个旗标的组合，帮手都在，未诬告）：

```
rules as shipped: one helper a door, cell 7 at most: ---=3 --R=3 -K-=3 -KR=3 W--=3 W-R=4 WK-=4 WKR=4
helpers may stack on one door, walk home dropped: ---=3 --R=3 -K-=3 -KR=4 W--=4 W-R=4 WK-=4 WKR=4
```

replay: 970 complete nights through the compiled dialogue, 0 disagreements, 0 dialogue problems。这一行用的是真实对白：脚本把编译后的节点跑一遍，帮手菜单和路线菜单全部枚举，得到的最差和最好与表一致。

结论：

- 零推理保底成立。十六种组合里，最差路线也不低于 2 个目标。没有韦尔奇、没有捷径时，先盖尔再露西是 2 个；未诬告时再把一个帮手放在老天鹅，第 7 格赶上内德，是 3 个。
- 满分路线存在。韦尔奇加基蒂或旁注，再把一个帮手放在弗利特：盖尔第 2 格，露西第 4 或 5 格，威尔第 7 格。三样都有时不靠帮手，盖尔、露西、威尔正好第 2、4、6 格。
- 帮手不会让全部目标太容易。没有韦尔奇时最多 3 个；只有韦尔奇没有捷径时也是 3 个。帮手的用处是把差一格的第三站或第四站接上，放错门就白放。
- 诬告的代价落在路上。没有捷径时只剩 2 个，比未诬告少 1 个。有韦尔奇时内德的路程不再要紧，以利亚一个帮手仍能走到满分。

## 这一夜的钟点

| 格 | 结束 | 门和事 |
| --- | --- | --- |
| 1 | 19 点 | 钱送到摩尔菲尔德和弗利特；老天鹅满潮，内德随潮回来 |
| 2 | 20 点 | 法衣室开始念证明；盖尔已经开始吐 |
| 4 | 22 点 | 公会落闩（帮手拖到 23 点） |
| 5 | 23 点 | 伯利恒封廊，杯子端进去（帮手拖到午夜）；科布到老天鹅 |
| 6 | 24 点 | 弗利特落栅；内德收船（帮手各拖到 1 点）；午夜 |
| 7 | 1 点 | 她回教堂巷；科布沿泰晤士街到教堂巷 |
| 8 | 2 点 | 伏击；老塔克走到灯下 |

午夜落在哪：第 6 格做完在那一处；第 7 格才做完的，钟在到达时或现场中间响；更早做完无处可去时，在盖尔榻前（最后一站是盖尔）或黑马酒馆（其余）。

她在 `ch4_bell_count` 里用十二下钟数十一条命，第十二下没有名字。这段各版本共用，前后两段按地点写。

## 轻场景

连续两个沉重场景之间至少一个轻场景。夜里每走完一站，`ch4_route` 插一段路上小戏：新人讲价、旧式的鲱鱼、学徒的七年。

- 跳蚤的蠢事：卖“今年没人要的日子”，九月三日到十三日的废书页，一张一便士，水手买了十号给老婆过生日，被打（`ch4_flea_menu`、`ch4_st_flea`）。
- “十一日”轻事件：霍奇的粉笔账写上门框，9 月 14 日写了半截墙，算出来和不算一样；九月十九天；七小时睡掉十一天；讨工钱；算生日；二十二位先生退稿。
- 亨利的腿，以利亚的桶，皮姆的茶，韦尔奇的靴子，玛德莱娜母亲的枕头。

沉重场景：四处现场、午夜、伏击、早晨的名单。9 月 14 日先沉后轻，时间线之后再收回来。

## 埋下的线索和回收位置

| 线索 | 位置 | 回收 |
| --- | --- | --- |
| 拇指数信 | `ch4_lawyer`（必经） | 第六章“身体” |
| 第五件事，“皮姆说的”，她想不出皮姆喝多的样子 | `ch4_lawyer`（必经） | 第六章对照表，皮姆否认 |
| 三封信，只丢内德一封；“没有到他手里。是我的错” | `ch4_ned` 或 `ch4_morning`，`ch4_end` | 第六章 `c4_letter_lost` |
| 先去伯利恒和弗利特 | `ch4_dusk`（必经），`walled_first` | 第六章：他让她先保 S.H. 的命 |
| 法衣室十几人看皮姆念证明 | `ch4_pym_menu`、`ch4_return`（必经）、`ch4_st_pym` | 第六章排除皮姆 |
| 手抖，换握法；手套；灯笼落地 | `ch4_ambush`（必经） | 第六章“身体”，第七章 |
| 桌上的马德拉酒和名片，窗台的药瓶 | `ch4_go_gale` | 第五章试酒试药 |
| 同一只手 | `ch4_go_gale`，她当成凶手是一个人 | 第五、六章 |
| 报病的人写着两个字母 | `ch4_st_maddy` | 第六章 |
| 科布整夜没到法官巷 | `ch4_timeline`，`c4_timeline` | 第五章第一幕 |
| 科布不知道她是 17 号 | `ch4_timeline`（`ch4_told_john`） | 第六章律师 |
| 左脚摔伤 | `ch4_ambush` | 第五章约翰听出来 |
| 荷兰的日子和英国的日子都对 | `ch4_maddy_menu` | 第五章 |
| 此地当各地 | `f4_tide`，低风险首次出现 | 第五章“英国没有这一天” |
| 勒费弗尔太太的律师 | `ch4_maddy_menu` | 第五章冻结基金 |

第三章的三个小请求全部兑现：市长的字、肥皂、十一道。赶不上时由她事后做完，写在 `ch4_st_ned`、`ch4_st_will`；肥皂没有送到的那一版不提，留白。

## 史实及出处

研究笔记 `docs/research/history-notes.md`：

- 周报 8 月 25 日当周 371 人、天花 57；9 月 1 日 328；下一期 9 月 19 日，账面隔 18 天实隔 7 天（第 2 节，Creighton、GM 1752 年 9 月）。
- 法案第 1、4、6 条：法院名义日、集市和租金自然日、利息按真实天数、足额年日；“各该同样的自然日”用复数（第 1 节）。
- 老贝利 9 月 14 日开庭，新式第一天（第 12 节）。
- 《科文特花园杂志》周六刊，3 便士（第 7 节）。
- “Have I slept away 11 days in 7 hours”（GM 1752 年 9 月）。折减表（GM 1752 年 8 月）。Philalethes 二十二位（GM p.431）。
- 守夜人晚 9 点到早 6 点，报时辰天气，岗亭灯笼（守夜一节）。所以 `ch4_dusk` 写“还没到守夜的钟点”。
- 韦尔奇 1752 年是霍尔本高级警役；老天鹅在城里，他管不着（第 7 节）。
- 弗利特市场盖在河上；伦敦桥上还有房子；只有两座桥（第 10 节）。

笔记以外的常识，未核一手：乔纳森·怀尔德 1725 年绞死；尤维纳利斯“谁来看管看守的人”；马尔普拉凯 1709 年；客人在老天鹅下船绕过桥（第三章同样用了）。

潮水：1752 年 9 月 13 日（新式）按平均朔望月推算月龄约 5.1 天。“朔望三点满潮、每天迟四十八分”是船夫的经验算法，得七点满潮。桥上桥下差一个钟头是简化。

虚构，不假装史实：公会十点落闩、伯利恒十一点封廊、弗利特午夜落栅、守夜人岗亭传信、值夜簿、科布的习惯、潮水牌。

## 穷举脚本

从仓库根目录运行。规定不增加文件，脚本放在这里。

```python
#!/usr/bin/env python3
"""Exhaustive check of the night of 2 September (chapter 4), in two halves.

model():  the table from the outline, every helper assignment, every route.
replay(): the same search through the compiled dialogue, so the prose cannot drift from the table.
Run from the repository root.
"""
import itertools
import pathlib
import random
import sys

sys.path.insert(0, "tools")
import storyc
import story_check

NAMES = ("lucy", "will", "ned", "gale")
ALIVE = {"lucy": "lucy_alive", "will": "will_alive", "ned": "ned_alive", "gale": "ruth_verified"}
FLAGS = ("welch_won", "kitty_alive", "register_note", "ch1_blamed_keane")
LAST_CELL = 7        # a stop may run into cell 7, then the walk home is cell 8


def table(welch, kitty, register, blamed):
    road = dict(lucy=1, will=1, ned=2 if blamed else 1, gale=1)
    scene = dict(lucy=1 if kitty else 2, will=1 if register else 2, ned=1, gale=1)
    last = dict(lucy=5, will=6, ned=6, gale=4)
    goals = [n for n in NAMES if not (welch and n == "ned")]
    return road, scene, last, goals


def assignments(goals, blamed, helpers=True):
    """Flea stays away if Keane was blamed. One helper to one door; two never share a door."""
    if not helpers:
        return [()]
    if blamed:
        return [(e,) for e in goals]
    return [(f, e) for f in goals for e in goals if f != e]


def routes(road, scene, last, goals, helped):
    """The player must keep walking while any door is open; a shut door cannot be chosen."""
    def walk(night, todo, order):
        moves = []
        for n in todo:
            finish = night + road[n] + scene[n]
            if finish <= min(last[n] + (n in helped), LAST_CELL):
                moves.append((n, finish))
        if not moves:
            yield tuple(order)
            return
        for n, finish in moves:
            yield from walk(finish, todo - {n}, order + [n])
    yield from walk(0, set(goals), [])


def model():
    print("| 韦尔奇 | 基蒂 | 看守名 | 诬告 | 目标数上限 | 帮手分配数 | 路线总数 | 最差 | 最好 | 不派帮手最好 | 满分分配 |")
    print("| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |")
    summary = {}
    for welch, kitty, register, blamed in itertools.product((0, 1), repeat=4):
        road, scene, last, goals = table(welch, kitty, register, blamed)
        worst, best, total, full = 9, 0, 0, set()
        plans = assignments(goals, blamed)
        for plan in plans:
            for order in routes(road, scene, last, goals, set(plan)):
                score = len(order) + welch
                total += 1
                worst, best = min(worst, score), max(best, score)
                if score == 4:
                    full.add(plan)
        bare = max(len(o) + welch for o in routes(road, scene, last, goals, set()))
        y = lambda b: "是" if b else "否"
        print(f"| {y(welch)} | {y(kitty)} | {y(register)} | {y(blamed)} | 4 | {len(plans)} | {total} "
              f"| {worst} | {best} | {bare} | {len(full)} |")
        summary[(welch, kitty, register, blamed)] = (worst, best, bare, len(full), len(plans))
    return summary


def what_if():
    """The two rules that keep helpers honest. Loosen either and reasoning stops mattering."""
    def best(welch, kitty, register, plan, cap):
        road, scene, last, goals = table(welch, kitty, register, 0)
        high = dict(last)
        for n in plan:
            high[n] += 1
        def walk(night, todo):
            top = 0
            for n in todo:
                finish = night + road[n] + scene[n]
                if finish <= min(high[n], cap):
                    top = max(top, 1 + walk(finish, todo - {n}))
            return top
        return walk(0, set(goals)) + welch
    for name, cap, plans in (
            ("rules as shipped: one helper a door, cell 7 at most", 7,
             [(a, b) for a in NAMES for b in NAMES if a != b]),
            ("helpers may stack on one door, walk home dropped", 8,
             [(a, b) for a in NAMES for b in NAMES])):
        rows = []
        for welch, kitty, register in itertools.product((0, 1), repeat=3):
            top = max(best(welch, kitty, register, plan, cap) for plan in plans)
            rows.append(f"{'W' if welch else '-'}{'K' if kitty else '-'}{'R' if register else '-'}={top}")
        print(name + ": " + " ".join(rows))


class Replay:
    def __init__(self):
        self.problems = story_check.Problems()
        root = pathlib.Path("content")
        data, _ = storyc.compile_content(root, 4)
        self.game = story_check.Game(data, storyc.load_cast(root, 4), self.problems)

    def run(self, flags, picks):
        """Plays dusk to morning. Route and helper menus follow picks; other menus take option 0."""
        state = story_check.State()
        state.flags = set(flags)
        state.cards = ["c4_gale_letter"]
        depth = [0]
        seen_menus = []

        def choose(s, node, options):
            if node not in ("ch4_assign", "ch4_assign_elias", "ch4_route_menu"):
                return options[0]
            live = [o for o in options if "赶不上" not in o["text"]]
            i = depth[0]
            depth[0] += 1
            seen_menus.append(len(live))
            pick = picks[i] if i < len(picks) else 0
            return live[pick]

        self.game.run_node(state, "ch4_dusk", choose)
        self.game.run_node(state, "ch4_morning", choose)
        return state, seen_menus

    def explore(self, flags):
        results = []
        stack = [[]]
        while stack:
            picks = stack.pop()
            state, menus = self.run(flags, picks)
            for i in range(len(picks), len(menus)):
                for alt in range(1, menus[i]):
                    stack.append(picks + [0] * (i - len(picks)) + [alt])
            results.append(state)
        return results


def replay(summary):
    r = Replay()
    bad = 0
    runs = 0
    for welch, kitty, register, blamed in itertools.product((0, 1), repeat=4):
        flags = {f for f, v in zip(FLAGS, (welch, kitty, register, blamed)) if v}
        scores = []
        for state in r.explore(flags):
            runs += 1
            score = sum(ALIVE[n] in state.flags for n in NAMES)
            scores.append(score)
            for must in ("gale_dead", "ch4_night_over", "bell_done"):
                if must not in state.flags:
                    print("MISSING", must, flags)
                    bad += 1
            if "c4_letter_lost" not in state.cards:
                print("NO LETTER CARD", flags)
                bad += 1
            if state.vars.get("night") != 8:
                print("NIGHT ENDS AT", state.vars.get("night"), flags)
                bad += 1
        worst, best, _, _, _ = summary[(welch, kitty, register, blamed)]
        if (min(scores), max(scores)) != (worst, best):
            print("MISMATCH", flags, (min(scores), max(scores)), "model", (worst, best))
            bad += 1
    print(f"\nreplay: {runs} complete nights through the compiled dialogue, {bad} disagreements, "
          f"{len(r.problems.items)} dialogue problems")
    for item in r.problems.items[:20]:
        print(item)
    return bad


if __name__ == "__main__":
    s = model()
    print()
    what_if()
    sys.exit(1 if replay(s) else 0)
```
