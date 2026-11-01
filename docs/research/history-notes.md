# 1752 年 8 月至 1753 年 1 月伦敦：史料考证笔记

用途：中文叙事推理游戏编剧查用。主角为圣吉尔斯教区验尸婆（searcher of the dead），主线涉及《死亡周报》、1752 年历法改革、人寿存续基金。

## 0. 使用说明

### 可信度标记

- 【已证实】：我直接读过一手文本、扫描页或学术研究原文（法案全文、Old Bailey 审判记录、《绅士杂志》扫描、Graunt、Creighton、Poole、Old Bailey Online 与 London Lives 的学术背景页等）。
- 【次级】：只见于 Wikipedia、百科、博物馆页、学术书目摘要等次级来源，未回到一手。
- 【存疑】：来源互相矛盾、单一 OCR 无法确认，或只是流传说法。
- 【未查到】：本次没有找到可靠来源，不要当事实写。
- 【推算】：我根据已证实材料自己算出或推出的结论，不是原始史料的话。
- 【建议】：编剧建议，不是史实。

### 取证方式与局限

- Old Bailey Online 网站对脚本返回 403。审判、Ordinary 记录是通过其数据接口 `https://www.dhi.ac.uk/api/data/oldbailey_record_single?idkey=<编号>` 读取的。下文给的是人类可打开的 `oldbaileyonline.org/record/<编号>` 链接，内容一致，但这个链接格式我只从搜索结果确认过，未能逐条用脚本打开。
- Old Bailey Online 的背景页通过 Wayback 存档读取。
- 《绅士杂志》（Gentleman's Magazine，下称 GM）用 archive.org 的 OCR 通读，关键页看了扫描图。OCR 对数字有误读，凡标“扫描核对”的数字以图为准，其余 OCR 数字可能有个位误差。引文里长 s（ſ）已改为 s，其余拼写保留。
- 法案引文来自 [爱尔兰法令书全文](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html)，与 [legislation.gov.uk](https://www.legislation.gov.uk/apgb/Geo2/24/23) 的条文结构一致。
- 未能查阅：牛津国家传记大辞典（ODNB）、Beattie《The First English Detectives》全文、Clark《Betting on Lives》全文、Poole《Time's Alteration》全书、Survey of London 相关卷（第 10 节因此多用次级来源）。
- 1752 年 9 月 2 日以前的日期都是旧历（O.S.），星期按当时实际。9 月 14 日以后是新历（N.S.）。

---

## 1. 《1750 年历法（新式）法案》(24 Geo. II c.23) 与新旧历

### 可直接用的事实

**法案本身**

- 【次级】王室批准日 1751 年 5 月 22 日。([Wikipedia](https://en.wikipedia.org/wiki/Calendar_(New_Style)_Act_1750))
- 【已证实】法案没有单一“生效日”，有两个节点：1751 年 12 月 31 日之后停用 3 月 25 日岁首；1752 年 9 月 2 日之后跳日。([法案全文](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】新年规定原文：3 月 25 日岁首算法 “shall not be made use of from and after the last day of December one thousand seven hundred and fifty-one”（1751 年 12 月最后一天之后不得再用）。下一个 1 月 1 日即 1752 年第一天。([法案第 1 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【推算】因此 1751 年在英格兰只有 3 月 25 日至 12 月 31 日，共 282 天。1752 年是闰年，扣掉 11 天后实有 355 个自然日。
- 【已证实】跳日原文：“the natural day next immediately following the said second day of September shall be called, reckoned, and accounted to be the fourteenth day of September, omitting for that time only the eleven intermediate nominal days”（9 月 2 日的次一自然日称为 9 月 14 日，仅此一次略去其间 11 个名义日）。星期序列不跳，所以 9 月 2 日星期三接 9 月 14 日星期四。([法案第 1 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】一切契据文书从 1752 年 1 月 1 日起必须按新法写日期：“all acts, deeds, writings, notes, and other instruments ... shall bear date according to the said new method of supputation”（所有文书都须按新算法署日期）。([法案第 1 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】序言承认 1 月 1 日岁首早已是民间习惯，也是苏格兰法定算法：“from the legal method of computation in that part of Great Britain called Scotland, and from the common usage throughout the whole kingdom”（与苏格兰法定算法及全国通行习惯不同）。([法案序言](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))

**法院与集市：两套规则，方向相反**

- 【已证实】普通法院、季审、教会法院、法人团体集会按“同一名义日”举行，也就是实际提前 11 天：“eleven days sooner than the respective days whereon the same are now holden and kept”（比现行日子早 11 天）。([法案第 1 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】集市（包括雇工集市）及其附属法庭、苏格兰最高民事法院等按“同一自然日”：“upon or according to the same natural days ... that is to say, eleven days later”（在同一自然日，即名义上晚 11 天）。条文明确写到 “for the hiring of servants”（雇佣仆人）。([法案第 4 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】公地开放和封闭也按自然日，名义上晚 11 天。([法案第 5 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))

**财产保全（第 6 条，与基金主线最相关）**

- 【已证实】不得提前任何租金、年金或款项的支付：“to accelerate or anticipate the time of payment of any rent or rents, annuity or annuities, or sum or sums of money”。适用范围是 1752 年 9 月 14 日之前已订立的 “custom, usage, lease, deed, writing, bond, note, contract, or other agreement”，以及依议会法案应付的款项。([法案第 6 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】到期日原文：“at and upon the same respective natural days and times as the same should and ought to have been payable ... in case this Act had not been made”（在若无本法本应支付的同一自然日与时间）。([法案第 6 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】利息只按真实天数计：“for the true number of natural days for which the principal sum bearing such interest shall continue due and unpaid”（按本金实际欠付的真实自然日数）。([法案第 6 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】成年与学徒：任何人须满足 “the full number of years and days”（足额年日）才算满 21 岁或完成服务期；适用于 9 月 14 日前出生者和 “any apprenticeship or other service”（学徒或其他服役）。([法案第 6 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】租约的起止、土地交还同样按自然日。([法案第 6 条](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html))
- 【已证实】纠正一个常见引法：法案正文用的是复数 “same natural days” 和 “same respective natural days and times”，没有单数 “same natural day”。全文也没有 “bill of exchange” 一词，只有 “bond, note, contract, or other agreement”。Poole 的研究把效力解释为及于 “rents, wages and bills”，所以汇票属于学者解释。([法案全文](https://www.irishstatutebook.ie/1750/en/act/pub/0023/print.html)；[Poole 1995, p.114 一带](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))

**修补立法**

- 【已证实】原法没规定本应落在被删 11 天内的事项，修正案 25 Geo. II c.30 规定 1752 年这些事项按旧历办理。([Poole 1995, p.114](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【已证实】修正案还处理了依附于移动节日的放牧地和租金，并调整伦敦市长宣誓。([25 Geo. II c.30](https://www.legislation.gov.uk/apgb/Geo2/25/30/data.htm))
- 【已证实】市长就职被改到 11 月 8 日宣誓。GM 记录 1752 年 11 月 8 日星期三 Crisp Gascoyne 在市政厅宣誓，9 日赴威斯敏斯特。([Poole 1995, p.115](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf)；[GM 1752 年 11 月](https://archive.org/details/sim_gentlemans-magazine_1752-11_22_11))
- 【已证实】24 Geo. II c.48 同时缩短了米迦勒节开庭期，起始日推后 10 天。([Poole 1995, p.115 注 70](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))

**荷兰**

- 【已证实】荷兰省和泽兰省早在 1582 至 1583 年之交改用格里历，共和国其余省份仍用儒略历，两者相差约十天，所以十七世纪文书常并列两种日期。鹿特丹属荷兰省，1752 年时用新历。([KNAW 历法页](https://dwc.knaw.nl/en/?p=293)；[Wikipedia](https://en.wikipedia.org/wiki/Adoption_of_the_Gregorian_calendar))
- 【存疑】荷兰省的精确切换日各来源不一：一说 1582 年 12 月 21 日后接 1583 年 1 月 1 日，一说 1 月 1 日=11 日。编剧只写“1583 年起”即可。([KNAW](https://dwc.knaw.nl/en/?p=293)；[Groniek 论文](https://ugp.rug.nl/groniek/article/download/27608/25021/32103))
- 【未查到】荷兰省何时固定以 1 月 1 日为岁首。

**1752 年后的新旧并存**

- 【已证实】国家账目继续以旧历季度日结算：“5 January, 5 April, 5 July and 10 October”。这就是现代英国 4 月 5/6 日财政年的来源，Poole 明确解释 3 月 25 日与 4 月 5 日恰好相差 11 天（3 月 25 日是旧财政季度的最后一天）。([Poole 1995, pp.117 及注 77](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【已证实】财政部秘书 James West 的原话：“this will make an odd jumble in Annuities and other payments at the Exchequer”（这会让财政部的年金和其他支付乱作一团）。他曾拟每镑扣 7 便士的折减表，后因违背法案放弃。([Poole 1995, p.117](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【已证实】一手印证：GM 记 1753 年 1 月 25 日南海公司宣布 2% 股息，“for the half-year ending the 5th inst.”（截至本月 5 日的半年）。1 月 5 日正是旧圣诞。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))
- 【已证实】GM 1752 年 8 月刊登读者建议：名义季度日照旧付款，按比例扣 11 天，并附仆人工资折减表；同文说名义季度日仍是 9 月 29 日，真实季度日要晚 11 天，即 10 月 10 日（这句 OCR 残缺，月份按上下文判断）。([GM 1752 年 8 月](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8)，扫描未逐字核对)
- 【已证实】GM 1752 年 9 月反驳这一方案：会迫使农民在收获时节换人换地。([GM 1752 年 9 月](https://archive.org/details/sim_gentlemans-magazine_1752-09_22_9))
- 【已证实】GM 1753 年 1 月目录有 “Table of abatement, or allowance for rent, wages, &c, in respect of the 11 days omitted in September 1752”，折减表确实流通。Poole 也说这类表 “widely published in the press”，尽管法案并不支持。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1)；[Poole 1995, p.117](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【已证实】GM 1752 年 8 月：报上大量广告通知集市名义日后移 11 天，如 29 日的集市改到 10 月 10 日。([GM 1752 年 8 月](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8))
- 【已证实】1753 年出版的 16 种历书里，旧历节日被提及 123 次，占可能次数 160 的 77%；1755 年降到 61%，之后回升。([Poole 1995](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【已证实】布里斯托尔 1753 年 1 月 6 日：“Yesterday being Old Christmas Day” 乡民普遍过节，集市几乎无人，少数赶集者把黄油抬到每磅 9 至 10 便士。伍斯特报告同类情形，当地人称 “Anti-Gregorians”（反格里历者），有人要求讲 “Old-Stile nativity sermon”（旧历圣诞布道）。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))
- 【已证实】贵格会的年度会议平稳接受新历，把一月从“第十一月”改称“第一月”。([Poole 1995, p.116](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【已证实】改革前的双年写法实例：Chesterfield 书信在 Poole 注释里写作 “28 Feb., 18 Mar. 1750-1 O.S.”。([Poole 1995 注 57](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【未查到】1752 年以后民间继续写 “1752/3” 这种双年份的具体实例。法律上已禁止，节日和季度日的沿用才是有证据的。

**“还我十一天”骚乱：误传**

- 【已证实】Poole 的结论：骚乱 “universally known but defy detection”（人人知道却查无实据）。公共档案局的骚乱证词沉默，当时报刊也沉默，“there is not a ripple”（连一点涟漪都没有）。([Poole 1995, p.101](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【已证实】传说的来源：霍加斯 1755 年选举版画《An Election Entertainment》里被撕破的标语（表现的是牛津郡选举，不是历法骚乱）；1820 年 Henry Hunt 案检察官 James Scarlett 的说法；Mahon 与 Lecky（1878）的史书扩散。([Poole 1995, pp.95–105](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【已证实】同样不要写“布里斯托尔骚乱最烈”，那是现代通俗说法，Poole 列为待破除的版本。([Poole 1995](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))

### 剧情可用的细节

- 【已证实】当时的玩笑：GM 1752 年 9 月转载的信件写道 “Have I slept away 11 days in 7 hours”（我是不是七小时睡掉了十一天）。([GM 1752 年 9 月](https://archive.org/details/sim_gentlemans-magazine_1752-09_22_9))
- 【已证实】GM 1752 年 9 月 p.431 编者启事：一位署名 Philalethes 的人寄来计算到 “22 places of fractions”（小数点后 22 位）的长文，指责新历法案原理有误，编者拒登。可作为数学怪人角色的原型。([GM 扫描 p.431](https://archive.org/download/sim_gentlemans-magazine_1752-09_22_9/page/n46_w1400.jpg))
- 【建议】基金的半年付款若是 1752 年 9 月 14 日前订约，法定到期日是旧历季度自然日。名义日与自然日之间的 11 天是最好的诡计窗口：被保生命（nominee）死在 9 月 29 日（新）与 10 月 10 日（新）之间时，到底算不算“到期日仍在世”？
- 【建议】9 月 14 日当天恰是老贝利开庭首日（见第 12 节），可用来制造“第一个新历日”的戏。

### 不能写的时代错误

- 不写 1752 年有民众暴动索要 11 天。不满可以写，形式是旧历过节、拒赶集、牢骚和小册子。
- 不写“工资被少发 11 天”或“人寿命被缩短”成为普遍恐慌。
- 不把“same natural day”当原文引用。
- 不写 1752 年 1 月至 3 月的文书合法地用 1751 年。
- 集市不要和法院搞反：法院名义日不变（实际提前），集市自然日不变（名义推后）。
- 荷兰来信是新历，1752 年 9 月 14 日前比伦敦日期早 11 天。写 9 月以前的信件时序要换算。

---

## 2. 验尸婆、周报与教区书记公会

### 可直接用的事实

**任命、身份、宣誓**

- 【已证实】Graunt 1662：“The Searchers hereupon (who are ancient Matrons, sworn to their Office) repair to the place where the dead Corps lies, and by view of the same, and by other enquiries, they examine by what Disease or Casualty the Corps died.”（验尸婆是宣过誓的老年妇女，到尸体所在处，凭查看和询问判断死因。）([Graunt, Hull 编 1899 重印](https://archive.org/details/economicwritings02pett))
- 【已证实】教区书记公会书记 John Bell 1665：验尸婆 “generally ancient women”，由 “the eminentest men of the Parish” 挑选，经查核后 “sworn to that Office by the Dean of the Arches, or some Justices of the Peace”（由教会上诉法院院长或治安法官主持宣誓）。([Bell《London's Remembrancer》](https://distantreader.org/stacks/earlyprint/txt/A27350.txt))
- 【已证实】同书誓言内容：“to view and search the bodies of all persons Deceasing within their Parish; and to give a true Account to the Clerk of what Diseases the Parties Die”（查验教区内一切死者遗体，并向书记如实报告死因）。([Bell](https://distantreader.org/stacks/earlyprint/txt/A27350.txt))
- 【已证实】最接近游戏年代的陈述见 1759 年 Birch 序言：“Every parish appoints a searcher, whose business it is to examine the corpse, and to report the distemper.” 他还说人选 “low capacity”（能力低下），但认为再聪明的人也只能转述家属愿意说的话。([Birch 1759](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【次级】验尸婆多为贫穷老妇，不兼任照护病人或接生。([Wikipedia](https://en.wikipedia.org/wiki/Searcher_of_the_dead))
- 【未查到】1752 年圣吉尔斯教区有几名验尸婆、叫什么、由谁宣誓。Birch 写 “a searcher”（一名），十七世纪材料多用复数，老贝利 1750 年记录里一次出现两人同行（见下）。两人一组更有史料支持，但不能算 St Giles 的定数。

**收费**

- 【已证实】Graunt 的贿赂原文：“the Old-women Searchers, after the mist of a Cup of Ale, and the bribe of a Two-groat fee, in stead of one given them, cannot tell whether this emaciation or leanness were from a Phthisis, or from an Hectick Fever, Atrophy, &c.”（验尸婆喝了一杯麦酒，又收了两格罗特而非一格罗特的贿赂，就分不清这消瘦是痨病、消耗热、萎缩还是别的。）这段的语境是掩盖梅毒死因。([Graunt p.356 一带](https://archive.org/details/economicwritings02pett))
- 【推算】格罗特 = 4 便士，所以 Graunt 暗示 1662 年常规 4 便士，贿赂价 8 便士。
- 【次级】伊丽莎白时代约每具 2 至 4 便士，各教区不同；十九世纪有每人索 1 先令的说法。([Wikipedia: Searcher](https://en.wikipedia.org/wiki/Searcher_of_the_dead)；[Wikipedia: Bills](https://en.wikipedia.org/wiki/Bills_of_mortality))
- 【未查到】1750 年代伦敦的具体收费标准。编剧可写“四便士上下”，但要知道这是从 1662 年外推的。

**工作流程**

- 【已证实】Graunt 1662 全流程：丧钟或向教堂司事订坟 → 司事通知验尸婆 → 验尸婆看尸并询问 → 报教区书记 → 书记 “every Tuesday night” 把一周葬礼和洗礼账交公会书记 → “On Wednesday the general Accompt is made up and printed, and on Thursday published and dispersed to the several Families who will pay four Shillings per Annum for them.”（星期三汇总付印，星期四发行，订户每年付四先令。）([Graunt](https://archive.org/details/economicwritings02pett))
- 【已证实】1752 年周账确实以星期二为截止日：GM 列出 8 月 4、11、18、25 日，9 月 1、19、26 日，10 月 3、10、17、24 日，都是星期二。([GM 8–10 月](https://archive.org/details/sim_gentlemans-magazine_1752-09_22_9))
- 【存疑】1752 年仍是星期四发行：只有 1662 年的直接证据，加上截止日未变，大概率如此。
- 【已证实】验尸婆会在老贝利作证。1745 年 Ann Landey：“I am one of the searchers. The prisoner desired me to search the body of the deceased ... it was bruised very barbarously”（我是验尸婆之一，被告请我查验，发现伤得很惨）。([t17450424-33](https://www.oldbaileyonline.org/record/t17450424-33))
- 【已证实】1750 年 “Mary Webb and Mary Arnold, the searchers, deposed they saw nothing upon the body but what was common with all other corpses”（两名验尸婆作证，遗体上没有异于常尸之处）。([t17500711-65](https://www.oldbaileyonline.org/record/t17500711-65))
- 【已证实】1730 年圣安德鲁·霍尔本教区验尸婆 Elizabeth Horton 作证：未见暴力痕迹，按 “Convulsion-Fits” 上报。([t17300704-41](https://www.oldbaileyonline.org/record/t17300704-41))
- 【已证实】1726 年案件中验尸婆细述尸体瘀伤。([t17260831-38](https://www.oldbaileyonline.org/record/t17260831-38))
- 【已证实】1735 年一案中，证人起疑后告诉邻居，邻居 “sent for the Searchers”，可见验尸婆可能是最早介入可疑死亡的人。([t17351015-5](https://www.oldbaileyonline.org/record/t17351015-5))；1735 年另一案验尸婆当场判断 “starved to death”。([t17351210-37](https://www.oldbaileyonline.org/record/t17351210-37))

**周报范围与圣吉尔斯**

- 【已证实】1752 年总账的分区：97 个城墙内教区（葬 1,699）、17 个城墙外教区（4,651）、22 个米德尔塞克斯和萨里外围教区（9,450）、10 个威斯敏斯特城和特区教区（4,685），合计 146 区，总数 20,485，四项相加吻合。([Birch 1759 重印 1752 总账](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【已证实】St Giles in the Fields 属 **22 个外围教区（Out-Parishes in Middlesex and Surrey）**。Wikipedia 记城墙外 16 区，1752 年一手账面是 17。([Birch 1759](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【已证实】GM 月账把这四区写作 “Within the walls / Without the walls / Mid. and Surry / City & Sub. West.”。([GM 扫描 p.431](https://archive.org/download/sim_gentlemans-magazine_1752-09_22_9/page/n46_w1400.jpg))
- 【已证实】圣吉尔斯教区逐年葬数（Birch 1759，OCR，序列连贯）：1749 年 1,157；1750 年 1,068；1751 年 912；**1752 年 884**；1753 年 951；1754 年 1,045；1755 年 1,008。St George Bloomsbury 另列，1752 年 353。([Birch 1759](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【已证实】对比：1752 年 St Martin in the Fields 940，St Paul Covent Garden 265，St James Westminster 992，St Luke Middlesex 1,088。([Birch 1759](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【已证实】Graunt 特别点名圣吉尔斯：死于 French Pox 的记录 “were returned by the Clerks of Saint Giles's and Saint Martin's in the Fields only”（只出自这两区书记），因为那里 “most of the vilest and most miserable Houses of Uncleanness” 集中，他推断只有 “hated persons, and such, whose very Noses were eaten off” 才会被如实上报。([Graunt p.356](https://archive.org/details/economicwritings02pett))

**1752 年数字**

- 【已证实】全年总账期间 **1751 年 12 月 10 日至 1752 年 12 月 12 日**：洗礼 15,308（男 7,868，女 7,440）；葬 20,485（男 10,306，女 10,179）；比上年减少 543。([GM 扫描](https://archive.org/download/sim_gentlemans-magazine_1752-12_22_12/page/n82_w1400.jpg)；[Birch 1759](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【已证实】天花 **3,538**，是 1721–1760 年最高，全世纪只被 1772 年（3,992）和 1796 年（3,548）超过。GM 的 OCR 误作 3528，扫描、Birch 和 Creighton 都是 3538。([Creighton 第 2 卷](https://www.gutenberg.org/ebooks/43671))
- 【已证实】主要死因（GM 扫描，与 Birch 互校）：Convulsions 5,431；Consumption 3,558；Small Pox 3,538；Fever 类 2,070；Aged 1,263；Teeth 968；Dropsy 680；Abortive and Stilborn 529；Asthma & Tissick 289；Mortification 199；Cough/Hooping-Cough 188；Apoplexy & Sudden 178；Childbed 156；Colic/Gripes 123；Jaundies 115；Measles 111；Headmouldshot/Horshoehead/Water in the Head 85；Palsy 80；Lunatick 69；French Pox 54；Cancer 53；Gout 40；Grief 4。([GM 扫描](https://archive.org/download/sim_gentlemans-magazine_1752-12_22_12/page/n82_w1400.jpg))
- 【已证实】事故与暴力（Casualties，账面合计 316）：Drowned 98；Killed by Falls and several other Accidents 53；Self-Murder 44；Overlaid 37；Found Dead 22；Executed 17；Excessive Drinking 10；Burnt 8；Broken Limbs 7；Murdered 6；Scalded 4；Bruised 3；Bit by a Mad Dog 2；Smothered 2；Bit by a Viper 1；Poisoned 1；Starved 1；Frighted 0。([GM 扫描](https://archive.org/download/sim_gentlemans-magazine_1752-12_22_12/page/n82_w1400.jpg)；[Birch 1759](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【存疑】GM 比 Birch 多列一项 “Stabbed 1”（扫描较淡）。这样 GM 的分项相加是 317，而它印的合计仍是 316；Birch 无此项，相加正好 316。两个版本差一具尸体，原因未知。
- 【已证实】年龄段：两岁以下 8,239；2–5 岁 2,225；5–10 岁 814；10–20 岁 660；20–30 岁 1,566；30–40 岁 1,823；40–50 岁 1,633；50–60 岁 1,348；60–70 岁 1,079；70–80 岁 738；80–90 岁 316；90–100 岁 37；另有 100 岁以上零星。([GM 扫描](https://archive.org/download/sim_gentlemans-magazine_1752-12_22_12/page/n82_w1400.jpg))
- 【已证实】周报不给各病种的死者年龄，年龄只在书记公会账簿里有记录。([Creighton](https://www.gutenberg.org/ebooks/43671))

**周报如何撞上历法改革（本笔记最重要的发现之一）**

- 【已证实】GM 1752 年 9 月账 “from Aug. 25. to Sept. 26”：周数据为 “Weekly Sept. 1. 328 / 19. 338 / 26. 335”，合计 1,001。男 523，女 478；洗礼 930；城内 67，城外 243，中萨 493，威斯敏斯特 198。**9 月 1 日的下一周直接记为 9 月 19 日**，账面相隔 18 天，实际仍是 7 个自然日。([GM 扫描 p.431](https://archive.org/download/sim_gentlemans-magazine_1752-09_22_9/page/n46_w1400.jpg))
- 【已证实】GM 总账脚注原文：“A gent. who signs P. hints to us that it ought to be noted that the bill for 1752 contains only 356 Days, i.e. from December 10, 1751 to December 12, 1752, and that making a proportionable allowance for the 9 Days wanting, we shall find the decrease to be but 24.”（署名 P. 的先生提示：1752 年总账只含 356 天，按比例补上缺的 9 天，减少数其实只有 24。）([GM 扫描](https://archive.org/download/sim_gentlemans-magazine_1752-12_22_12/page/n82_w1400.jpg))
- 【推算】从旧历 12 月 10 日到新历 12 月 12 日实为 357 个自然日，正好 51 周，比 P. 的 356 多一天。按 P. 的方法算出的补偿后差额约为 25，与他的 24 接近。
- 【已证实】同页诺里奇账注明 “(11 days shorter)”，减少 237，可见当时人意识到短年问题。([GM 扫描](https://archive.org/download/sim_gentlemans-magazine_1752-12_22_12/page/n82_w1400.jpg))
- 【推算】所以 1752 年账面“死亡减少 543”大半是日历假象，只有会算的人才看得出。

**公会与印刷**

- 【已证实】1625 年公会依高等委员会或星室法庭的决定取得在会馆设印刷机的权利，“a printer is assigned by the archbishop of Canterbury”（印工由坎特伯雷大主教指派），7 月 18 日开机。([Birch 1759 序言](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【已证实】公会 1611 年重新特许，前身为 1233 年的 Fraternity of St Nicholas（Birch 的说法）。([Birch 1759](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【次级】会馆先在 Bishopsgate 一带，后在 Wood Street，1666 年大火烧毁，重建于 Silver Street，一直用到 1940 年毁于轰炸。([Wikipedia](https://en.wikipedia.org/wiki/Worshipful_Company_of_Parish_Clerks))
- 【未查到】1752 年周报印工姓名；1750 年代周报订费（只有 1662 年 4 先令）。

### 剧情可用的细节

- 【已证实】Birch 1759 对死因报告的怀疑可直接做主题句：账面只是 “such an account, as the family and friends of the deceased would be pleased to give”（家属亲友愿意给的说法）。([Birch 1759](https://archive.org/details/bim_eighteenth-century_a-collection-of-the-year_1759))
- 【已证实】1752 年天花周数：3 月 3 日当周 64，6 月 2 日当周 125（最高），8 月 4 日 72，8 月 11 日 58，8 月 18 日 43，8 月 25 日 57。年底降到高峰的四分之一到五分之一。总死亡周数：8 月 316、350、297、371，合计 1,334；9 月 328、338、335。([Creighton](https://www.gutenberg.org/ebooks/43671)；[GM](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8))
- 【已证实】10 月 31 日当周 407，11 月 7 日 327，14 日 366，21 日 336，合计 1,436（OCR 可相加核对）。([GM 11 月](https://archive.org/details/sim_gentlemans-magazine_1752-11_22_11))
- 【已证实】Fothergill 记录 9 月和 10 月是 convulsions 高发月（婴儿腹泻），此时天花已近尾声。([Creighton](https://www.gutenberg.org/ebooks/43671))
- 【建议】验尸婆手里的“真实周账”和印刷周账之间，9 月 1 日到 19 日的跳号是天然线索。

### 不能写的时代错误

- 验尸婆不是医生，也不开死亡证明。1752 年没有民事死亡登记。
- 周报记的是葬礼，不是死亡。不葬在国教坟地者可能漏记（Birch 序言提到大量未入账葬礼）。
- 不要写 Searcher 有“女验尸官”的官职地位，她们是教区雇的底层老妇。
- 周报是分病种、分教区、分年龄的数字表。我看到的 GM 与 Birch 账面都没有死者姓名，不要写主角在周报上看到某人的名字。
- 不要把 Clerks 的会馆写成在 Bishopsgate。

---

## 3. 死因调查（coroner's inquest）

### 可直接用的事实

- 【已证实】可疑死亡在 48 小时内开调查，通常在本地酒馆（alehouse）、教区济贫院或死者所在屋内进行；在押犯死亡自动开庭。([London Lives](https://old.londonlives.org/static/IC.jsp))
- 【已证实】比例：只有略多于 1% 的死亡进入调查。样本：1763 年威斯敏斯特各教区周报死亡 3,926，调查 50 件。([London Lives](https://old.londonlives.org/static/IC.jsp))
- 【已证实】任命：威斯敏斯特验尸官由威斯敏斯特修道院主任牧师和教士团任命；米德尔塞克斯、伦敦城、萨瑟克由自由持有人选举，分别由米德尔塞克斯季审或市参事法庭确认。多有法律或医学背景，通常终身任职。米德尔塞克斯分东西两区。([London Lives](https://old.londonlives.org/static/IC.jsp))
- 【推算】圣吉尔斯在米德尔塞克斯，不在威斯敏斯特，所以由米德尔塞克斯验尸官管辖。东区还是西区【未查到】。
- 【已证实】验尸官发传票，令本地警役（constable）组陪审团。陪审员是有产户主，名单最多 24 人，几乎都是本教区居民；监狱调查半数是囚犯。印好的传票表格从十八世纪中叶出现。([London Lives](https://old.londonlives.org/static/IC.jsp))
- 【已证实】裁决文书是羊皮纸，用语含 chance medley（过失或自卫致死）、felo de se（自杀）、manslaughter、drowning、fever 等。([London Lives](https://old.londonlives.org/static/IC.jsp))
- 【已证实】“Visitation of God” 确有用例：1732 年老贝利记录陪审团在验尸官调查书上认定 Mary Vezey “dy'd (by the Visitation of God)”。([t17320114-12](https://www.oldbaileyonline.org/record/t17320114-12))
- 【已证实】费用：1752 年新法（GM 称 “25 Geo. II”）规定每次调查 “twenty shillings”，每英里 9 便士（狱中死亡除外），另保留亨利七世法的 13 先令 4 便士，超收算勒索，可免职；GM 称 “It took place at Midsummer last”（去年仲夏生效）。([GM 1752 年 9 月](https://archive.org/details/sim_gentlemans-magazine_1752-09_22_9))
- 【存疑】该法章号：通行写 25 Geo. II c.29，AIM25 写 “26 Geo II c29”。原文本次未读到（legislation.gov.uk 无此页）。([AIM25](https://aim25.com/cats/118/14296.htm))
- 【已证实】Umfreville：1761 年《Lex Coronatoria》作者 Edward Umfreville；老贝利最早点名是 1762 年 “E. Umfreville, Esq; coroner”；1783–84 年仍在任；卒于 1786 年。([t17620224-18](https://www.oldbaileyonline.org/record/t17620224-18)；[TNA](https://discovery-cdn.nationalarchives.gov.uk/details/r/N13890102)；[VIAF](https://viaf.org/viaf/51566395))
- 【未查到】1752 年米德尔塞克斯和威斯敏斯特验尸官的姓名。Umfreville 在 1752 年是否已在任无证据，不要写他在任。
- 【已证实】验尸官有权下令开膛：1745 年证人说 “I gave orders for the coroner to be sent for, and he ordered the body to be opened”。([t17450424-33](https://www.oldbaileyonline.org/record/t17450424-33))
- 【已证实】调查笔录会在审判中被用来质证：“Did you tell the same before the Coroner you do now?”([t17500117-18](https://www.oldbaileyonline.org/record/t17500117-18))

### 剧情可用的细节

- 【已证实】1750 年证人公开指控验尸官：“the Coroner would not hear me, I believe he was fee'd; he made a laughing at me”（OCR “see'd”，疑为 fee'd）。([t17500117-18](https://www.oldbaileyonline.org/record/t17500117-18))
- 【已证实】1752 年 5 月 Pestell 案：验尸官说看不到外伤，辩方问他是否曾认定自然死亡。([t17520514-23](https://www.oldbaileyonline.org/record/t17520514-23))
- 【已证实】1752 年 4 月 Mary Selby 案：验尸官令众人去看停在卫兵室的尸体，因尸体发臭，军士不让进，外科医生无法证明那就是被告丈夫的尸体，法庭认为死亡证明不足。([t17520408-48](https://www.oldbaileyonline.org/record/t17520408-48))
- 【已证实】《Lex Coronatoria》记诺桑普顿一案：一桩旧案：陪审团三次要判精神失常，验尸官是享有自杀者财产权的庄园管家，一再休庭，直到陪审团改判 felo de se，后查明他从遗产中得了 8 镑。([Umfreville 1761](https://archive.org/details/bim_eighteenth-century_lex-coronatoria-or-the_umfreville-edward_1761_1))
- 【已证实】GM 1752 年 10 月：有人在 Hounslow 开枪自杀，被说成拦路劫匪，后更正为 Turnagain Lane 的 Ship 店主，“had for some time been disorder'd in his senses”，“was buried the night before in St Sepulchre's church yard”。([GM 1752 年 10 月](https://archive.org/details/sim_gentlemans-magazine_1752-10_22_10))

### 不能写的时代错误

- 1752 年没有法医官，没有尸检常规。开膛由外科医生或药剂师做，且要验尸官下令。
- 验尸官不抓人，陪审团也不是职业人员。
- 不写 “Dr Umfreville” 或把他写成医生。
- 1752 年女性不当陪审员（陪审中唯一女性形式是判断孕否的 jury of matrons，见第 12 节）。

---

## 4. 弓街

### 可直接用的事实

- 【已证实】Bow Street 办公室由 Thomas De Veil 1739 年设立，1748 年他死后由 Henry 和 John Fielding 接手。([Old Bailey Online: Policing](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Policing.jsp))
- 【已证实】Henry Fielding 住在 Bow Street，法庭大概是住宅的底层房间，他要求把失窃通知送到 “at his House in Bow-Street”。([Jensen 1915 编《Covent-Garden Journal》导言](https://archive.org/details/coventgardenjour01fieluoft))
- 【未查到】“4 Bow Street” 门牌的同代证据。
- 【已证实】他的健康：1740 年代初起痛风，1749 年重病，1752 年 “still going about on crutches”（仍拄拐），自称老人（实际未满 46 岁）。另有近 Ealing 的住处。([Jensen 导言](https://archive.org/details/coventgardenjour01fieluoft))
- 【次级】还有哮喘和肝硬化，1754 年赴葡萄牙，10 月 8 日死于里斯本。([Wikipedia](https://en.wikipedia.org/wiki/Henry_Fielding))
- 【次级】John Fielding：1721–1780，1740 年失明，1750 年 2 月与兄同办 Universal Register Office，1750 年任兄助理，1751 年 11 月 21 日结婚，1754 年接任。([Wikipedia](https://en.wikipedia.org/wiki/John_Fielding))
- 【存疑】John Fielding 何时任治安法官：Cambridge 书章摘要说 1751 年 1 月与 Welch 同列威斯敏斯特名册；Wikipedia 说 1754 年。([Cambridge](https://resolve.cambridge.org/core/books/henry-fielding/prison-gates-the-enquiry-and-amelia/CD6E3C32C5E779584B4F9F7D2E545F69))
- 【已证实】《科文特花园杂志》：1752 年 1 月 4 日创刊，星期二和星期六出版，到 7 月 4 日（第 53 号）后改周六，11 月 25 日终刊，共 72 号，定价 3 便士，Mrs Dodd 在 Temple Bar 的 Peacock 印售，总部在 Strand 对 Cecil Street 的 Universal Register Office，笔名 Sir Alexander Drawcansir。([Jensen 导言](https://archive.org/details/coventgardenjour01fieluoft))
- 【已证实】从第 2 号起有 Bow Street 法庭新闻栏，由 Fielding 和书记 Joshua Brogden 撰写。([Jensen 导言](https://archive.org/details/coventgardenjour01fieluoft))
- 【存疑】Jensen 称第 61 号（8 月 29 日）到第 62 号（“September 15th”）之间因历法只隔五天。9 月 15 日是星期五，与“周六刊”不合，可能应为 9 月 16 日，实隔 7 个自然日。未核原件。
- 【次级】Saunders Welch（1711–1784）：Holborn 高级警役 1746–1755，1755 年起任米德尔塞克斯治安法官，1754 年著《An Essay on the Office of Constable》。([Wikipedia](https://en.wikipedia.org/wiki/Saunders_Welch))
- 【已证实】Fielding 称他 “One of the best officers who was ever concerned in the execution of justice”。([Jensen 导言转引](https://archive.org/details/coventgardenjour01fieluoft))
- 【已证实】Fielding 的人是雇来的 thief-taker，有酬金（retainer）；官方头衔 Principal Officer，“Bow Street Runners” 这个名字他们本人不喜欢。([Old Bailey Online](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Policing.jsp))
- 【存疑】人数和年代：Wikipedia 说 1749 年六人；书评称 “finally came into being in 1754, there were only six of them”，政府补助 200 镑，后 400 镑（1758）和 600 镑（1765），酬金约每年 20 镑。([Wikipedia](https://en.wikipedia.org/wiki/Bow_Street_Runners)；[LRB 书评](https://www.lrb.co.uk/the-paper/v34/n11/john-barrell/something-for-theresa-may-to-think-about))
- 【已证实】称呼检索：老贝利数据里 1749–1760 年 “Fielding's people” 零命中；“Fielding's men” 最早 1756 年（[t17560528-32](https://www.oldbaileyonline.org/record/t17560528-32)），之后 1757、1758、1760 年都有；“Bow-street runner” 最早命中 1800 年（[t18000115-11](https://www.oldbaileyonline.org/record/t18000115-11)）。检索受转录拼写限制，不能证明绝对不存在。
- 【已证实】1752 年老贝利证词中常见的说法是 “before Mr. Fielding”“Justice Fielding”。([t17520914-4](https://www.oldbaileyonline.org/record/t17520914-4)；[t17521026-49](https://www.oldbaileyonline.org/record/t17521026-49))
- 【已证实】守夜：警役不负责侦查。守夜人从晚 9 或 10 点巡到日出，负责盘问可疑者。原由户主轮值，后渐成雇人代班，受 Watch Acts 推动，低薪低地位。([Old Bailey Online](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Policing.jsp))

### 剧情可用的细节

- 【已证实】Fielding 1752 年出版《Examples of the Interposition of Providence in the Detection and Punishment of Murder》。([Wikipedia](https://en.wikipedia.org/wiki/Henry_Fielding))
- 【已证实】1752 年 10 月强奸案里 Fielding 10 月 17 日问话。([t17521026-49](https://www.oldbaileyonline.org/record/t17521026-49))
- 【建议】若要一个能进 Bow Street 的联络人，用 Welch 或 Brogden 最稳。

### 不能写的时代错误

- 1752 年对白里不要出现 “Bow Street Runners”“detective”“police officer”“constable on patrol” 的现代含义。用 “Mr Fielding's man”“thief-taker”“constable”。
- 不写穿制服的弓街警官。
- 不写 John Fielding 1752 年已是 “Sir”，也不写 1752 年有 “Blind Beak” 的称号（称号【未查到】最早时间）。
- 不写 Henry Fielding 1752 年已去里斯本。
- Welch 1752 年还不是治安法官。

---

## 5. 玛丽·布兰迪案

### 可直接用的事实

- 【已证实】1752 年 3 月 2 日星期一大陪审团通过起诉；**3 月 3 日星期二**开审，牛津郡巡回审判，主审 Heneage Legge 和 Sir Sydney Stafford Smythe 两位财政法庭法官。扉页写 “On Saturday the 29th of February, 1752” 是巡回庭开幕日，不是审判日。([《The Tryal of Mary Blandy》1752](https://archive.org/details/tryalofmarybland00blan))
- 【未查到】审判所在的具体厅堂。常见说法是 Divinity School，本次未在一手中确认。
- 【次级】1752 年 4 月 6 日在牛津堡外绞死。([Wikipedia](https://en.wikipedia.org/wiki/Mary_Blandy))
- 【存疑】Wikipedia 称这天是 “Easter Monday”。【推算】1752 年旧历复活节是 3 月 29 日，所以 4 月 6 日不是复活节星期一，是其后一周。
- 【已证实】Addington 的症状证词：喝粥后 “an extraordinary Grittiness in his Mouth, attended with a very painful Burning and Pricking in his Tongue, Throat, Stomach and Bowels, and with Sickness and Gripings”（口中异常砂砾感，舌、喉、胃、肠灼痛刺痛，恶心绞痛），呕吐和腹泻后缓解。([Tryal](https://archive.org/details/tryalofmarybland00blan))
- 【已证实】尸检：胃和十二指肠内膜 “prodigiously inflamed and excoriated”（严重发炎破损），比作被大麦芒刷破流血的眼白；未见自然衰败。([Tryal](https://archive.org/details/tryalofmarybland00blan))
- 【已证实】Addington 的五点检验（[Tryal](https://archive.org/details/tryalofmarybland00blan)）：
  1. 乳白色；
  2. 砂砾感、几乎无味；
  3. 部分浮于冷水成淡硫色薄膜，大部沉底不溶；
  4. 投在 “red-hot iron”（烧红的铁）上不起火，升起浓白烟，“the Stench of Garlick”（蒜臭），冷铁悬其上结 “white Flowers”（白华）；
  5. 十格令煮于四盎司水，过滤分五份，分别加 Spirit of Sal Ammoniac、Lixivium of Tartar、Spirit of Vitriol、Spirit of Salt、Syrup of Violets，再与同法处理的真砒霜并排对比，结果 “an exact Similitude”。([Tryal](https://archive.org/details/tryalofmarybland00blan))
- 【已证实】对照用的白砒是 “bought of Mr. Wilcock, Druggist in Reading”，说明药商可买。Wilcock 在场称量。([Tryal](https://archive.org/details/tryalofmarybland00blan))
- 【已证实】纠正：蒜味检验用的是烧红的铁，不是炭火。炭火属于另一情节：Mary 把纸包丢进厨房炉火，仆人添了新炭，纸包被捡出，纸上写着 “clean the Pebbles”，里面有白粉。([Tryal](https://archive.org/details/tryalofmarybland00blan))
- 【已证实】毒药以 “Scotch Pebbles” 的清洗粉名义寄来。([Tryal](https://archive.org/details/tryalofmarybland00blan))
- 【已证实】Cranstoun 1751 年 12 月 2 日死于 Furnes（今比利时），死前皈依天主教，据 Tryal 卷首信件。([Tryal](https://archive.org/details/tryalofmarybland00blan))

**砒霜的可得性与名称**

- 【已证实】老贝利记录用语：1720 年 “White Arsenick”([t17200427-43](https://www.oldbaileyonline.org/record/t17200427-43))；1762 年 “white arsenic”，外科医生对比残渣([t17620526-18](https://www.oldbaileyonline.org/record/t17620526-18))；1770 年砒霜混入金酒([t17700530-48](https://www.oldbaileyonline.org/record/t17700530-48))；1776 年用于马丸([t17760522-42](https://www.oldbaileyonline.org/record/t17760522-42))。
- 【未查到】老贝利 1700–1800 年文本中 “ratsbane” 零命中，“rats bane” 词组 1720–1780 年零命中。作为普通英语词可用，但本次无 1750 年代实例。
- 【未查到】1752 年有无售毒登记要求的法令；本次没找到任何当时限制，但也未读到明确的“无限制”陈述。

### 剧情可用的细节

- 【已证实】Addington 的检验是把未知物与已知物并排对照，这是推理游戏的现成机制（颜色、口感、水中行为、灼烧气味、五种试剂）。([Tryal](https://archive.org/details/tryalofmarybland00blan))
- 【已证实】《Tryal》由 Rivington 在圣保罗教堂墓园印行，售 1 先令，扉页注明 “Published by permission of the Judges”（经法官许可出版），案情当年是公开读物。([Tryal](https://archive.org/details/tryalofmarybland00blan))

### 不能写的时代错误

- 不用 Marsh 试验（1836）或任何“能在人体组织里定性砷”的说法。Addington 检的是残粉，不是尸体组织。
- 不写蒜味来自尸体或呼吸。原文是灼烧粉末的烟味。
- 不写 “forensic”“toxicology”。

---

## 6. 《谋杀法》、解剖与绞刑

### 可直接用的事实

- 【已证实】生效：“from and after the first day of Easter term, in the year of our Lord one thousand seven hundred and fifty two”（1752 年复活节开庭期首日起）。不是 6 月 26 日。([法案文本](https://statutes.org.uk/site/the-statutes/eighteenth-century/1751-25-geo2-c37-murder-act/))
- 【推算】按 1752 年 3 月 29 日复活节，开庭期始于 4 月中旬，精确日期未核。
- 【已证实】期限：“on the day next but one after sentence passed, unless the same shall happen to be the Lord's day, commonly called Sunday, and in that case on the Monday following”（判决后第二天处决，逢周日改周一）。不是“两天内”，而是“隔一天”。([法案第 1 条](https://statutes.org.uk/site/the-statutes/eighteenth-century/1751-25-geo2-c37-murder-act/))
- 【已证实】有博客说逢星期五改星期六，法文无此规定。([法案文本](https://statutes.org.uk/site/the-statutes/eighteenth-century/1751-25-geo2-c37-murder-act/))
- 【已证实】尸体：伦敦和米德尔塞克斯案件 “immediately conveyed ... to the hall of the Surgeons Company”，“dissected and anatomized”。法官可改判吊尸：“to be hung in chains”。但 “in no case whatsoever the body of any murderer shall be suffered to be buried; unless after such body shall have been dissected and anatomized”（未经解剖不得下葬）。([第 2、5 条](https://statutes.org.uk/site/the-statutes/eighteenth-century/1751-25-geo2-c37-murder-act/))
- 【已证实】判决须当庭立即宣布，须写明行刑时间和羞辱标记；法官有合理理由可缓刑。([第 3、4 条](https://statutes.org.uk/site/the-statutes/eighteenth-century/1751-25-geo2-c37-murder-act/))
- 【已证实】判决后单独关押，非经批准只有狱卒可接触；“fed with bread and water only”（只给面包和水），病伤和圣餐例外；医生须登记姓名住址。([第 6、8 条](https://statutes.org.uk/site/the-statutes/eighteenth-century/1751-25-geo2-c37-murder-act/))
- 【已证实】武力抢囚犯或抢尸体均为死罪，不得享神职特权。([第 9、10 条](https://statutes.org.uk/site/the-statutes/eighteenth-century/1751-25-geo2-c37-murder-act/))
- 【已证实】首例：老贝利记录 “Note, This is the first case after the new act of parliament.” Thomas Wilford 杀妻 Sarah，5 月 25 日案发，判决后 “executed on Thursday the 2d of this instant, 48 hours after his receiving sentence”，尸体送 Surgeons Hall。([t17520625-31](https://www.oldbaileyonline.org/record/t17520625-31)；[o17520625-6](https://www.oldbaileyonline.org/record/o17520625-6)；[Ordinary OA17520702](https://www.oldbaileyonline.org/record/OA17520702))
- 【存疑】GM 1752 年 7 月说 Wilford “to be executed the next morning”，与法庭记录不合，以法庭记录为准。([GM 7 月](https://archive.org/details/sim_gentlemans-magazine_1752-07_22_7))
- 【次级】外科医师公会：1745 年与理发师分家。([RCS 档案](https://surgicat.rcseng.ac.uk/Details/archive/110004614))
- 【已证实】新会馆在 Old Bailey：GM 1752 年 11 月刊出 “an Elevation of the New Hall built by the Surgeons Company, in the Old Bailey”（版画）。([GM 11 月](https://archive.org/details/sim_gentlemans-magazine_1752-11_22_11))
- 【未查到】“Cyprian Theatre” 这个名称的可靠来源，不要用。
- 【次级】William Hunter：1746 年 10 月 13 日起在 Covent Garden 开课，收费四几尼，借 Samuel Sharp 外科学会的房间，Great Windmill Street 是 1760 年代后期的事。Covent Garden 迁址年有 1746 与 1749 两说。([Encyclopedia.com](https://www.encyclopedia.com/people/medicine/medicine-biographies/william-hunter)；[Archives Hub](https://archiveshub.jisc.ac.uk/data/ead-gb133-mmm%230000000%23MMM-23-1-7))
- 【次级】行刑路线：Newgate 到 Tyburn 约 3 英里，囚车可走到 3 小时；中途在 St Giles 的 Bowl Inn 停车，“St Giles's Bowl” 喝一碗麦酒。Tyburn 三角绞架到 1759 年才换。([Wikipedia](https://en.wikipedia.org/wiki/Tyburn,_London))

**1752 年下半年 Tyburn 行刑日（全部有一手）**

| 日期 | 星期 | 受刑者 | 备注 | 来源 |
|---|---|---|---|---|
| 1752-07-02 | 四 | Thomas Wilford | 新法首例，解剖 | [OA17520702](https://www.oldbaileyonline.org/record/OA17520702) |
| 1752-07-13 | 一 | 11 人 | | [OA17520713](https://www.oldbaileyonline.org/record/OA17520713) |
| 1752-09-22 | 五 | Randolph Branch、William Descent | 杀酿酒商书记 Joseph Brown，解剖 | [OA17520922](https://www.oldbaileyonline.org/record/OA17520922) |
| 1752-10-11 | 三 | Matthew Lee、John Wilks、Thomas Butler | | [OA17521011](https://www.oldbaileyonline.org/record/OA17521011) |
| 1752-11-13 | 一 | William Montgomery（一人） | 破产伪证罪 | [OA17521113](https://www.oldbaileyonline.org/record/OA17521113) |
| 1752-12-11 | 一 | Abraham Ward | 杀 Eliz. Saunders | [OA17530212](https://www.oldbaileyonline.org/record/OA17530212) |
| 1753-02-12 | 一 | 8 人 | 游戏结束后 | [OA17530212](https://www.oldbaileyonline.org/record/OA17530212) |

- 【已证实】表中没有 1752 年 8 月和 1753 年 1 月的 Tyburn 行刑（依 Ordinary 记录，GM 也无记载）。数据接口列出的 Ordinary 记录为 1752 年 1 月 13 日、3 月 23 日、4 月 27 日、6 月 1 日、7 月 2 日、7 月 13 日、9 月 22 日、10 月 11 日、11 月 13 日和 1753 年 2 月 12 日。([OB 数据](https://www.dhi.ac.uk/api/data/oldbailey_oa?year_gte=1752&year_lte=1753))
- 【推算】Ward 是周日条款的实例：12 月庭 6–8 日，星期五 8 日定罪判决，隔一天是星期日，改星期一 11 日。

### 剧情可用的细节

- 【已证实】Branch 和 Descent 的 Ordinary 记录写明 “on the second Day after Conviction, and receiving Sentence, they were to be executed, and their Bodies delivered to Surgeons-Hall to be dissected and anatomized”，他们在庭上还 “great Undauntedness”。([OA17520922](https://www.oldbaileyonline.org/record/OA17520922))
- 【已证实】GM 记 9 月 22 日 “their bodies were delivered to the surgeons, pursuant to the late act of parliament”。([GM 9 月](https://archive.org/details/sim_gentlemans-magazine_1752-09_22_9))
- 【已证实】抢尸争斗是立法背景之一。([Old Bailey Online: Punishment](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Punishment.jsp))
- 【建议】48 小时规则让被冤杀人犯没有翻案时间，时限压力现成。

### 不能写的时代错误

- 不写 1752 年有 Newgate 门前绞刑（1783 年才迁）。
- 不写 Tyburn 可移动绞架（1759 年后）。
- 不写谋杀犯可以下葬。
- 不写 Hunter 在 Great Windmill Street 上课。

---

## 7. 寿险与 tontine

### 可直接用的事实

**1693 国家 tontine**

- 【已证实】4 Will. & Mar. c.3（Million Act）：每年分配 10 万镑到 1700 年，之后 7 万镑；出资 100 镑得一份，在所提名生命（nominee）存续期内分红；“to be yearly distributed till An. 1700, and afterwards £70,000”。([法案](https://www.legislation.gov.uk/aep/WillandMar/4/3))
- 【已证实】监护人可为未满 12 岁幼儿出资，幼儿即 nominee。([第 10 条](https://www.legislation.gov.uk/aep/WillandMar/4/3))
- 【已证实】**存活证明原文（第 11 条）**：领取半年款时，除非 nominee 本人到财政部，“shall produce a Certificate of the Life of his or their respective Nominee signed by the Minister and Church wardens of the Parish where such Nominee shall be then liveing upon the day when the said Halfe yearely payment shall become due”（须出示由 nominee 所住教区牧师和教区委员签署的存活证明，证明其在到期日在世）；牧师和委员 “without fee or reward”（不得收费）；证明存档于财政部。立法目的：“that all Deceits ... may be prevented and that the full advantage of survivorship may be had”。([法案](https://www.legislation.gov.uk/aep/WillandMar/4/3))
- 【已证实】nominee 在苏格兰或海外时，由财政法院男爵 “in a summary way” 取证，认定 “it doth seem probable”（看来可能活着）即可。([第 12 条](https://www.legislation.gov.uk/aep/WillandMar/4/3))
- 【已证实】领取人逾期，到下一次付款前 20 天仍未申领，这一期就没收，分给其他人，“as if his respective Nominee had been dead”，下次仍可恢复。([第 13 条](https://www.legislation.gov.uk/aep/WillandMar/4/3))
- 【已证实】财政官员凭伪造证明付款免责，除非共谋（“Proviso for Officer paying Money upon forged Certificate, except privy”）。([第 17 条](https://www.legislation.gov.uk/aep/WillandMar/4/3))
- 【已证实】法案全文没有 “tontine” 一词。([Milevsky & Salisbury](https://ar5iv.labs.arxiv.org/html/1307.2824))
- 【已证实】认购远低于百万镑。1,013 名 nominee（1,081 份）留在 tontine，其余转 14% 终身年金。Halley 1693 年即指出 14% 终身年金对年轻人极划算。([Milevsky & Salisbury](https://ar5iv.labs.arxiv.org/html/1307.2824))
- 【已证实】最后存活者是 10 岁入选的女性，活到 100 岁，1783 年死于 Wimbledon，末年分红 1,081 镑。【推算】所以这个 tontine 在 1752 年还在付钱。([Milevsky & Salisbury](https://ar5iv.labs.arxiv.org/html/1307.2824))
- 【次级】怀疑作弊的说法：实际存活率远高于预期，后来有参与者抱怨并提到欺诈（我读到的是论文摘录和搜索摘要）。([Milevsky](https://arxiv.org/pdf/1307.2824))
- 【次级】替换儿童冒充 nominee 的手法是 Kitces 对研究的二手概括，只能算“当时的怀疑”。([Kitces](https://www.kitces.com/blog/tontine-agreement-instead-of-annuity-lifetime-income-mortality-credits-and-book-review-of-milevsky-king-williams-tontine/))
- 【次级】《London Gazette》第 13123 期的 tontine 规则（约 1789 年，摘要未显示日期）要求存活证明附宣誓书，海外证明由当地首席治安官出具。1808 年法案有防伪条款。时间都晚于游戏，只作类比。([Gazette 13123](https://thegazette.co.uk/London/issue/13123/page/552/data.pdf)；[1808 年法](https://legislation.gov.uk/cy/ukpga/Geo3/48/142/contents/enacted?view=plain))

**私人团体与人寿保险**

- 【未查到】1700–1750 年间英格兰具名的私人 tontine 实例。最接近的可靠实例是 1775 年共济会 tontine，“a life annuity with the benefit of survivorship”。([Clements](https://blog.history.ac.uk/2019/02/risk-or-reward-an-eighteenth-century-tontine/))
- 【存疑】ThinkAdvisor 称英格兰 1581 年左右已有私人存续安排。([ThinkAdvisor](https://thinkadvisor.com/2013/11/01/annuity-history-a-most-curious-william))
- 【次级】Amicable Society：1706 年 7 月 25 日成立，特许状；年费入共同基金，分给当年死者的受益人。不收 45 岁以上者，James Dodson 因此被拒并萌生 “equitable” 方案。([Wikipedia: Amicable](https://en.wikipedia.org/wiki/Amicable_Society_for_a_Perpetual_Assurance_Office)；[Wikipedia: Dodson](https://en.wikipedia.org/wiki/James_Dodson_(mathematician)))
- 【已证实】1774 年法序言（1752 年尚未立法）：“the making insurances on lives or other events wherein the assured shall have no interest hath introduced a mischievous kind of gaming”（对无利害关系者的生命投保，已成一种害人的赌博）。([14 Geo. 3 c.48](https://www.legislation.gov.uk/apgb/Geo3/14/48))
- 【存疑】王室批准日 Wikipedia 自相矛盾（1774 年 4 月 20 日与 5 月 20 日）。
- 【次级】有人给病重名人和被控死罪者投保。1761 年 Mortimer 说，病人看到报上自己的赔率会丧失希望。([Contingent Fee Blog](https://www.contingentfeeblog.com/2008/08/articles/corporate-owned-life-insurance/origin-of-the-insurable-interest-doctrine/))
- 【次级】海险已先走一步：1745 年法（19 Geo. 2 c.37）限制无利益保单。([Wikipedia](https://en.wikipedia.org/wiki/Marine_Insurance_Act_1745))
- 【未查到】赌沃波尔寿命的具体案例。

**公债与交易场所**

- 【已证实】Pelham 1749 年起减息：部分债先 3.5% 七年再 3%，次年余下者 3.5% 五年再 3%；**1751 年设立整合 3% 公债**；“Its price in 1752 was 106¾, the highest point it ever reached”（1752 年价格 106¾，是历史最高）。([Bastable《Public Finance》](https://www.econlib.org/book-chapters/chapter-bk-vch-iii/))
- 【已证实】1752 年议会又决议把 3.5% 年金并成银行可转让的联合股，办理期 “between October 10, 1752, and April 5, 1753”。两个日期都是旧历季度日。([GM 1752 年 9 月](https://archive.org/details/sim_gentlemans-magazine_1752-09_22_9))
- 【存疑】Wikipedia 把 1752 年写成“Consolidated 3.5%”，与 1751 年 3% 说法有混淆。按 Bastable 处理。
- 【已证实】南海公司 1753 年仍在分红（见第 1 节）。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))
- 【未查到】专论 1720 年后投资者心理的可靠材料（本次未单独查）。
- 【次级】Jonathan's：约 1680 年开在 Change Alley，1698 年 John Castaing 张贴行情，1748 年前后伦敦证券市场中心，1748 年毁于大火后重建，1761 年 150 名经纪人成会。([Wikipedia](https://en.wikipedia.org/wiki/Jonathan%27s_Coffee-House))
- 【次级】Lloyd's：1686 年 Tower Street 开业，1691 年 12 月迁 Lombard Street，1752 年时仍在此，靠航运消息和拍卖。([Wikipedia](https://en.wikipedia.org/wiki/Lloyd%27s_Coffee_House))

### 剧情可用的细节

- 【已证实】**Moravia 案（1752 年 6 月）**：Moses Moravia 和 John Manoury 被控合谋令船长沉 “Elizabeth and Martha” 号骗保，Solomon Carolina 无罪，Samuel Wilson 已死。([t17520625-51](https://www.oldbaileyonline.org/record/t17520625-51))
- 【已证实】判决：纽盖特监禁一年，期内到 Tower Hill 和 Royal Exchange 枷示，各罚 20 镑，具结 5 年。([GM 7 月](https://archive.org/details/sim_gentlemans-magazine_1752-07_22_7))
- 【已证实】1752 年 12 月 14 日星期四，二人第二次枷示，“severely pelted by the populace”。([GM 12 月](https://archive.org/details/sim_gentlemans-magazine_1752-12_22_12))
- 【已证实】GM 1752 年 10 月：伦敦城官职买卖有强制“保险”：买官者要按价值向 Chamber 投保，City Hunt 职位年交 60 镑，职位价 2,100 镑，继承人可从下任买家取回。死后财产来自官职的设计，“that the families of those who threw themselves out of trade ... should not be left destitute”。([GM 10 月](https://archive.org/details/sim_gentlemans-magazine_1752-10_22_10))
- 【建议】基金的存活证明可直接仿第 11 条：教区牧师和委员签字，日期落在到期日。验尸婆是唯一可能知道某 nominee 已死的人。她的上报与委员的签字互相矛盾，是核心证据。
- 【建议】nominee 住海外（鹿特丹）时只需“看来可能活着”，这是证据链的自然漏洞。
- 【建议】幼儿 nominee 的替换嫌疑可写成“当时的怀疑”。

### 不能写的时代错误

- 1752 年无 Equitable 保险社，它是 Dodson 死后才成立的（常说 1762 年，本次未复核年份）。不写 “actuary” 作为职业名（Dodson 时代的称呼【未查到】）。
- 1752 年赌他人寿命不违法，1774 年才立法。
- 不写 Stock Exchange 大楼。1752 年的证券交易在 Jonathan's 等咖啡馆和 Change Alley。
- 不写国家 tontine 由 Bank of England 管理，1693 年支付由财政部（Exchequer）办理。
- 不把 tontine 写成“最后一人全拿”。1693 年是把份额分给在世者，且有封顶。
- “tontine” 一词 1693 年法案未用，人物可说 “survivorship”。

---

## 8. 数学与统计

### 可直接用的事实

- 【次级】棣莫弗 1667 年生，1752 年 85 岁，1754 年 11 月 27 日死于伦敦；胡格诺难民；一生贫困，靠家教，常去 Old Slaughter's（St Martin's Lane，靠下棋赚钱）。([Wikipedia](https://en.wikipedia.org/wiki/Abraham_de_Moivre)；[MacTutor](https://mathshistory.st-andrews.ac.uk/Biographies/De_Moivre/))
- 【存疑】Bellhouse & Genest 译注的 Maty 传记里，1751 年一封信把他与 Pons 咖啡馆联系起来。我只见到搜索摘要，未读全文。([arXiv 0708.3965](https://arxiv.org/pdf/0708.3965))
- 【存疑】预言死期的传说（每晚多睡 15 分钟，算出死日）只是流传。Wikipedia 用 “common claim”，MacTutor 直接写成事实，1842 年 Britannica 说他每天要睡二十小时。([Wikipedia](https://en.wikipedia.org/wiki/Abraham_de_Moivre)；[Britannica 1842](https://jimclifford.ca/early_encyclopedia_britannica/articles/1842/eb_7th_1842_012638.html))
- 【次级】1752 年他还活着，死亡传说属 1754 年，不能入戏。([Wikipedia](https://en.wikipedia.org/wiki/Abraham_de_Moivre))
- 【次级】“替赌徒解题”只见于弱来源。“靠下棋和解棋题” 证据较好。([Wikipedia](https://en.wikipedia.org/wiki/Abraham_de_Moivre))
- 【已证实】《Annuities upon Lives》1725 年初版。([archive.org](https://archive.org/details/bim_eighteenth-century_annuities-upon-lives-or_moivre-abraham-de_1725))
- 【已证实】Halley 1693：《An Estimate of the Degrees of the Mortality of Mankind》，据 Breslau 表，在 Philosophical Transactions 发表，并算出 14% 终身年金的便宜。([archive.org](https://archive.org/details/philtrans05474358)；[Milevsky](https://ar5iv.labs.arxiv.org/html/1307.2824))
- 【次级】Thomas Simpson（1710–1761）：织工之子，自学；1743 年起在 Woolwich 教数学；《The Doctrine of Annuities and Reversions》简化棣莫弗。([Wikipedia](https://en.wikipedia.org/wiki/Thomas_Simpson))
- 【次级】他 1752 年还不是《Ladies' Diary》主编（Wikipedia 明言 1750 年代初 “not yet edited by Simpson”）。1752 年主编【未查到】。([Wikipedia: Simpson](https://en.wikipedia.org/wiki/Thomas_Simpson))
- 【次级】James Dodson（约 1705–1757）：棣莫弗学生，1742 年《Anti-Logarithmic Canon》，1747 年起《Mathematical Miscellany》，《Mathematical Repository》第 3 卷 1755 年；教师兼会计；死时 “in very mean circumstances”。([Wikipedia](https://en.wikipedia.org/wiki/James_Dodson_(mathematician)))
- 【次级】Macclesfield 1752 年任皇家学会会长，是 Dodson 的朋友，也是历法法案的技术主导。([Wikipedia](https://en.wikipedia.org/wiki/James_Dodson_(mathematician))；[Poole 1995](https://web.archive.org/web/2021/https://insight.cumbria.ac.uk/id/eprint/684/1/Poole_GiveUsOurEleven.pdf))
- 【次级】Spitalfields Mathematical Society：1717 年退役水手 Joseph Middleton 创办，最初在 Monmouth Street 的 Monmouth's Head，1725 年迁 Wheeler Street 的 White Horse，约 1735 年迁 Ben Johnson's Head。**Crispin Street 要到 1793 年**。([Wikipedia](https://en.wikipedia.org/wiki/Spitalfields_Mathematical_Society)；[MacTutor](https://mathshistory.st-andrews.ac.uk/Societies/Spitalfields/))
- 【次级】成员约一半是丝织工，其余是酿酒、铜匠、面包师、砖匠等；1735 年起上限 49 人（7 的平方）；规则之一：有会员提问，必须尽力讲解。Thomas Simpson、John Dollond、John Canton 是会员。([Wikipedia](https://en.wikipedia.org/wiki/Spitalfields_Mathematical_Society)；[Technical Education Matters](https://technicaleducationmatters.org/2009/05/14/the-spitalfields-mathematical-society-1717-to-1846/))
- 【次级】《Ladies' Diary》1704–1841 年，John Tipper 创办，Henry Beighton 1713 年接手，1720 年代就出微积分题；女性解谜题很强，Mary Wright 是有记载的女性解题者。([Wikipedia](https://en.wikipedia.org/wiki/The_Ladies%27_Diary)；[MAA](https://old.maa.org/press/periodicals/convergence/mathematical-treasure-ladies-diary-mathematical-problems))
- 【次级】培根《新工具》卷二第 11、12、13 格：Table of Essence and Presence；Table of Deviation, or of Absence in Proximity；Table of Degrees or Comparison。译名不一。([IntraText](https://www.intratext.com/IXT/ENG1110/EP.HTM))
- 【已证实】休谟原句：“all our experimental conclusions proceed upon the supposition that the future will be conformable to the past”（一切经验结论都建立在未来将合于过去这一假设上）。引自后来的定本，1748 年初版文字未逐字核对。([Gutenberg](https://www.gutenberg.org/ebooks/9662))
- 【已证实】书名陷阱：1748 年书名是《Philosophical Essays Concerning Human Understanding》，1758 年才改《An Enquiry...》。([davidhume.org](https://davidhume.org/texts/e/notes))

### 剧情可用的细节

- 【建议】三表法可做游戏机制：在场表（死者都有什么）、缺席表（相近但未死者缺什么）、程度表（严重程度随什么变）。这是培根的方法，用在周账上正好。
- 【建议】休谟的归纳问题配周账：未来的死亡率要合于过去，而 1752 年的账本身少了 9 天。

### 不能写的时代错误

- 人物不能说“人类理解研究”，1752 年叫 Philosophical Essays。
- 没有 “normal distribution”“statistics”“probability theory” 的现代用语（当时 “statistics” 用法【未查到】）。说 “doctrine of chances”“political arithmetic”。
- 1756 年版《Doctrine of Chances》是棣莫弗身后才出的，1752 年手里只能是更早的版本。
- 不写 Spitalfields 学会在 Crispin Street。
- 不写棣莫弗之死。

---

## 9. 医疗

### 可直接用的事实

- 【已证实】1752 年天花：Fothergill 说总体温和但极普遍，未得过的人少有幸免，一到三岁的孩子最重。([Creighton](https://www.gutenberg.org/ebooks/43671))
- 【已证实】1752 年 1 月 “A distinct benign kind of smallpox”，7 月转向凶险。([Creighton](https://www.gutenberg.org/ebooks/43671))
- 【已证实】1752 年基本无热病，到 11 月才有致命热病，延续到 1753 年 1、2 月；9 月有流行性卡他热；12 月流感。([Creighton](https://www.gutenberg.org/ebooks/43671))
- 【已证实】天花医院：1746 年 9 月 26 日开张。Islington 有病院，Old Street 的房子做接种，1750 年起附近 Frog Lane 的房子接收接种后的病人；**1752 年购入 Coldbath Fields 大楼，130 床**。到 1759 年 3 月共收 3,946 人，死 1,030 人。([Creighton](https://www.gutenberg.org/ebooks/43671))
- 【已证实】1752 年接种 112 人。入院需捐助人荐信，主要对象是刚从乡下来的家仆。流程是一月预备、约两周留院，每七周一批。([Creighton](https://www.gutenberg.org/ebooks/43671))
- 【已证实】1752 年 10 月 18 日 Edw. Paterson 遗赠天花医院 200 镑。([GM 10 月](https://archive.org/details/sim_gentlemans-magazine_1752-10_22_10))
- 【次级】人痘风险：Jurin 约 50 人死 1 人，自然天花约 7–8 人死 1 人。各来源数字有差别：1/48、1/50、最多 1/91。([Wikipedia](https://en.wikipedia.org/wiki/James_Jurin)；[James Lind Library](https://www.jameslindlibrary.org/articles/james-jurin-and-the-avoidance-of-bias-in-collecting-and-assessing-evidence-on-the-effects-of-variolation/))
- 【次级】Joshua Ward（1685–1761）：1734 年回伦敦，Pill and Drop 号称包治百病，1736 年 Clutton 分析称含锑、钴和砷。([Wikipedia](https://en.wikipedia.org/wiki/Joshua_Ward))
- 【次级】Daffy's Elixir：1647 年 Thomas Daffy 名下，十八世纪最流行成药之一。([Wikipedia](https://en.wikipedia.org/wiki/Daffy%27s_Elixir))
- 【次级】Foundling Hospital：1741 年 3 月 25 日在 Hatton Garden 开始收婴，“particular writing, or other distinguishing mark or token” 作辨认；新院在 Lamb's Conduit Fields。([Wikipedia](https://en.wikipedia.org/wiki/Foundling_Hospital))
- 【已证实】1752 年 11 月 11 日星期六 “Twenty children (out of 59) were admitted, by lot”（59 个孩子抽 20 个）。([GM 扫描 p.534](https://archive.org/download/sim_gentlemans-magazine_1752-11_22_11/page/n45_w1400.jpg))
- 【次级】1756 年才全面收容，1752 年仍抽签。([Wikipedia](https://en.wikipedia.org/wiki/Foundling_Hospital))

### 剧情可用的细节

- 【已证实】1752 年 9 月 16 日《Daily Advertiser》招女伴，要求写明 “if had the small-pox”（是否出过天花）。45 人回信，后又 18 人。这是雇佣时的真实问题。([GM 9 月](https://archive.org/details/sim_gentlemans-magazine_1752-09_22_9))
- 【已证实】1752 年 11 月 3 日星期五：一名意大利人号称有溶石秘方，当众演示，被发现用两个瓶子调包，液体是强碱水（soap lees）；此前他给 Bolton Street 的 Forrester 先生注射，病人极度痛苦，8 月下旬死去（日期 OCR 不清），开腹后发现结石毫未减小。([GM 11 月](https://archive.org/details/sim_gentlemans-magazine_1752-11_22_11))
- 【已证实】Newgate 通风：1752 年 7 月 11 日 Pringle 检查 Hales 的通风装置，涌出恶臭，改建通风口的 11 名木匠有 7 人染上牢狱热。([Creighton](https://www.gutenberg.org/ebooks/43671))
- 【已证实】1750 年 “Black Sessions”：5 月 11 日（新历）开庭，牢狱热随囚犯入庭，GM 1753 年 1 月刊 Pringle 的记述。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))

### 不能写的时代错误

- 牛痘接种（1796）不能出现。
- 没有 “vaccination”，只有 “inoculation”。
- 天花医院不在 St Pancras（1768 年）。
- 不写消毒、细菌。当时说 “contagion”“infection”“putrid”。
- 信物展览是后世的事。

---

## 10. 地理与日常

本节多为 Wikipedia。Survey of London 未查阅。

### 可直接用的事实

- 【次级】St Giles：教区名 St Giles in the Fields，1665 年瘟疫由此起。贫民窟核心在 Great Russell Street、Tottenham Court Road、St Giles High Street、Dyott Street 之间。([Wikipedia](https://en.wikipedia.org/wiki/St_Giles,_London))
- 【已证实】St Giles 是伦敦最早最大的爱尔兰聚居地。([Old Bailey Online: Irish](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Irish.jsp))
- 【已证实】老贝利文本里 “rookery” 1700–1800 年只有一次（1779 年，语境不明），1764 年有 “the Black horse in St. Giles's Ruins”。([t17791020-13](https://www.oldbaileyonline.org/record/t17791020-13)；[t17641212-59](https://www.oldbaileyonline.org/record/t17641212-59))
- 【存疑】“St Giles Rookery” 作为 1752 年当时称呼没有证据。“The Holy Land” 老贝利 1740–1775 年无命中。
- 【次级】Seven Dials：Thomas Neale 1690 年代开发，七条街当时叫 Great/Little Earl Street、Great/Little White Lion Street、Great/Little St Andrew's Street、Queen Street；日晷柱 1773 年才拆。([Wikipedia](https://en.wikipedia.org/wiki/Seven_Dials,_London))
- 【次级】Tom King's：Covent Garden 的棚屋咖啡馆，打烊后开到天亮。Moll King 1696 年生于 St Giles 的 Vine Street，约 1745 年退居 Haverstock Hill，1747 年 9 月 17 日去世。1752 年她已死。([Wikipedia](https://en.wikipedia.org/wiki/Tom_King%27s_Coffee_House))
- 【未查到】Tom King's 1752 年是否仍在营业。
- 【次级】Fleet Market：1736 年建在覆盖后的 Fleet 河上，今 Farringdon Street 一带。([Wikipedia](https://en.wikipedia.org/wiki/Fleet_Market))
- 【次级】Fleet 婚姻：到 1754 年 3 月 25 日《婚姻法》生效才止，1752 年合法存在。1742–1754 年 Keith's Chapel 也办。([Wikipedia](https://en.wikipedia.org/wiki/Fleet_Marriage))
- 【次级】Newgate：1672 年 Wren 时期重建的旧监狱；1752 年屋顶加了 Hales 的风车通风；新监狱 1769 年后才建。([Wikipedia](https://en.wikipedia.org/wiki/Newgate_Prison))
- 【已证实】Old Bailey 的 Justice Hall：审判在此，首页写 “HELD AT JUSTICE-HALL in the OLD-BAILEY”。([f17520914-1](https://www.oldbaileyonline.org/record/f17520914-1))
- 【已证实】Smithfield：8 月 19 日 Smithfield-Bars 油铺失火；8 月 22 日市长宣告 Bartholomew Fair。([GM 8 月](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8))
- 【次级】Execution Dock：Wapping 泰晤士河边，处决海盗和海事罪犯。([Wikipedia](https://en.wikipedia.org/wiki/Execution_Dock))
- 【已证实】Spitalfields：胡格诺丝织中心；Soho 是另一个法裔聚居地，金银匠和钟表匠多。([Old Bailey Online: Huguenot](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Huguenot.jsp))
- 【次级】Bevis Marks：1701 年落成的塞法迪犹太会堂。([Wikipedia](https://en.wikipedia.org/wiki/Bevis_Marks_Synagogue))
- 【次级】Bedlam：1676 年迁 Moorfields，1815 年离开。([Wikipedia](https://en.wikipedia.org/wiki/Bethlem_Royal_Hospital))
- 【已证实】1752 年 10 月 12 日 Common Council 下令拆除上下 Moorfields 之间的墙，“as it was a screen for thieves and obnoxious persons”。([GM 10 月](https://archive.org/details/sim_gentlemans-magazine_1752-10_22_10))
- 【次级】Westminster Bridge：1750 年 11 月 18 日开通，1752 年已是第二座桥。([Wikipedia](https://en.wikipedia.org/wiki/Westminster_Bridge))
- 【次级】London Bridge：桥上房屋 1752 年还在。1756 年法案才授权收购拆除，Samuel Scott 1757 年还画过。([Wikipedia](https://en.wikipedia.org/wiki/Old_London_Bridge))
- 【次级】Vauxhall：入场 1 先令，正式名 New Spring Gardens，1785 年才定名 Vauxhall Gardens，民间早叫 Vauxhall；主要坐船去，Vauxhall Bridge 要到 1810 年代。Ranelagh：1742 年开，2 先令 6 便士，有 Rotunda。([Wikipedia: Vauxhall](https://en.wikipedia.org/wiki/Vauxhall_Gardens)；[Wikipedia: Ranelagh](https://en.wikipedia.org/wiki/Ranelagh_Gardens))
- 【次级】Rocque 地图：1737 年开工，1746 年 24 张出版。1752 年情形已有变（如 Westminster Bridge）。([Wikipedia](https://en.wikipedia.org/wiki/John_Rocque))
- 【次级】City 1736 年立法收税点灯，到 1739 年 4,000 多盏油灯；1750 年代威斯敏斯特落后于近郊教区。([Westminster 照明史](https://email.sevendials.com/resources/History_of_Public_Lighting_in_the_City_of_Westminster.pdf))
- 【次级】linkboy 持火把领路，常与轿夫配合。([Wikipedia](https://en.wikipedia.org/wiki/Link-boy))
- 【已证实】轿夫、脚夫中爱尔兰人多。([Old Bailey Online: Irish](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Irish.jsp))
- 【已证实】船夫：从 Westminster 到 London Bridge 要 6 便士。([Old Bailey Online: Currency](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Coinage.jsp))

### 剧情可用的细节

- 【已证实】8 月 7 日：20 名女子 “sold themselves for four years” 给费城船长，从 Fleet Street 的 Bolt and Tun 出发。([GM 8 月](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8))
- 【已证实】8 月 21 日：几名出租轿夫到白厅报名去 Nova Scotia。([GM 8 月](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8))
- 【已证实】8 月 1 日报道：不久前 Lincoln's Inn 大火，瓦砾筛过后失主取回大部分金银，Pickering 先生丢失 1,100 镑银行券，大部分得以补办。([GM 8 月](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8))
- 【已证实】8 月 11 日：St Dunstan's 修缮工人在头骨上颌发现一片金子。([GM 8 月](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8))

### 不能写的时代错误

- 不写 Moll King 在世，不写 “New Oxford Street”（十九世纪）。
- Seven Dials 的柱子 1752 年还在。
- 不用现代街名 Shaftesbury Avenue、Charing Cross Road。
- 不写 Vauxhall Bridge、Blackfriars Bridge。泰晤士河上 1752 年伦敦段只有 London Bridge 和 Westminster Bridge。
- 无煤气灯（1807 年）。无 Regent Street。
- 不写 Fleet 河露天流过 Fleet Market 段。
- 不写 1752 年有 Peeler、警哨。

---

## 11. 物价与工资

### 可直接用的事实

- 【已证实】几尼 = 21 先令（随成色略有浮动）；1 镑 = 20 先令；1 先令 = 12 便士；crown 5 先令；half crown 2 先令 6 便士。([Old Bailey Online: Currency](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Coinage.jsp))
- 【已证实】工资（十八世纪总体）：女仆最低 2–3 镑（含食宿），housemaid 6–8 镑，熟练管家最高 15 镑，footman 8 镑，车夫 12–26 镑；手工业者年入 15–20 镑算低，养家约 40 镑；木匠 1700 年日薪 2 先令 6 便士。([Old Bailey Online: Currency](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Coinage.jsp))
- 【已证实】施舍一次 1 法寻到 2 便士；教区救济每周几便士到几先令；脚夫搬一蒲式耳煤 1 便士。([同上](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Coinage.jsp))
- 【已证实】住宿：合睡廉价旅馆一晚 2 便士；便宜的无家具房间约 1 先令 4 便士一周；租 10 镑一年的房子才获定居权。([同上](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Coinage.jsp))
- 【已证实】当时证词实例：1751 年两间房每周 14 便士([t17510417-45](https://www.oldbaileyonline.org/record/t17510417-45))；一间每周 9 便士([t17510523-23](https://www.oldbaileyonline.org/record/t17510523-23))；一间每周 1 先令 6 便士([t17510523-52](https://www.oldbaileyonline.org/record/t17510523-52))；1750 年霍尔本一楼带家具每周 7 先令([t17500711-2](https://www.oldbaileyonline.org/record/t17500711-2))；1743 年食宿每周 8 先令([t17430629-18](https://www.oldbaileyonline.org/record/t17430629-18))。
- 【已证实】小屋年租 5–6 镑（1760 年）。([t17600116-2](https://www.oldbaileyonline.org/record/t17600116-2))
- 【已证实】高技能织工周入 30–40 先令（1753 年，纱罗工）。([t17530502-11](https://www.oldbaileyonline.org/record/t17530502-11))
- 【已证实】1750 年有人雇黑人仆从，约定年薪 10 镑。([t17500530-4](https://www.oldbaileyonline.org/record/t17500530-4))
- 【已证实】1753 年酒馆有 “a pennyworth of gin”，1745 年有 “half a quartern of gin”，所以 1 便士一杯的说法成立。([t17530502-60](https://www.oldbaileyonline.org/record/t17530502-60)；[t17450424-10](https://www.oldbaileyonline.org/record/t17450424-10))
- 【未查到】一夸脱金酒的 1750 年代市价。
- 【已证实】面包：平常年景 1 磅面包约半便士到 1 便士 1 法寻。([Old Bailey Online: Currency](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Coinage.jsp))
- 【已证实】1762 年起诉书把一个 quartern loaf 估值 4 便士。([t17620224-13](https://www.oldbaileyonline.org/record/t17620224-13))
- 【次级】面包按 Assize 定价，通常固定价格、改变重量。([Wikipedia](https://en.wikipedia.org/wiki/Assize_of_Bread_and_Ale))
- 【未查到】1752 年伦敦 Assize 的确切数字；1750 年代普通劳工日薪（搜到的只是 Schwarz 系列书目）。
- 【已证实】出版物：老贝利 Proceedings 4 便士；Ordinary 记录 6 便士；《Covent-Garden Journal》3 便士；Blandy 审判记录 1 先令。([f17520914-1](https://www.oldbaileyonline.org/record/f17520914-1)；[OA17520922](https://www.oldbaileyonline.org/record/OA17520922)；[Jensen](https://archive.org/details/coventgardenjour01fieluoft)；[Tryal](https://archive.org/details/tryalofmarybland00blan))
- 【已证实】周报订阅 1662 年每年 4 先令。1750 年代【未查到】。([Graunt](https://archive.org/details/economicwritings02pett))
- 【未查到】《Daily Advertiser》等日报 1752 年零售价；GM 售价（一般说 6 便士，本次未见印证）。
- 【次级】1751 年法案（24 Geo. 2 c.40，正式短名 Sale of Spirits Act 1750）：1751 年 6 月 25 日批准，7 月 1 日生效。([Wikipedia](https://en.wikipedia.org/wiki/Gin_Act_1751))
- 【次级】内容：禁止蒸馏商卖给无证商贩，零售执照限于有一定财产者，收高费，并提高酒税。([Wikipedia](https://en.wikipedia.org/wiki/Gin_Act_1751))
- 【未查到】执照 2 镑、20 先令以下酒债不可追讨等细节（我未读到原文）。

### 剧情可用的细节

- 【已证实】布里斯托尔黄油旧圣诞价 9–10 便士一磅，见第 1 节。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))
- 【已证实】陪审团可用部分裁决减轻罪名，让被告躲开死刑。([Old Bailey Online: Punishment](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Punishment.jsp))
- 【建议】验尸婆的 4 便士可对比一周 9 便士的房租。

### 不能写的时代错误

- 不用十进制。金币是 guinea 系列，不写 sovereign。
- 1751 年法案后金酒没有消失，1753 年证词里还有一便士金酒。

---

## 12. 老贝利审判程序

### 可直接用的事实

- 【已证实】每年约八次开庭。([London Lives: OBP](https://old.londonlives.org/static/OBP.jsp))

| 庭期 | 日期 | 备注 | 来源 |
|---|---|---|---|
| 1752 第 1–5 次 | 1 月 16–20 日，2 月 18–19 日起，4 月 8 日起，5 月 14 日起，6 月 25–30 日 | Wilford 在 6 月庭 | [f17520116-1](https://www.oldbaileyonline.org/record/f17520116-1)；[OA17520713](https://www.oldbaileyonline.org/record/OA17520713) |
| 第 7 号 | **9 月 14、15、16、18、19、20 日** | 开庭首日就是新历第一天 | [f17520914-1](https://www.oldbaileyonline.org/record/f17520914-1) |
| 第 8 号 | 10 月 26、27、28、30 日 | Alsop 任内第 3 次 | [f17521026-1](https://www.oldbaileyonline.org/record/f17521026-1) |
| Gascoyne 第 1 号 | 12 月 6、7、8 日 | | [f17521206-1](https://www.oldbaileyonline.org/record/f17521206-1) |
| Gascoyne 第 2 号 | 1753 年 1 月 11、12、13、15 日 | | [f17530111-1](https://www.oldbaileyonline.org/record/f17530111-1) |

- 【已证实】6 月 30 日到 9 月 14 日之间没有开庭，所以夏季被捕者要在狱中等到 9 月。([f17520914-1](https://www.oldbaileyonline.org/record/f17520914-1)：Number VII，紧接 6 月庭的 Number VI)
- 【已证实】市长：Thomas Winterbottom（到 1752 年年中）、Robert Alsop、Sir Crisp Gascoyne（11 月 8 日起，11 月 22 日受封爵士）。Recorder 为 Richard Adams。([f17520116-1](https://www.oldbaileyonline.org/record/f17520116-1)；[OA17520922](https://www.oldbaileyonline.org/record/OA17520922)；[GM 扫描 p.534](https://archive.org/download/sim_gentlemans-magazine_1752-11_22_11/page/n45_w1400.jpg))
- 【已证实】9 月庭结果：102 人受审，5 人死刑，2 人流放 14 年，47 人流放 7 年，3 人烙手，5 人鞭刑（GM 记 4 人）。([s17520914-1](https://www.oldbaileyonline.org/record/s17520914-1)；[GM 9 月](https://archive.org/details/sim_gentlemans-magazine_1752-09_22_9))
- 【已证实】被告律师：重罪案 1730 年代中期起才准律师，只限法律点和交叉询问，不得向陪审团总结；被告不宣誓，须自己讲；律师仍是少数。([Old Bailey Online: Trial Procedures](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Trial-procedures.jsp))
- 【已证实】1752 年法律允许法院在定罪后补偿贫穷控方的费用。([同上](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Trial-procedures.jsp))
- 【已证实】老贝利有 City 和 Middlesex 两个陪审团，审判极快。([同上](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Trial-procedures.jsp))
- 【已证实】大陪审团只听控方。Middlesex 案在 Hicks Hall 先审起诉状。([同上](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Trial-procedures.jsp))
- 【已证实】女性可作证（第 2 节验尸婆实例）；法庭人员全为男性；孕妇请求缓刑时由旁听女性组成 jury of matrons。([Old Bailey Online: Gender](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Gender.jsp))
- 【已证实】枷刑适用于诈骗、伪证等，群众投掷物品，有人被打死；每年伦敦约 5–10 人。([Old Bailey Online: Punishment](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Punishment.jsp))
- 【已证实】神职特权：1706 年废除识字测试，自动适用于未被排除的罪名；烙拇指到 1779 年；女性 1691 年起待遇同等。([Punishment](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Punishment.jsp)；[Gender](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Gender.jsp))
- 【已证实】流放：7 年或 14 年，返回者死罪（Thomas Butler 1752 年 “for returning from transportation”）。([GM 9 月](https://archive.org/details/sim_gentlemans-magazine_1752-09_22_9))
- 【已证实】Ordinary of Newgate 的记录每次行刑印发，6 便士，印商 T. Parker 和 C. Corbett。([OA17520922](https://www.oldbaileyonline.org/record/OA17520922))
- 【未查到】1752 年 Ordinary 本人姓名（我没读到署名，通常说是 John Taylor）。
- 【已证实】死刑判决的实际执行率不到五分之一（整个时段）。([Punishment](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Punishment.jsp))
- 【已证实】已婚妇女：Blackstone 1765 年 “the very being or legal existence of the woman is suspended during the marriage”（妇女的法律存在在婚内中止），称 feme-covert；不能单独起诉或被诉。比游戏晚 13 年，只作法理总结。([Blackstone](https://avalon.law.yale.edu/18th_century/blackstone_bk1ch15.asp))
- 【已证实】实务：已婚女子的衣物被偷，失主记在丈夫名下；只有约七分之一的控方是女性。([Old Bailey Online: Gender](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Gender.jsp))
- 【已证实】寡妇可用自己名义持有财产：1752 年 9 月案 “the property of Mary Showring, widow”。([t17520914-1](https://www.oldbaileyonline.org/record/t17520914-1))
- 【未查到】配偶互证规则（本次所读 Blackstone 段落未见）。

### 剧情可用的细节

- 【已证实】Montgomery 案：1748 年宣誓时称 “a fugitive beyond the seas on Jan. 1, 1747” 以骗债权人，判死。([GM 11 月](https://archive.org/details/sim_gentlemans-magazine_1752-11_22_11)；[OA17521113](https://www.oldbaileyonline.org/record/OA17521113))
- 【已证实】1752 年 10 月 27 日，“Baron” Bourke 受审获释。他追上劫匪拿了金表，转而被控抢劫，自称德意志男爵。([GM 10 月](https://archive.org/details/sim_gentlemans-magazine_1752-10_22_10))
- 【建议】寡妇身份是主角的法律资本：可以自己持有基金份额。再嫁即失去。

### 不能写的时代错误

- 没有无罪推定的现代口号，没有 “objection”，没有控方律师主导的庭审。
- 被告不宣誓作证。
- 不写 1752 年有公设辩护人。

---

## 13. 人群、社会与大事

### 可直接用的事实

**黑人**

- 【已证实】1783 年伦敦约 5,000–10,000 名黑人；1752 年具体数字【未查到】。主要从事家仆和底层工作；住东区贫穷教区较多，黑人乞丐与 St Giles 相关；法律地位暧昧：“the law neither clearly recognised the legality of slavery, nor granted them freedom from it”。([Old Bailey Online: Black Communities](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Black.jsp))
- 【次级】估计差距极大：Chater 3,000–5,000；Sharp 称 20,000。([Jane Austen's World 转述](https://janeaustensworld.com/tag/kathleen-chater/)；[Wikipedia](https://en.wikipedia.org/wiki/18th-century_London))
- 【次级】Francis Barber：约 1742/3 年生于牙买加，原名 Quashey，10 岁左右被 Bathurst 上校带到英国，1752 年入 Gough Square 的约翰逊家，时值约翰逊丧妻后数周，一说 4 月。住到 1756 年。([Dr Johnson's House](https://www.drjohnsonshouse.org/francis-barber)；[Wikipedia](https://en.wikipedia.org/wiki/Francis_Barber))
- 【已证实】约翰逊之妻 Tetty 死于 “the 17th of March, O.S.”（1752 年），约翰逊 1753 年记 “March 28, 1753. I kept this day as the anniversary of my Tetty's death”。([Boswell](https://jacklynch.net/Texts/BLJ/b166.html))
- 【推算】17 + 11 = 28，他守的是自然日。这是历史名人遵循“同一自然日”最好的实例。Harvard 展览写 3 月 25 日，属少数说法。

**犹太人**

- 【已证实】分两群：塞法迪人先到，同化快；阿什肯纳兹人来自中东欧，贫穷，多以旧货、水果等街头小贩为生，Great Synagogue 在 Duke's Place。([Old Bailey Online: Jewish](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Jewish.jsp))
- 【次级】《犹太人入籍法》26 Geo. 2 c.26，1753 年 7 月批准，同年 12 月撤销（27 Geo. 2 c.1）。游戏结束后才发生，1752 年人物不能提。([Wikipedia](https://en.wikipedia.org/wiki/Jewish_Naturalisation_Act_1753)；[statutes.org.uk](https://statutes.org.uk/site/the-statutes/eighteenth-century/))
- 【已证实】Moravia 案中 Zac. Levi 和 Daniel Peires 作证，体现了犹太商业圈。([t17520625-51](https://www.oldbaileyonline.org/record/t17520625-51))

**胡格诺与爱尔兰**

- 【已证实】法裔教堂在 Threadneedle Street（严格加尔文派）和 Savoy（用英国国教礼）。([Old Bailey Online: Huguenot](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Huguenot.jsp))
- 【已证实】1752 年 8 月底，Tryton 号从鹿特丹载约 80 名法国新教徒抵都柏林，“bred in the silk business”。这是逃亡仍在继续的证据，也说明鹿特丹是中转站。([GM 扫描 p.431](https://archive.org/download/sim_gentlemans-magazine_1752-09_22_9/page/n46_w1400.jpg))
- 【已证实】爱尔兰人：季节工、街头小贩、建筑工、轿夫、脚夫；1720–30 年代 Moll Harvey 和 Isabella Eaton 帮派有爱尔兰背景。([Old Bailey Online: Irish](https://web.archive.org/web/2019/https://www.oldbaileyonline.org/static/Irish.jsp))

**其他**

- 【次级】Gin Lane：霍加斯 1751 年 2 月中在《London Evening Post》登广告。背景就是 St Giles，《Four Stages of Cruelty》第一幅也在 St Giles。([Wikipedia](https://en.wikipedia.org/wiki/Beer_Street_and_Gin_Lane))
- 【已证实】格拉斯顿伯里圣荆棘：GM 1753 年 1 月：人们在新历圣诞夜去看，没开，“which made them watch it narrowly the 5th of Jan. the Christmas-Day, Old-Stile, when it blow'd as usual”（1 月 5 日旧圣诞才开）。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))
- 【已证实】同页 Quainton（白金汉郡）：2,000 多人提灯看一株号称圣荆棘枝条的黑刺李，没开花，于是认定 “Decemb. 25 N.S. could not be the right Christmas-Day”，拒绝上教堂，附近牧师被迫宣布旧圣诞照常守。同页引 Millar 的园艺辞典作反驳。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))
- 【次级】Elizabeth Canning：1753 年 1 月 1 日失踪，约一个月后回到 Aldermanbury；Lord Mayor Gascoyne 后来介入。审判一手记录见 1753 年 2 月庭。([Wikipedia](https://en.wikipedia.org/wiki/Elizabeth_Canning)；[t17530221-47](https://www.oldbaileyonline.org/record/t17530221-47))
- 【次级】Hans Sloane 1753 年 1 月 11 日去世。([Wikipedia](https://en.wikipedia.org/wiki/Hans_Sloane))
- 【已证实】GM 记 1 月 27 日星期六上午约 10 点，40 多名遗嘱受托人为他的藏品开会。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))

### 1752 年 8 月至 1753 年 1 月大事日表（均出自 GM 或 Old Bailey）

| 日期 | 星期 | 事件 |
|---|---|---|
| 8/1 | 六 | Lincoln's Inn 火灾废墟筛财 |
| 8/7 | 五 | 20 名女子卖身赴费城 |
| 8/8 | 六 | Carlisle Street 荷兰面包师 Scarsfield 家失火 |
| 8/19 | 三 | Smithfield-Bars 失火 |
| 8/22 | 六 | 宣告 Bartholomew Fair；Serjeants-Inn 审议议员精神病案 |
| 8/25–26 | 二三 | 大风暴，肯特啤酒花杆倒毁，Tyne 和 Wear 河洪水 |
| 8/31 | 一 | 诺里奇毛梳工罢工营地（10 月 3 日前复工） |
| 9/1 | 二 | 周账 328 |
| 9/2 | 三 | 旧历最后一天 |
| 9/14 | 四 | 新历第一天；老贝利开庭 |
| 9/16 | 六 | 《Daily Advertiser》招女伴广告 |
| 9/19 | 二 | 周账 338（紧接 9/1） |
| 9/20 | 三 | 老贝利闭庭 |
| 9/22 | 五 | Branch、Descent 绞死并解剖 |
| 10/11 | 三 | Lee、Wilks、Butler 绞死 |
| 10/12 | 四 | 下令拆 Moorfields 隔墙 |
| 10/14 | 六 | Hounslow 枪击事件 |
| 10/18 | 三 | 圣路加日哈维演讲；遗赠天花医院 |
| 10/26–30 | | 老贝利 |
| 11/3 | 五 | 意大利溶石骗子被揭穿 |
| 11/8 | 三 | 市长宣誓；国王离汉诺威 |
| 11/9 | 四 | 市长赴威斯敏斯特 |
| 11/11 | 六 | Foundling 抽签 20/59；国王抵 Helvoetsluys |
| 11/13 | 一 | Montgomery 绞死 |
| 11/18 | 六 | 国王在 Gravesend 登陆，当晚回圣詹姆斯宫 |
| 11/25 | 六 | 《Covent-Garden Journal》终刊 |
| 12/6–8 | | 老贝利 |
| 12/11 | 一 | Abraham Ward 绞死 |
| 12/12 | 二 | 1752 总账截止 |
| 12/14 | 四 | Moravia、Manoury 二次枷示 |
| 12/20 | 三 | Richmond Park 开放 |
| 12/25 | 一 | 新历圣诞 |
| 1/1 | 一 | Canning 失踪 |
| 1/5 | 五 | 旧圣诞，南海半年结算日 |
| 1/11 | 四 | Sloane 去世；议会开会；老贝利开庭 |
| 1/15 | 一 | 老贝利闭庭，5 人死刑 |
| 1/25 | 四 | 南海公司宣布 2% 股息 |
| 1/27 | 六 | Sloane 受托人开会 |

### 天气（GM 气象栏，OCR）

- 【已证实】8 月 25–26 日大风暴，见日表。查尔斯顿 9 月 15 日大飓风的详报刊在 GM 12 月号。([GM 12 月](https://archive.org/details/sim_gentlemans-magazine_1752-12_22_12))
- 【已证实】GM 11 月号气象栏（所指月份未注明，疑为 10 月）：本月多变，月初 “mostly fair, clear and frosty; with thick fogs morning and nights in the city”（多晴冷有霜，城里早晚大雾）。气温 46–57 度。([GM 11 月](https://archive.org/details/sim_gentlemans-magazine_1752-11_22_11))
- 【已证实】GM 1753 年 1 月号气象栏（疑指 12 月）：冷、晴、霜，24 日有 “hard frost”，伦敦露天 32–44 度。同期 Bristol 12 月 12 日大雷雨，Penzance 12 月 20 日雷暴。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))
- 【已证实】12 月底路上积水：牛津到布里斯托尔的邮路耽误 10 小时。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))
- 【已证实】Aberdeen 到 12 月 22 日才有第一场霜。([GM 1753 年 1 月](https://archive.org/details/sim_gentlemans-magazine_1753-01_23_1))

### 剧情可用的细节

- 【建议】Tetty 守忌日的例子可让约翰逊家的 Barber 成为“忌日按自然日算”的线索来源。
- 【建议】Quainton 的提灯人群可作旧圣诞夜的大场面，不过它在白金汉郡，要写成报上传闻。

### 不能写的时代错误

- 1752 年无《入籍法》风波，要到 1753 年 7 月后。
- Canning 案开始于最后一个月，不要提前。
- 不写 1752 年 Sloane 的藏品已成大英博物馆（1753 年立法，1759 年开放，后两者年份【次级，未在本次重查】）。
- Barber 1752 年是约十岁的孩子，不能当成年助手。

---

## 14. 英荷交通

### 可直接用的事实

- 【已证实】Nugent《Grand Tour》（我读的是 1778 年版，初版 1749）：“The English packet-boat sets off from hence for Harwich twice a week, that is Wednesdays and Saturdays”（英国邮船每周两班，周三周六从 Helvoetsluys 开往 Harwich）。([Nugent 1778 第 1 卷](https://archive.org/details/bim_eighteenth-century_the-grand-tour-or-a-jo_nugent-thomas_1778_1))
- 【已证实】同书：从 Helvoetsluys 去 Briel 的马车每天早 8 点出发，每人 7 stiver，走路约一个半小时。([Nugent](https://archive.org/details/bim_eighteenth-century_the-grand-tour-or-a-jo_nugent-thomas_1778_1))
- 【次级】Harwich 邮船线 1660 年代起运行，到 1805 年；1668 年通告称 60 吨邮船。([GBPS 1668](https://gbps.org.uk/information/sources/po-notices/1660-1669/166802.php)；[Harwich Society](https://www.harwich-society.co.uk/maritime-heritage-trail/))
- 【存疑】航程：一份未辨明年代与航线的十八世纪文本说，顺风约 16 小时，逆风 48 小时，有时 13–14 小时。([ECCO 摘要](https://quod.lib.umich.edu/e/ecco/004853688.0001.002/69:4)，只见搜索摘要)
- 【已证实】最硬的实例：**1752 年 11 月乔治二世回国**。11 月 8 日离汉诺威，10 日下午约 4 点到 Utrecht，6 点登驳船，11 日上午约 10 点到一处水闸（OCR 残缺，疑为 Maesland Sluys），约 1 点到 Helvoetsluys；**星期六 18 日下午 2 点在 Gravesend 登陆，5 点到圣詹姆斯宫**。GM 称他 “about 10 on the 19th” 启航，这个日期印错了（当时才 18 日），实际启航日不明。([GM 扫描 p.534](https://archive.org/download/sim_gentlemans-magazine_1752-11_22_11/page/n45_w1400.jpg)；[GM 11 月 OCR](https://archive.org/details/sim_gentlemans-magazine_1752-11_22_11))
- 【推算】国王从抵港到登陆隔了 7 天，大部分很可能在等风。普通旅客等更久也合理。
- 【推算】鹿特丹到伦敦：内河到 Helvoetsluys 半天到一天，等班 0–3 天（每周两班），渡海 1–2 天，Harwich 到伦敦 1–2 天，合计约 4–8 天，逆风可超两周。这是我的推算，不是史料原话。
- 【已证实】1752 年 8 月 26 日大风把一艘荷兰船吹上岸（地名 OCR 不清），北海风险是真实的。([GM 8 月](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8))
- 【未查到】1752 年官方票价与 Harwich 到伦敦马车时间。

### 剧情可用的细节

- 【推算】连国王都在 Helvoetsluys 等了约一周，所以“信晚到一周”不需要解释。([GM 扫描 p.534](https://archive.org/download/sim_gentlemans-magazine_1752-11_22_11/page/n45_w1400.jpg))
- 【已证实】外电在 GM 里标 N.S.（如 8 月 “13. N.S.”）。([GM 8 月](https://archive.org/details/sim_gentlemans-magazine_1752-08_22_8))
- 【建议】鹿特丹来的证明，9 月 14 日以前落款比伦敦早 11 天，容易让人以为证人“提前知道”了死讯。

### 不能写的时代错误

- 不写蒸汽船。不写 Hook of Holland 航线。
- 鹿特丹和伦敦 1752 年 9 月 14 日之后日期才一致。
- 不写固定时刻表准点到达。

---

## 附：未查到或存疑总表

1. 1752 年 St Giles 验尸婆人数、姓名、宣誓者。
2. 1750 年代验尸收费（只有 1662 年 1 格罗特和伊丽莎白时代数字）。
3. 1752 年周报是否星期四发行（1662 年证据外推）。
4. 1752 年周报印工姓名与订费。
5. 1752 年 Middlesex 与 Westminster 验尸官姓名；Umfreville 1752 年是否在任；St Giles 属东区还是西区。
6. 1752 年《验尸官法》章号（c.29 的年份归属）及原文。
7. “Mr Fielding's People” 一语的出处；1752 年 Fielding 手下确切人数。
8. Fielding “4 Bow Street” 门牌同代证据；John Fielding 何时成为治安法官。
9. Blandy 案审判厅（Divinity School 说法未核）；“Easter Monday” 说法与复活节日期不合。
10. “ratsbane” 1750 年代实例；1752 年售砒是否有任何管制。
11. 外科医师公会 “Cyprian Theatre” 名称。
12. 复活节开庭期 1752 年的精确首日。
13. 1700–1750 年具名私人 tontine 实例。
14. 赌沃波尔寿命实例；1720 年后投资者心理的专门材料。
15. Pons 咖啡馆与棣莫弗（只见摘要）；“替赌徒解题”只见弱来源。
16. 1752 年《Ladies' Diary》主编。
17. 1752 年 Tom King's 是否营业；Survey of London 对 St Giles 的具体记述。
18. “Rookery” 1752 年用法。
19. 一夸脱金酒价、1752 年面包 Assize 价、1750 年代劳工日薪、日报售价、GM 售价。
20. 1751 年《杜松子酒法》的执照费和酒债条款。
21. 配偶互证规则；Ordinary 本人姓名。
22. 1752 年后民间继续写双年份的实例；荷兰省岁首年份；荷兰省精确切换日。
23. 1752 年 Harwich 邮船航时、票价；Harwich 到伦敦时间；国王实际启航日。

---

## 15. 补充考证（主线专用，由主编核查）

本节不属于上面的统计。【虚构】表示有意虚构的设定，不是史实。

### 人痘“皇家实验”

- 【次级】Charles Maitland 1721 年 8 月 9 日在 Newgate 给六名死囚种痘，Hans Sloane 在场；1722 年 2 月下旬又给圣詹姆斯教区（St James Westminster）的孤儿种痘，人数各来源说五到六人。1722 年 4 月两位公主种痘。Maitland 8 月 8 日致 Sloane 的信确认了 Newgate 手术的时间。([Sloane Letters](https://sloaneletters.com/?p=43724)；[Wikipedia: Maitland](https://en.wikipedia.org/wiki/Charles_Maitland_(physician))；[HNN](https://www.historynewsnetwork.org/article/162034))
- 【存疑】Newgate 试验有来源误作 1722 年，以 1721 年 8 月为准。
- 【虚构】奥姆医生 1722 年春夏为 24 名孤儿种痘是虚构，放在真实试验之后。对白里只说“皇家试验之后”。

### 周报的年龄栏

- 【次级】年龄从 1727 年 12 月至 1728 年起进入周报，只有年龄段，不和死因交叉。([Wikipedia: Bills](https://en.wikipedia.org/wiki/Bills_of_mortality)；[Britannica 1823](https://jimclifford.ca/early_encyclopedia_britannica/articles/1823/eb_6th_1823_020593.html))

### 排字工

- 【已证实】排字工从字盘取字，在排字手托上按镜像排列，要能读反字；Moxon《Mechanick Exercises》（1683–84）有图。铸字的 p 和 q、b 和 d 容易混。([LOC](https://blogs.loc.gov/bibliomania/2025/01/15/dont-get-out-of-sorts-composing-text/)；[St Brigid Press](https://stbrigidpress.net/blog/tag/composing+stick))

### 守夜

- 【次级】守夜人晚 9 点到早 6 点沿固定路线巡逻，报时辰和天气，有岗亭和灯笼，1737 年法案（10 Geo. II c.22）扩大威斯敏斯特等地守夜权限。喊时辰的原话 1750 年代没有一手记录。([Art in Society](https://www.artinsociety.com/watchmen-goldfinders-and-the-plague-bearers-of-the-night.html)；[Middle Temple](https://www.middletemple.org.uk/node/4675))

### 配偶不得互证

- 【次级】普通法下配偶不能互相作证，理由是夫妻在法律上是一人。规则只适用于有效且存续的婚姻。([Wikipedia: Spousal privilege](https://en.wikipedia.org/wiki/Spousal_privilege)；[爱尔兰法律改革委员会 1985](https://lawreform.ie/_fileupload/Reports/rSpousesandWitnesses.htm))
- 【次级】例外来自 Lord Audley's Case（1631）：丈夫对妻子人身施暴的案件，妻子可以作证。([同上](https://en.wikipedia.org/wiki/Spousal_privilege))
- 【存疑】十八世纪英格兰重婚案中第二任“妻子”可否作证的判例未直接查到，但规则的前提是有效婚姻，无效婚姻不受此限，这是法理推论。
