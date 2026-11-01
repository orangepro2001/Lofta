-- Starting a game, saving it, loading it.
local codec = require("codec")
local dialogue = require("dialogue")
local flow = require("flow")
local story = require("story")
local world = require("screens.world")

local M = {}

local SAVE_MAGIC = "LOFTA1\n"

function M.new_game(seed)
    core.init(seed)
    story.new()
    flow.reset()
    world.load("demo_bowstreet")
    flow.push(world)
end

function M.save(name)
    local body = codec.encode({
        story = story.s,
        world = world.save(),
        dialogue = dialogue.position(),
    })
    local blob = core.state_blob()
    return sys.write(name, SAVE_MAGIC .. string.pack("<I4", #body) .. body .. blob)
end

function M.load(name)
    local data = sys.read(name)
    if data == nil or data:sub(1, #SAVE_MAGIC) ~= SAVE_MAGIC then
        return false
    end
    local length = string.unpack("<I4", data, #SAVE_MAGIC + 1)
    local start = #SAVE_MAGIC + 5
    local state = codec.decode(data:sub(start, start + length - 1))
    if not core.state_load(data:sub(start + length)) then
        return false
    end
    story.s = state.story
    flow.reset()
    world.restore(state.world)
    flow.push(world)
    return true
end

return M
