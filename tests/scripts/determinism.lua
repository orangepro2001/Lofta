-- A fixed input script. The hashes must match between Linux and Windows builds.
local d = require("drive")
local t = require("testlib")
local world = require("screens.world")

local function run(seed)
    main.on_new_game(seed)
    t.draw()
    core.actor_walk(world.player, 12, 7)
    d.settle(world.player)
    core.actor_walk(world.player, 3, 3)
    t.ticks(30)
    core.light(120, 130, 190)
    t.draw()
    return core.state_hash(), core.frame_hash()
end

local s1, f1 = run(11)
local s2, f2 = run(11)
t.eq(s1, s2, "the same seed gives the same state")
t.eq(f1, f2, "the same seed gives the same frame")
host.print("hash state " .. s1)
host.print("hash frame " .. f1)
