-- The three verbs, the fund and the text variables, on the real runtime.
local cards = require("cards")
local story = require("story")
local t = require("testlib")

main.on_new_game(2)

-- Induction: too few cases is a different answer from a wrong column.
story.give("c_demo_case_a")
story.give("c_demo_case_b")
local recipe, miss = cards.induce("t_demo", "存根")
t.check(recipe == nil, "two rows are not enough")
t.eq(miss, "short", "the idea is right, one case short")
recipe, miss = cards.induce("t_demo", "教区")
t.check(recipe == nil, "the other column gives nothing")
t.eq(miss, "none", "that column is no pattern")
story.give("c_demo_case_c")
recipe = cards.induce("t_demo", "存根")
t.eq(recipe and recipe.id, "i_demo_stub", "three rows make the rule")
t.check(story.has("c_demo_rule_stub"), "the rule card exists")

-- A superset of rows is accepted: holding more cases is never punished.
story.give("c_demo_case_d")
t.eq(#cards.table_rows("t_demo"), 3, "only the table's own cases are rows")

-- Rebuttal retires the old rule, and the card stays in hand as unproved.
recipe = cards.rebut("c_demo_rule_stub", "c_demo_case_d")
t.eq(recipe and recipe.id, "r_demo_stub", "the counter-example breaks the rule")
t.check(story.has("c_demo_rule_fixed"), "the amended rule exists")
t.check(story.s.flags.unproved_c_demo_rule_stub, "the old rule is marked unproved")
t.check(story.has("c_demo_rule_stub"), "the old card is not taken away")

-- A case that does not fit the rule leaves it alone.
recipe, miss = cards.rebut("c_demo_rule_stub", "c_demo_case_a")
t.check(recipe == nil, "a fitting case refutes nothing")
t.eq(miss, "half", "the rule was the right target")

-- The fund agrees with tools/calendar_facts.py, see story_check.
local f = story.s.flags
f.lucy_alive, f.will_alive, f.kitty_alive, f.ned_alive = true, true, true, true
story.compute_fund()
t.eq(story.var("fund_ruth"), 15568, "everyone alive, Harry counted")
t.eq(story.var("fund_shares"), 3, "three shares are hers")
t.eq(story.var("fund_total"), 6, "six shares in all")
f.lyle_guilty = true
story.compute_fund()
t.eq(story.var("fund_ruth"), 18682, "Harry's share gone")
f.pereira_hanged = true
story.compute_fund()
t.eq(story.var("fund_ruth"), 31137, "Pereira's shares gone too")
f.lucy_alive, f.will_alive = nil, nil
f.pereira_hanged = nil
story.compute_fund()
t.eq(story.var("fund_ruth"), 10379, "one of three")
t.eq(story.fill("她得 {fund_ruth} 镑。"), "她得 10379 镑。", "text variables fill in")
