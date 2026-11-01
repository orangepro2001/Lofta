-- Screenshots for looking at: day and night views of the test scene.
local t = require("testlib")
local world = require("screens.world")

main.on_new_game(9)
t.shot("tour_day", 2)
core.light(110, 120, 200)
t.shot("tour_night", 2)
core.light(256, 256, 256)
core.actor_walk(world.player, 3, 3)
for _ = 1, 400 do
    t.ticks(1)
end
t.shot("tour_cutaway", 2)
