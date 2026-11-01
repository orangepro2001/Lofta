-- A small tagged format for saves. load() is removed from the sandbox on purpose, so a save
-- can never run code. Keys are written sorted: the same state gives the same bytes.
local M = {}

local function key_order(a, b)
    local ta, tb = math.type(a) or type(a), math.type(b) or type(b)
    if ta ~= tb then
        return ta < tb
    end
    return a < b
end

local function encode(value, out)
    local kind = type(value)
    if kind == "boolean" then
        out[#out + 1] = value and "b1" or "b0"
    elseif kind == "number" then
        if math.type(value) ~= "integer" then
            error("saves hold integers only")
        end
        out[#out + 1] = "i" .. value .. ";"
    elseif kind == "string" then
        out[#out + 1] = "s" .. #value .. ":" .. value
    elseif kind == "table" then
        local keys = {}
        for k in pairs(value) do -- sorted below
            keys[#keys + 1] = k
        end
        table.sort(keys, key_order)
        out[#out + 1] = "t" .. #keys .. ";"
        for _, k in ipairs(keys) do
            encode(k, out)
            encode(value[k], out)
        end
    else
        error("cannot save a " .. kind)
    end
end

function M.encode(value)
    local out = {}
    encode(value, out)
    return table.concat(out)
end

local function decode(text, pos)
    local tag = text:sub(pos, pos)
    if tag == "b" then
        return text:sub(pos + 1, pos + 1) == "1", pos + 2
    elseif tag == "i" then
        local stop = text:find(";", pos, true)
        return math.tointeger(tonumber(text:sub(pos + 1, stop - 1))), stop + 1
    elseif tag == "s" then
        local stop = text:find(":", pos, true)
        local length = math.tointeger(tonumber(text:sub(pos + 1, stop - 1)))
        return text:sub(stop + 1, stop + length), stop + 1 + length
    elseif tag == "t" then
        local stop = text:find(";", pos, true)
        local count = math.tointeger(tonumber(text:sub(pos + 1, stop - 1)))
        local result = {}
        local cursor = stop + 1
        for _ = 1, count do
            local k, v
            k, cursor = decode(text, cursor)
            v, cursor = decode(text, cursor)
            result[k] = v
        end
        return result, cursor
    end
    error("bad save at byte " .. pos)
end

function M.decode(text)
    local value = decode(text, 1)
    return value
end

return M
