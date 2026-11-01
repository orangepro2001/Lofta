-- The dialogue machine. Compiled nodes are flat lists of operations with jump targets, so the
-- whole position in a conversation is a node id and a number, and it saves as such.
local content = require("content_data")
local story = require("story")

local M = { node = nil, pc = 1 }

local function ops()
    return content.nodes[M.node]
end

-- Runs until something needs the player. Returns {kind="line"|"choice"} or nil at the end.
local function run()
    local guard = 0
    while M.node do
        guard = guard + 1
        if guard > 10000 then
            error("dialogue loop in " .. M.node)
        end
        local list = ops()
        if list == nil then
            error("unknown dialogue node " .. tostring(M.node))
        end
        local op = list[M.pc]
        if op == nil then
            M.node = nil
            return nil
        end
        local name = op[1]
        if name == "say" then
            return { kind = "line", speaker = op[2], text = story.fill(op[3]) }
        elseif name == "do" then
            story.apply(op[2])
            M.pc = M.pc + 1
        elseif name == "jf" then
            if story.test(op[2]) then
                M.pc = M.pc + 1
            else
                M.pc = op[3]
            end
        elseif name == "jmp" then
            M.pc = op[2]
        elseif name == "goto" then
            M.node = op[2]
            M.pc = 1
            story.s.seen[M.node] = true
        elseif name == "end" then
            M.node = nil
            return nil
        elseif name == "choice" then
            local options = {}
            for _, o in ipairs(op[2]) do
                local open = story.test(o.cond) and not (o.once and story.s.picked[o.once])
                if open then
                    options[#options + 1] = o
                end
            end
            if #options == 0 then
                M.pc = op[3]
            else
                return { kind = "choice", options = options }
            end
        else
            error("unknown dialogue op " .. tostring(name))
        end
    end
    return nil
end

function M.start(node)
    M.node = node
    M.pc = 1
    story.s.seen[node] = true
    return run()
end

-- After a line.
function M.next()
    M.pc = M.pc + 1
    return run()
end

-- After a choice: option is one of the tables run() returned.
function M.choose(option)
    if option.once then
        story.s.picked[option.once] = true
    end
    M.pc = option.pc
    return run()
end

function M.position()
    return { node = M.node or "", pc = M.pc }
end

function M.resume(pos)
    if pos.node == "" then
        M.node = nil
        return nil
    end
    M.node = pos.node
    M.pc = pos.pc
    return run()
end

return M
