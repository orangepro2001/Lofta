-- Entry points the hosts call. Everything else hangs off the screen stack in flow.lua.
local flow = require("flow")
local game = require("game")

local M = {}

function M.on_start()
    game.new_game(1)
end

function M.on_new_game(seed)
    game.new_game(seed or 1)
end

function M.on_tick()
    flow.tick()
end

function M.on_draw()
    flow.draw()
end

function M.on_tap(x, y)
    flow.tap(x, y)
end

function M.on_key_down(key)
    flow.key(key)
end

function M.on_key_up(_)
end

return M
