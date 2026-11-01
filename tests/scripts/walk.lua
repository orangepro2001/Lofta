local d = require("drive")
local t = require("testlib")
local world = require("screens.world")

main.on_new_game(3)
t.draw()
local player = world.player
local x, y = core.actor_cell(player)
t.eq(x, 3, "player starts at x 3")
t.eq(y, 12, "player starts at y 12")

-- A tap on open ground walks there.
t.check(d.tap_cell(5, 10), "cell (5,10) is on screen")
t.check(core.actor_moving(player), "the tap started a walk")
t.check(d.settle(player), "the walk ends")
x, y = core.actor_cell(player)
t.eq(x, 5, "arrived at x 5")
t.eq(y, 10, "arrived at y 10")

-- Walls and water refuse a route, and the player stays put.
t.eq(core.actor_walk(player, 0, 0), -1, "no route into a wall")
t.eq(core.actor_walk(player, 5, 13), -1, "no route onto water")
d.settle(player)

-- Stepping indoors opens the building.
t.check(core.actor_walk(player, 3, 3) > 0, "a route leads through the door")
t.check(d.settle(player), "the walk indoors ends")
t.ticks(1)
x, y = core.actor_cell(player)
t.eq(core.cell_region(x, y), 1, "player is inside region 1")
t.eq(world.cut, 1, "the building is cut open")
t.shot("walk_indoors", 2)

t.check(core.actor_walk(player, 8, 8) > 0, "a route leads back outside")
t.check(d.settle(player), "the walk outdoors ends")
t.ticks(1)
t.eq(world.cut, 0, "the building closes again")
t.shot("walk_outdoors", 2)
