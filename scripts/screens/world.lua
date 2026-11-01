-- Walking around a scene. Tap the ground to go there, tap a person to go and talk.
local content = require("content_data")
local flow = require("flow")
local story = require("story")
local ui = require("ui")

local M = { map = nil, player = -1, npcs = {}, pending = nil, cut = 0 }

local WALK_SPEED = 7

local function find_map(id)
    local map = content.maps[id]
    if map == nil then
        error("unknown map " .. tostring(id))
    end
    return map
end

local function build_scene(map)
    if not core.scene_begin(map.w, map.h) then
        error("scene too large: " .. map.id)
    end
    for y = 1, map.h do
        local row = map.rows[y]
        for x = 1, map.w do
            local cell = map.legend[row:sub(x, x)]
            core.scene_cell(x - 1, y - 1, cell.ground, cell.height, cell.wall, cell.prop, cell.region, cell.flags)
        end
    end
end

local function spawn_people(map)
    M.npcs = {}
    for _, n in ipairs(map.npcs) do
        local present = story.test(n.cond)
        if present then
            local id = core.actor_spawn(n.outfit, n.x, n.y)
            M.npcs[#M.npcs + 1] = { id = id, data = n }
        end
    end
end

function M.load(id)
    local map = find_map(id)
    M.map = map
    build_scene(map)
    M.player = core.actor_spawn("searcher", map.start[1], map.start[2], WALK_SPEED)
    core.player_set(M.player)
    spawn_people(map)
    M.pending = nil
    M.cut = 0
    story.s.chapter = story.s.chapter
end

function M.save()
    local npcs = {}
    for i, n in ipairs(M.npcs) do
        npcs[i] = { id = n.id, key = n.data.id }
    end
    return { map = M.map.id, player = M.player, npcs = npcs, cut = M.cut, pending = M.pending and M.pending.key or "" }
end

-- The core's own state already holds the scene and the people; only names come back here.
function M.restore(saved)
    M.map = find_map(saved.map)
    M.player = saved.player
    M.cut = saved.cut
    M.npcs = {}
    M.pending = nil
    for i, n in ipairs(saved.npcs) do
        for _, data in ipairs(M.map.npcs) do
            if data.id == n.key then
                M.npcs[i] = { id = n.id, data = data }
                if saved.pending == n.key then
                    M.pending = { key = n.key, actor = n.id, data = data }
                end
            end
        end
    end
end

local function npc_of_actor(actor)
    for _, n in ipairs(M.npcs) do
        if n.id == actor then
            return n
        end
    end
    return nil
end

local function start_talk(n)
    local talk = require("screens.talk")
    local px, py = core.actor_cell(M.player)
    local nx, ny = core.actor_cell(n.id)
    core.actor_face(M.player, nx, ny)
    core.actor_face(n.id, px, py)
    talk.begin(n.data.talk)
end

function M.tick()
    local x, y = core.actor_cell(M.player)
    local region = core.cell_region(x, y)
    if region ~= M.cut then
        if M.cut ~= 0 then
            core.scene_cut_region(M.cut, false)
        end
        if region ~= 0 then
            core.scene_cut_region(region, true)
        end
        M.cut = region
    end
    if M.pending and not core.actor_moving(M.player) then
        local p = M.pending
        M.pending = nil
        start_talk({ id = p.actor, data = p.data })
    end
end

local function draw_labels()
    for _, n in ipairs(M.npcs) do
        if pointer then
            local actor = core.pick_actor(pointer.x, pointer.y)
            if actor == n.id then
                local w = core.text_width(n.data.name)
                core.draw_rect(pointer.x - w // 2 - 3, pointer.y - 20, w + 6, 15, "panel")
                core.draw_text(pointer.x - w // 2, pointer.y - 19, n.data.name, "paper")
            end
        end
    end
end

function M.draw()
    ui.begin()
    core.draw_world()
    draw_labels()
    core.draw_rect(0, 0, core.fb_w, 18, "panel")
    core.draw_text(6, 3, M.map.title, "paper")
    ui.button(core.fb_w - 62, 324, 56, 28, "线索", function()
        flow.push(require("screens.board"))
    end)
end

function M.tap(x, y)
    if ui.tap(x, y) then
        return
    end
    local actor = core.pick_actor(x, y)
    if actor and actor ~= M.player then
        local n = npc_of_actor(actor)
        if n then
            local nx, ny = core.actor_cell(n.id)
            if core.actor_walk(M.player, nx, ny, 1) >= 0 then
                M.pending = { key = n.data.id, actor = n.id, data = n.data }
            end
            return
        end
    end
    local cx, cy = core.pick_cell(x, y)
    if cx then
        M.pending = nil
        core.actor_walk(M.player, cx, cy, 0)
    end
end

local STEPS = {
    up = { -1, -1 }, down = { 1, 1 }, left = { -1, 1 }, right = { 1, -1 },
    w = { -1, -1 }, s = { 1, 1 }, a = { -1, 1 }, d = { 1, -1 },
}

function M.key(key)
    local step = STEPS[key]
    if step then
        local x, y = core.actor_cell(M.player)
        core.actor_walk(M.player, x + step[1], y + step[2], 0)
    elseif key == "c" or key == "tab" then
        flow.push(require("screens.board"))
    end
end

return M
