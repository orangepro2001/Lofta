# 第六章 老贝利（章节脚本）

1752 年 10 月 2 日至 12 月 11 日。主问题：谁是赫尔？新动词：按来源看牌、在记录里划线。新历法知识：岁首与反查年份。章末落点：岗亭里的字模。

## 日期与时段预算

16 个时段，分三幕。自由时段 12 个，强制场景花掉 4 个。

| 时段 | 幕 | 事件 |
| --- | --- | --- |
| 0–3 | 一 | 十月，自由 4 格。1 处跳蚤卖门票；2 处开庭预告 |
| 4 | 一 | 强制串：庭审或晚餐（+1）、弓街询问（+1）、水手、绞刑、断交、面包、解誓 |
| 6–8 | 二 | 十一月，自由 3 格。7 处门被刷净、周四夜预告；8 处鹿特丹的纸 |
| 9 | 二 | 强制串：周四夜（+1）、缝针、次晨的消息 |
| 10 | 二 | 自由 1 格 |
| 11–13 | 三 | 十二月，自由 3 格。11 处律师量坟 |
| 14 | 三 | 强制：爵士受审（+1）、街头小调 |
| 15 | 三 | 自由 1 格 |
| 16 | 三 | 绞刑、高潮岗亭 |

日期：10 月 2 日星期一；10 月 30 日星期一；10 月 31 日星期二；11 月 1 日星期三；11 月 3 日星期五；11 月 20 日星期一；11 月 23 日星期四；12 月 1 日星期五；12 月 6 日星期三；12 月 8 日星期五；12 月 11 日星期一。

必需线：强制 4 格，加开棺和摊牌 2 格，共 6 格，占 38%。限时内容：第一幕翻案准备最多 2 格，周四夜前的韦尔奇 1 格，共 3 格，占 19%，不含必需结论。不可逆限时事件只有周四夜一个，时段 7 预告，时段 9 发生。开棺不算限时：律师拿不到特许状，爵士定罪后她仍可在第 15 格去挖。

全部看完约要 15 格以上，自由时段只有 12 格，这一章看不完。

## 场景表

| 节点 | 地点 | 人物 | 目的 | 时段 |
| --- | --- | --- | --- | --- |
| `ch6_open` | 教堂巷 | 以利亚、玛德莱娜 | 粉笔数、科布开口、写信去鹿特丹 | 0 |
| `ch6_flea_penny` | 教堂巷 | 跳蚤 | 轻。卖门票，本章的蠢事 | 0，强制 |
| `ch6_trial_near` | 教堂巷 | 玛德莱娜 | 预告开庭 | 0，强制 |
| `ch6_newgate` | 纽盖特 | 佩雷拉 | 翻案准备 | 1 |
| `ch6_ship` | 老天鹅或斯皮塔佛德 | 内德或玛德莱娜 | 翻案第二条路 | 1 |
| `ch6_trial` 及 `one/two/three` | 老贝利 | 爵士的律师、记录官 | 3 轮对质，容错 2 次：布道词、星期不跳、无毒酒 | 1，强制 |
| `ch6_trial_nobody` | 老贝利 | | 一次没去看过他，没人传她 | 同上 |
| `ch6_win_dinner` | 贝维斯马克斯 | 佩雷拉 | 轻。已洗清的路线 | 1，强制 |
| `ch6_exam` 串 | 弓街 | 爵士的律师、菲尔丁兄弟 | 盘问，第一章的账翻出来，赛跑开始 | 1，强制 |
| `ch6_sailor` | 弓街 | 水手、亨利 | 轻。十一日事件 | 0，强制 |
| `ch6_tyburn` | 圣吉尔斯、外科医师公会 | 以利亚 | 佩雷拉绞死 | 0，强制 |
| `ch6_break` | 外科医师公会门口 | 玛德莱娜 | 决裂 | 0，强制 |
| `ch6_bread` | 教堂巷 | 跳蚤 | 轻 | 0，强制 |
| `ch6_struck_off` | 法衣室 | 教区执事、皮姆 | 解誓，回洗衣盆 | 0，强制 |
| `ch6_door_clean` | 教堂巷 | 跳蚤 | 不可跳过的小事：门被刷净 | 0，强制 |
| `ch6_warning` | 教堂巷 | 跳蚤、以利亚 | 预告周四夜 | 0，强制 |
| `ch6_rotterdam` | 教堂巷 | 玛德莱娜或以利亚 | 收下那张纸，不解读 | 0，强制 |
| `ch6_chelsea` | 切尔西 | 斯隆爵士 | 奥姆的名单，疤，十一天 | 1 |
| `ch6_door` | 教堂巷 | | 那扇门 | 0，配方触发 |
| `ch6_bethlem` | 伯利恒 | 药房伙计、露西 | 取回并化验瓶子 | 1 |
| `ch6_cared` | 教堂巷 | | 第十一处没有瓶子 | 0，配方触发 |
| `ch6_g_line` `ch6_holt` | 红十字街 | 霍尔特 | 沿缺尾的 g 找人 | 1 或 2 |
| `ch6_gale_read` | 她的房间 | | 札记：同一只手 | 0 |
| `ch6_will` `ch6_ned` `ch6_kitty` | 弗利特、老天鹅、来信 | 活下来的命 | 写人；威尔的信 | 1、1、0 |
| `ch6_lyle_visit` | 纽盖特 | 莱尔爵士 | 6 月的印刷信 | 1 |
| `ch6_stone` | 墓地墙根 | | 反字的墓石 | 0 |
| `ch6_the_night` | 教堂巷 | 跳蚤、以利亚 | 砖，三选一 | 1，强制 |
| `ch6_go_crane` | 德鲁里巷 | 克兰、韦尔奇 | 抢在调包之前 | 同上 |
| `ch6_pym_talk` | 法衣室 | 皮姆 | 烟道；他说出的是他的沉默 | 同上 |
| `ch6_stay` | 教堂巷 | 跳蚤 | 留下 | 同上 |
| `ch6_stitch` | 教堂巷 | 跳蚤、以利亚 | 轻 | 0，强制 |
| `ch6_after_night` | 德鲁里巷、法衣室 | 韦尔奇、司事 | 克兰、皮姆的下场 | 0，强制 |
| `ch6_dec_open` | 教堂巷 | 韦尔奇、以利亚 | 律师量坟 | 0，强制 |
| `ch6_coffin` 及 `ch6_coffin_hand` | 圣吉尔斯墓地 | 以利亚、跳蚤、老塔克 | 开棺 | 1 |
| `ch6_after_coffin` | 教堂巷 | 跳蚤、以利亚 | 轻 | 0，强制 |
| `ch6_workhouse` | 济贫院、墓地 | 女总管、司事 | 哈里森 | 0 |
| `ch6_note_again` | 她的房间 | | 倒查年份 | 0 |
| `ch6_alive_beat` | 她的房间 | | 他没有死 | 0，配方触发 |
| `ch6_table` | 她的房间 | 玛德莱娜或以利亚 | 按来源看牌，排除皮姆 | 1 |
| `ch6_pym` 选项 / `ch6_pym_ledger` | 法衣室 | 皮姆或账本 | 他没说过 | 0 |
| `ch6_grey` | 她的房间 | | 七张牌待证 | 0，配方触发 |
| `ch6_strike` | 她的房间 | | 在记录里划线 | 0 |
| `ch6_knows` | 教堂巷 | 老塔克 | 她追，灯笼也走 | 0，配方触发 |
| `ch6_lyle_trial` 串 | 老贝利 | 科布、爵士的律师、记录官、陪审员、爵士 | 三天庭审 | 1，强制 |
| `ch6_ballad` | 教堂巷 | 跳蚤 | 轻 | 0，强制 |
| `ch6_pym_tells` | 法衣室 | 皮姆 | `pym_spoke` 且没有 `c6_alive` 时他说完那半句，12 月 9 日 | 0，强制 |
| `ch6_lyle_hangs` | 圣吉尔斯 | 跳蚤 | 爵士绞死解剖 | 0，强制 |
| `ch6_kiosk` `ch6_kiosk_hut` | 教堂巷岗亭 | | 高潮。她把手托包进围裙，第七章开场读小条 | 0 |

可反复追问的菜单：`ch6_elias`、`ch6_madeleine`、`ch6_flea`、`ch6_tuck`、`ch6_john`、`ch6_pym`、`ch6_rudd`、`ch6_keane`、`ch6_books`、`ch6_pereira`。方向由 `ch6_direction_e`、`ch6_direction_m` 给，花 1 时段，来源干净。

## 卡片表

16 张新卡，加 1 张谬误。

| id | 种类 | tags | 内容 | 得到 |
| --- | --- | --- | --- | --- |
| `c6_orme_list` | fact | orme_list | 奥姆的名单，页头“1721 年 3 月” | `ch6_chelsea` |
| `c6_orme_1722` | inference | inoculated | 名单是 1722 年的，她种过痘 | `d6_year` |
| `c6_bottle_clean` | fact | bottle_clean | 露西的瓶子没有白渣 | `ch6_bethlem` |
| `c6_knows_17` | inference | knows_17 | 发瓶子的人知道她是谁，还在乎她 | `d6_eleventh` |
| `c6_hale_thomas` | inference | hale_thomas | 赫尔就是托马斯 | `d6_hale` 或 `ch6_pym_talk` |
| `c6_coffin` | fact | coffin | 棺内的人左手缺两指 | `ch6_coffin` |
| `c6_harrison` | fact | no_burial | 哈里森，葬册查无此人 | `ch6_workhouse` |
| `c6_note_year` | fact | no_burial | 便条写于 1752 年 3 月 4 日 | `ch6_note_again` |
| `c6_alive` | inference | alive | 托马斯没有躺在那口棺材里 | `d6_alive` |
| `c6_denied` | testimony | denied | 皮姆从没说过 | `ch6_pym` 或 `ch6_pym_ledger` |
| `c6_sources` | inference | inside | 一个守夜的怎么知道这些 | `r6_source` |
| `c6_typeline` | fact | reads_type | “这个字排错了” | `ch6_strike` |
| `c6_sleep` | fact | sleep_line | “睡吧，钟点替你醒着。” | `ch6_strike` |
| `c6_cobb_depo` | testimony | cobb_depo | 科布的供词 | `ch6_john` 或 `ch6_lyle_trial` |
| `c6_rotterdam` | fact | rotterdam | 鹿特丹的纸 | `ch6_rotterdam`，强制 |
| `c6_tuck` | inference | tuck_is_thomas | 老塔克就是托马斯 | `d6_tuck`，只有 `tuck_state` 为 3 时才有 |
| `c6_all_false` | fallacy | | 老塔克的话全不算数 | `f6_all_false` |

必需结论只有 `c6_rotterdam`。其余都可以缺，缺了只改变 `tuck_state` 和第七章的证据链。

## 配方表

| id | 动词 | 输入 | 输出 | 逻辑形式 |
| --- | --- | --- | --- | --- |
| `d6_year` | 演绎 | `c3_rule_dual` × orme_list | `c6_orme_1722` | 三月二十五前的年份属上一年；此页写 3 月、未注日在二十五日之后，试验在 1721 年 8 月；所以此页是 1722 年 |
| `d6_eleventh` | 演绎 | `c6_bottle_clean` × ten_list | `c6_knows_17` | 持有人不会毒自己的命；赫尔的瓶子无毒，第十一处无瓶也无地址之累；所以发瓶子的人是赫尔一方，知道 17 号是谁 |
| `d6_hale` | 演绎 | `c6_knows_17` × broken_g，需 `ch6_hand` | `c6_hale_thomas` | 知道她的只有皮姆和托马斯；赫尔的手或字指向托马斯；所以赫尔是托马斯 |
| `d6_alive` | 演绎 | `c6_coffin` × no_burial，退 `c2_note` | `c6_alive` | 棺内的人不是托马斯；此人另有名字或便条写于下葬之后；所以托马斯没有躺在里面 |
| `r6_source` | 反驳 | 靶 `c2_lefevre_book`，反例 denied，退 7 张 | `c6_sources` | 他说消息来自皮姆酒后；皮姆没说过；所以这些话另有来处 |
| `d6_tuck` | 演绎 | `c6_sources` × sleep_line，需 `c6_alive`、`c6_typeline` | `c6_tuck` | 守夜人是内部的人，读得出反字，说过只有托马斯说过的话；托马斯没死；皮姆、霍尔特已排除；所以他是托马斯 |
| `f6_all_false` | 谬误，诉诸来源 | `c6_sources` × cobb_knight | `c6_all_false` | 此人藏着；所以他说的都不算数。不能因来源否定结论，只能因来源要求佐证 |
| `d6_relight_cobb` | 演绎，换前提 | `c3_lyle` × cobb_depo，需 `made:r6_source` | 亮回 4 张 | 灰外套、科布跑腿、爵士雇人、律师盯九月二日，改由科布宣誓作证 |
| `d6_relight_book` | 演绎，换前提 | `c2_lefevre_book` × society，需 `made:r6_source` | 亮回 3 张 | 账、萨姆的股、吵架，改由长存会自己的记录和玛德莱娜作证 |

按来源：`r6_source` 一次变灰 7 张（六件真事加 `c3_lyle`），其中必需结论不超过 1 个。便条这个来源由 `d6_alive` 退掉 `c2_note`，替代牌是 `c6_note_year`。`retires` 设 `unproved_` 标记，亮回用 `effects` 的 `clear`。

## 本章读取和设置的标记

读取：`ch1_took_groat` `ch1_bill_true`（其余取值走 else）`ch1_blamed_keane` `bowell_soothed` `hannah_stone_told` `kitty_alive` `ned_alive` `lucy_alive` `will_alive` `gale_book` `have_almanac` `pereira_cleared` `pereira_committed`。

设置：`pereira_cleared` `pereira_hanged` `lyle_guilty` `crane_taken` `crane_dead` `pym_spoke` `pym_dead` `holt_found` `bottles_tested` `exhumed` `rotterdam_paper` `have_cordial` `hannah_stone` `ch6_done`，变量 `tuck_state`。

本章内部：`ch6_act2` `ch6_act3` `ch6_hand` `ch6_prep_newgate` `ch6_prep_ship` `ch6_type_line` `ch6_holt_named` `ch6_broken` `ch6_warned` `ch6_welch_watch` `ch6_stayed` `ch6_with_m` `ch6_said_alive` `ch6_evaded`，变量 `trial_miss`。

`pereira_hanged` 在 `ch6_exam_answer` 里临时设置再清除，只为让 `~ fund` 算出“佩雷拉绞死时她能拿多少”。这段窗口里只有露丝说话。

## 分支后果

- **翻案。** 一格准备都没花：没人传她，直接定罪。花了：3 轮对质，错到第 3 次败。纽盖特那一格补 `c5_sermon`，还可当庭烧酒；海上证人那一格是第一轮的第二条路（`ned_alive` 用船主约伯，否则用荷兰来信）。第二轮 `c5_no_skip` 或《历书》，第三轮 `c5_clean_wine` 或 `c5_wine_prop`。胜 `pereira_cleared`，败 `pereira_hanged`，败后 `玛德莱娜` 菜单消失，方向全由以利亚给。
- **盘问。** `ch1_took_groat`：当场打垮，设 `ch6_broken`，解誓时再提一次。`ch1_bill_true`：能答，亨利替她说一句。其余：不痛不痒。`pereira_cleared`：可以算账反驳；基蒂、内德都死时，差额为零，她改说“我一便士不差”。`gale_book` 为假：札记在律师手里。
- **周四夜。** 三选一，时段 7 预告。韦尔奇预先盯住克兰（弓街 1 格）才可能两全。四种组合都可达：克兰+皮姆（花了韦尔奇那格）、只克兰、只皮姆、都没有（留下陪跳蚤）。第七章结局 C1 需要后者之一。
- **开棺。** 第 14 格之前是抢先，之后是律师撤了。`bowell_soothed`：直接看手；否则先看衣服或牙。
- **老贝利作证。** 手里有 `c6_coffin` 时她有选择：挑过的真话（`ch6_evaded`），或说出棺材（`ch6_said_alive`，全场笑她，为结局 F1 铺垫）。
- **tuck_state。** 3 需 `c6_tuck`；2 需 `c6_alive`；否则 1，`pym_spoke` 时皮姆 12 月 9 日告诉她，她回家，夜里钟点照常（第七章开场这样写）。都走到岗亭。
- **蓝瓶。** 玩家选择拿不拿，设 `have_cordial`。

## 轻场景

卖门票、水手的孩子（十一日事件）、佩雷拉的刀叉、面包、缝针、铁锹的职业病、小调“死人先生一天多少钱”。斯隆的“十一天很贵”。重场景之间都插了一个，见 `content/story/ch6.toml` 的强制顺序。

## 线索回收

| 线索 | 埋设 | 回收 |
| --- | --- | --- |
| 十根手指 | 第 1 章验尸 | `ch6_coffin_hand` |
| “我没看他的手” | 第 2 章 | `ch6_coffin` |
| 便条的星期 | 第 2、3 章 | `ch6_note_again` |
| “这个字排错了” | 第 2 章 | `ch6_strike` |
| “睡吧，钟点替你醒着。” | 第 3 章 | `ch6_strike`；`ch6_stay` 她差点说出后半句 |
| 拇指敲提环 | 第 1 章 | `ch6_strike` |
| 十处单子 | 第 3 章 | `ch6_cared` |
| 内德的信丢了 | 第 4 章 | `ch6_table` |
| 老塔克打断科布的手臂 | 第 4 章 | `ch6_lyle_trial` 科布出门时的话 |
| 皮姆“笔停了” | 第 2 章 | `ch6_pym_talk`、`ch6_pym` |
| 鼻尖碰着纸 | 第 1 章 | `ch6_pym_talk`、`ch6_after_night` |
| 凉茶 | 第 1 章 | `ch6_struck_off`、`ch6_after_night` |
| 老塔克没有喊破开棺 | 本章 | `ch6_tuck`、第七章可用 |
| 鹿特丹的纸 | 第 1、5 章 | 本章只收下，第七章解读 |
| 反字的墓石 | 第 2 章 | `ch6_stone`（H、A、N、N、A、H，与 `ch7_wall` 一致），第七章结局 E |
| 门房“您那一封也在里头” | 第 5 章 | `ch6_strike` |
| 校正告示 | 第 5 章 | `ch6_strike` |
| 靠墙的铺位 | 第 5 章 | `ch6_will` |
| “谁来看着这些看守” | 第 5 章 | `ch6_grey` |
| 手托和小条 | 本章 | 第七章开场 |
| 蓝瓶 | 第 1 章 | `ch6_kiosk_hut`，第七章结局 D |

“私下怀疑”的回放没有接口标记，留给界面开发。

## 史实及出处

出处都是 `docs/research/history-notes.md`。

- 老贝利 10 月庭 26、27、28、30 日；12 月庭 6、7、8 日，加斯科因任内第一庭（第 12 节）。
- 10 月 27 日“男爵”伯克受审（第 12 节）。
- 《谋杀法》判决后隔一日行刑，逢周日改周一；尸体送外科医师公会；未经解剖不得下葬（第 6 节）。
- 外科医师公会新会馆在老贝利（第 6 节）。
- 囚车在圣吉尔斯停车喝一碗麦酒（第 6 节，次级来源）。
- 12 月 11 日亚伯拉罕·沃德绞死，杀伊丽莎白·桑德斯（第 6 节）。爵士与他同车是虚构。
- 被告不宣誓，律师不向陪审团总结，没有“反对”（第 12 节）。
- 纽盖特风车，11 名木匠 7 人染牢狱热（第 9 节）。
- 《科文特花园杂志》11 月 25 日终刊（第 4 节）。
- 11 月才有致命热病（第 9 节）。10 月多霜、早晚大雾；12 月冷晴霜（第 13 节）。
- 国王 11 月 18 日回圣詹姆斯宫（第 14 节）。10 月 12 日下令拆摩尔菲尔德隔墙（第 13 节）。
- 皇家试验：纽盖特 1721 年 8 月，斯隆在场；公主 1722 年 4 月种痘（第 15 节）。
- 排字工读反字、手托（第 15 节）。
- 《老贝利庭审记录》4 便士（第 11 节）。

虚构：奥姆和他的名单；开棺特许状走主教法庭的程序；教区解除验尸婆誓言的决议；伦敦城里的酒馆名“碗”沿用笔记的 Bowl；红十字街的酒馆；炭气闷死皮姆；哈里森。
