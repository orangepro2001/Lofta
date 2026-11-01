local cards = require("cards")
local d = require("drive")
local flow = require("flow")
local game = require("game")
local story = require("story")
local t = require("testlib")
local talk = require("screens.talk")
local world = require("screens.world")

main.on_new_game(5)
t.draw()

-- Walk up to the person and start talking, by tapping them.
local npc = world.npcs[1]
t.check(npc ~= nil, "the scene has a person")
d.tap_actor(npc.id)
t.check(world.pending ~= nil, "the tap queues a conversation")
t.check(d.settle(world.player), "the walk ends")
t.ticks(1)
t.eq(flow.top(), talk, "the conversation opened")

-- Two lines of narration, then the menu.
local function pass_lines()
    local guard = 0
    while flow.top() == talk and talk.step and talk.step.kind == "line" do
        t.key("space")
        t.key("space")
        guard = guard + 1
        t.check(guard < 50, "lines end")
        if guard >= 50 then
            break
        end
    end
end

pass_lines()
t.eq(talk.step.kind, "choice", "the menu is up")
t.eq(#talk.step.options, 4, "four options at first")
t.check(story.s.flags.demo_met, "the effect ran")
t.shot("talk_menu", 2)

-- Take all three once-only options; each disappears after use and gives a card.
t.key("1")
pass_lines()
t.eq(#talk.step.options, 3, "a once-only option disappears")
t.check(story.has("c_demo_no_gin"), "the first card arrived")
t.key("1")
pass_lines()
t.key("1")
pass_lines()
t.eq(#talk.step.options, 1, "only leaving remains")
t.check(story.has("c_demo_drunk_rule"), "the rule card arrived")
t.shot("talk_card", 2)
t.key("1")
t.check(flow.top() == world, "leaving closes the conversation")

-- Save here, then synthesize, then come back: the save must not remember the future.
t.check(game.save("test_story.dat"), "save written")
local before = core.state_hash()

-- A valid deduction, whichever card fills which part is the board's job, not the player's.
local recipe = cards.deduce("c_demo_drunk_rule", "c_demo_garlic")
t.check(recipe ~= nil and recipe.id == "d_demo_not_drunk", "rule and fact give the deduction")
t.check(story.has("c_demo_not_drunk"), "the inference card exists")

-- A fallacy looks like progress. Only the recipe knows.
recipe = cards.deduce("c_demo_drunk_rule", "c_demo_no_gin")
t.eq(recipe.fallacy, "否定前件", "the trap names its fault")
t.eq(cards.card(recipe.output).kind, "fallacy", "its card is a fallacy card")

-- The right rule with the wrong case is told apart from nonsense.
local miss
recipe, miss = cards.deduce("c_demo_drunk_rule", "c_demo_not_drunk")
t.check(recipe == nil, "this case is no minor premise")
t.eq(miss, "half", "the rule was the right tool")
recipe, miss = cards.deduce("c_demo_no_gin", "c_demo_garlic")
t.check(recipe == nil, "two cases make no deduction")
t.eq(miss, "none", "nothing fits")
t.eq(story.var("failed_syntheses"), 2, "misses are counted for playtest logs")

-- Board by tap: pick two cards, combine.
flow.push(require("screens.board"))
t.shot("board", 2)
t.tap(12 + 20, 28 + 20)
t.tap(12 + 206 + 20, 28 + 20)
t.tap(40, 330)
t.eq(require("screens.board").result, "这两张牌说的不是一回事。", "the board shows why two cases do not combine")
t.key("escape")
t.eq(flow.top(), world, "escape leaves the board")

t.check(game.load("test_story.dat"), "save read back")
t.eq(core.state_hash(), before, "the core comes back exactly")
t.check(not story.has("c_demo_not_drunk"), "the save does not know about later cards")
t.check(story.has("c_demo_drunk_rule"), "the save keeps earlier cards")
t.eq(story.s.picked.demo_menu_1, true, "once-only choices stay taken")
t.draw()
