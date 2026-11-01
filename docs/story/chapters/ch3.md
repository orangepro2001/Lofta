# 第三章 名单

1752 年 8 月 20 日星期四至 8 月 29 日星期六。主问题：谁在拿命下注？新动词：反驳二、缺席表。

## 日期与时段预算

共 18 时段。时段和日期按下表大致对应，只有固定 forced 场景写明日期。

| 时段 | 日期 |
| --- | --- |
| 0—1 | 8 月 20 日 星期四 |
| 2—3 | 8 月 21 日 星期五 |
| 4—5 | 8 月 22 日 星期六（集市宣告） |
| 6—7 | 8 月 23 日 星期日 |
| 8—9 | 8 月 24 日 星期一（基蒂预告） |
| 10—11 | 8 月 25 日 星期二（风暴夜，不可逆） |
| 12—13 | 8 月 26 日 星期三 |
| 14—15 | 8 月 27 日 星期四 |
| 16—17 | 8 月 28 日 星期五 |
| 章末 | 8 月 29 日 星期六 |

必需线 9 时段（弓街、盖尔、牛栏十字、内德、露西、威尔、沃平、克拉肯韦尔、长存册第一次），占 50%。限时内容 2 时段（基蒂预告、风暴夜），占 11%，不含必需结论：基蒂这一例无论救没救都进表。其余 7 时段自由。

## 场景表

| 节点 | 地点 | 人物 | 目的 | 时段 |
| --- | --- | --- | --- | --- |
| `ch3_open` | 科文特花园市场 | 账房、水果贩、跳蚤 | 周报页；十一日第一波笑话 | 0 |
| `ch3_market` | 科文特花园市场 | 水果贩、账房 | 轻场景，贝丝的旧摊位 | 0 |
| `ch3_flea` | 布罗德街 | 跳蚤 | 闲聊；`bowell_soothed` 小戏；灰外套进爵士宅 | 0 或 1 |
| `ch3_bow_first` | 弓街 | 亨利、约翰、科布一闪 | 三问；少答一题 | 1 |
| `ch3_gale_first` | 法官巷 | 盖尔 | 访视人、到期日、八日验明 | 1 |
| `ch3_letter` | 斯皮塔佛德 | 玛德莱娜 | 鹿特丹来信、《历书》 | 0 |
| `ch3_note_check` | 家中 | 露丝 | 查便条，放下 | 0，forced |
| `ch3_griggs_first` | 牛栏十字 | 普拉特太太 | 十处单子 | 1 |
| `ch3_tuck_cobb` | 岗亭 | 老塔克 | 第三件事，第一封信 | 0，forced |
| `ch3_tuck_cry` | 岗亭 | 老塔克 | 必到台词 | 0，forced |
| `ch3_ned_first` | 老天鹅码头 | 内德 | 狗的木牌 | 1 |
| `ch3_lucy_first` | 伯利恒 | 露西、看护 | 洗衣房童谣 | 1 |
| `ch3_will_first` | 弗利特 | 威尔、看守 | 楼梯 | 1 |
| `ch3_sam_first` | 沃平 | 奥克斯太太 | 原封的瓶 | 1 |
| `ch3_anne_first` | 克拉肯韦尔 | 特纳太太 | 原封的瓶 | 1 |
| `ch3_kitty_warn` | 史密斯菲尔德 | 基蒂 | 预告，8 月 24 日 | 1，forced |
| `ch3_storm` | 史密斯菲尔德或别处 | 基蒂、打手 | 不可逆选择，8 月 25 日 | 1 或 0，forced |
| `ch3_storm_news` | 史密斯菲尔德 | 跳蚤 | 没去时的后果，发 `c3_kitty_case` | 0，forced，`ch3_kitty_dead` 时 |
| `ch3_flea_tickets` | 布罗德街 | 跳蚤 | 蠢事：卖九月的日子 | 0，forced |
| `ch3_tuck_share` | 岗亭 | 老塔克 | 第四件事，第二封信 | 0，forced |
| `ch3_cobb_bridge` | 弗利特桥 | 科布 | 礼貌的威胁 | 0，forced |
| `ch3_violette` | 斯皮塔佛德 | 勒费弗尔太太 | 说服委托书 | 1 |
| `ch3_fitch` | 法官巷 | 菲奇 | 买通 | 0 |
| `ch3_reg_first` | 法官巷 | 看路线 | 抄 E.L.、V.L.、M.P. 三页 | 1 |
| `ch3_elias` | 科文特花园 | 以利亚 | 轻场景；查验 | 0 或 1 |
| `ch3_climax` | 法官巷、法衣室 | 皮姆 | 赫尔那页；17 号；问皮姆 | 章末 |

## forced 兜底

必到场景都有 forced 兜底，写成 `cond = "not seen:节点"`。玩家没去，时段到了事情自己找上门。第 2 至 6 时段兜底弓街、盖尔、来信、牛栏十字；第 12 至 14 时段兜底内德、露西、威尔、沃平、克拉肯韦尔；第 15 时段兜底长存册第一次。长存册没选路线时默认跳蚤。科布的场景排在第 15 时段，放在三条命和两处现场之后，他才能报出她走过的地方。

`c3_two_groups`、`c3_lyle` 要玩家自己合成，这一章不代做。第四章读到这两张牌时用 `has:` 判断，没合成也能往下走。

## 卡片表

共 20 张，不含谬误。

| id | 种类 | tags | 来源场景 |
| --- | --- | --- | --- |
| `c3_visitor` | testimony | | 盖尔 |
| `c3_rule_natural` | rule 必然 | | 盖尔 |
| `c3_rule_eight` | rule 必然 | | 盖尔 |
| `c3_rule_ns` | rule 必然 | | 玛德莱娜 |
| `c3_rule_dual` | rule 必然 | | 玛德莱娜 |
| `c3_ten_list` | fact | ten_list, griggs | 牛栏十字 |
| `c3_lucy_bottle` | fact | | 露西，瓶子在院方药房 |
| `c3_will_case` | fact | attacked | 威尔 |
| `c3_sam` | fact | unopened, attacked | 沃平 |
| `c3_anne` | fact | unopened, attacked | 克拉肯韦尔 |
| `c3_kitty_case` | fact | attacked | 风暴夜，两个分支都给 |
| `c3_cobb_knight` | testimony, six | cobb_knight | 老塔克 |
| `c3_sam_share` | testimony, six | | 老塔克 |
| `c3_flea_grey` | testimony | cobb_knight | 跳蚤，第二条路 |
| `c3_ten_doomed` | inference | | 配方 |
| `c3_two_groups` | rule 或然 | | 配方 |
| `c3_absent_el` | rule 或然 | | 配方 |
| `c3_lyle` | inference | lyle | 配方 |
| `c3_is_17` | fact | is_17 | 章末 |
| `c3_who_sh` | fact | | 章末 |
| `c3_griggs_did` | fallacy | | 配方 |
| `c3_mp_did` | fallacy | | 配方 |

## 配方表

| id | 动词 | 逻辑形式 | 输出 |
| --- | --- | --- | --- |
| `d3_ten_doomed` | 演绎 | 凡名单上的人都死于蓝瓶；这十处都发了蓝瓶；所以这十人都要死于蓝瓶 | `c3_ten_doomed` |
| `f3_griggs` | 演绎，谬误 相关当因果 | 瓶子杀人；格里格斯发瓶；所以他杀人。他 6 月已死，7 月、8 月还在死人 | `c3_griggs_did` |
| `r3_two_groups` | 反驳 | 规律：都死于蓝瓶。反例：萨姆、安妮的瓶子没开。改写：死亡分两组 | `c3_two_groups`，retires `c2_rule_bottle` `c3_ten_doomed` `c3_griggs_did` |
| `i3_absent` | 缺席表 | 四例遭袭，E.L. 一列全空；所以袭击在替 E.L. 清路 | `c3_absent_el` |
| `f3_most` | 归纳，谬误 以偏概全 | M.P. 这一列最多；所以佩雷拉脱不了干系 | `c3_mp_did` |
| `d3_lyle` | 演绎 | 袭击替 E.L. 清路；科布替一位爵士跑腿；E.L. 是持有人里唯一的爵士；所以莱尔爵士雇科布 | `c3_lyle` |

`i3_absent` 和 `f3_most` 要 `seen:ch3_reg_first`，列名来自册子。

`t3_attacks` 行：`c3_anne`（V.L.）、`c3_sam`（M.P.）、`c3_kitty_case`（M.P.）、`c3_will_case`（S.H.）。

`c3_lyle` 两条路：`c3_cobb_knight`（老塔克）或 `c3_flea_grey`（跳蚤），标签同为 `cobb_knight`。

## 标记

读取：`ch1_took_groat`、`ch1_bill_true`、`ch1_blamed_keane`、`bowell_soothed`。

对外设置：`have_almanac`、`kitty_alive`、`register_note`、`ch3_done`。

本章内部：`ch3_griggs_known`、`ch3_knows_cobb`、`ch3_kitty_dead`、`ch3_proxy_asked`、`ch3_proxy`、`ch3_proxy_refused`、`ch3_route_flea`、`ch3_storm_raid`、`ch3_spent_shillings`。变量：`ch3_john_left`（约翰还答几题）、`ch3_vio_miss`（说服勒费弗尔太太的失误）、`ch3_route`（1 委托书、2 菲奇、3 夜探）。

**给第四章读：** `ch3_fitch_bribed`。她买通了菲奇，爵士知道她抄过册子。

## 分支后果

1. **风暴夜。** 去集市：`kitty_alive`，打手逃脱，基蒂去巴斯，第四章露西现场减 1。不去：`ch3_kitty_dead`，基蒂死。趁风暴夜探：长存册第一次不花后面的时段，基蒂死。
2. **长存册路线。** 委托书：花 1 时段说服勒费弗尔太太，可能失败；白天抄，旁注白得。夜探：可选抄旁注，跳蚤挨打。菲奇：设 `ch3_fitch_bribed`，要旁注须付出三枚托马斯的先令（第二章那卷布里的七枚），设 `ch3_spent_shillings`。
3. **勒费弗尔太太。** 说服失败设 `ch3_proxy_refused`，这条路关闭。
4. **约翰的回答。** `ch1_took_groat` 时只答两题。问哪两题决定她知不知道韦尔奇和科布的名字。
5. **内德、露西、威尔的承诺。** 答不答应只改变对方的语气。

## 第四章兑现的小请求

- 内德：带墨来，替他在木牌上写狗的名字“市长”。
- 露西：带一小块肥皂。
- 威尔：9 月 2 日夜来作个见证，看他在墙上划掉十一道。

## 埋线与回收

| 线索 | 埋在 | 回收 |
| --- | --- | --- |
| “睡吧，钟点替你醒着。” | `ch3_tuck_cry` | 第六章划线 |
| 便条星期错，当发烧 | `ch3_note_check` | 第六章翻回来 |
| 老塔克递信两次 | `ch3_tuck_cobb`、`ch3_tuck_share` | 第四章、第六章 |
| 第 3、4 件事 | 同上 | 第六章对照表 |
| 内德渡过提篮的老兵 | `ch3_ned_first` | 第七章 L3 |
| 露西的瓶子在药房 | `ch3_lucy_first` | 第六章化验 |
| 威尔的债被人买走，每月有人往账上放钱 | `ch3_will_first` | 第五章赫尔的信 |
| 基蒂认识伯利恒看护 | `ch3_kitty_first` | 第四章捷径 |
| 看守霍奇旁注 | `ch3_climax` | 第四章捷径 |
| 第十一处另有人办 | `ch3_griggs_first` | 第六章 |
| 第十二只瓶，格里格斯喝干了 | `ch3_griggs_first` | 第七章账本 |
| 手上有墨的往年访视人 | `ch3_gale_first` 回忆第一章跳蚤的话 | 本章 |

## 轻场景与十一日

十一日：开场账房算工钱、生日被议会吃了；跳蚤卖九月的日子（本章蠢事）；威尔算利息。轻场景穿插在重场景之间：`ch3_flea_tickets` 隔开风暴夜和后面的岗亭，`ch3_kitty_warn` 隔开岗亭哭和风暴夜。

## 史实出处

`docs/research/history-notes.md`：第 1 节（法案第 1、4、6 条，复数“自然日”，荷兰用新式，双年写法）；第 2 节（周账 8 月 18 日 297，天花 43）；第 4 节（亨利拄拐，约翰，韦尔奇，科文特花园杂志）；第 10 节（8 月 22 日宣告集市，伯利恒在摩尔菲尔德，弗利特市场盖着河）；第 13 节（8 月 25、26 日风暴）；第 14 节（鹿特丹到伦敦约 4 至 8 天）。虚构：长存会、十处单子、人物普拉特太太、奥克斯太太、特纳太太、看守霍奇、童谣。
