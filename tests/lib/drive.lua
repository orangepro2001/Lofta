-- Plays by the same inputs a player has: taps at screen positions, found from the core.
local t = require("testlib")
local M = {}

local function screen_of(id)
    -- Scans for the actor under the pointer; a person is about 30 pixels tall at their feet.
    for y = 0, core.fb_h - 1, 4 do
        for x = 0, core.fb_w - 1, 4 do
            if core.pick_actor(x, y) == id then
                return x, y
            end
        end
    end
    return nil
end

function M.tap_actor(id)
    local x, y = screen_of(id)
    t.check(x ~= nil, "actor " .. id .. " is on screen")
    if x then
        t.tap(x, y)
    end
end

function M.tap_cell(cx, cy)
    for y = 0, core.fb_h - 1 do
        for x = 0, core.fb_w - 1, 2 do
            local px, py = core.pick_cell(x, y)
            if px == cx and py == cy then
                t.tap(x, y)
                return true
            end
        end
    end
    return false
end

function M.settle(actor, limit)
    for _ = 1, limit or 2000 do
        if not core.actor_moving(actor) then
            return true
        end
        t.ticks(1)
    end
    return false
end

return M
