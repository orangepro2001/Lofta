-- Narrative state: everything the story remembers, in plain tables of strings and integers.
-- Conditions and effects are data compiled from content/, evaluated here and nowhere else.
local M = { s = nil }

function M.new()
    M.s = {
        chapter = 0,
        slot = 0,      -- time slots spent in the chapter
        flags = {},
        vars = {},
        cards = {},    -- ids in the order found
        made = {},     -- recipe ids already performed
        seen = {},     -- dialogue nodes visited
        picked = {},   -- once-only choices taken
        ending = "",
    }
    return M.s
end

function M.has(card)
    for _, id in ipairs(M.s.cards) do
        if id == card then
            return true
        end
    end
    return false
end

function M.give(card)
    if M.has(card) then
        return false
    end
    M.s.cards[#M.s.cards + 1] = card
    return true
end

function M.var(name)
    return M.s.vars[name] or 0
end

local COMPARE = {
    [">="] = function(a, b) return a >= b end,
    ["<="] = function(a, b) return a <= b end,
    [">"] = function(a, b) return a > b end,
    ["<"] = function(a, b) return a < b end,
    ["=="] = function(a, b) return a == b end,
    ["!="] = function(a, b) return a ~= b end,
}

function M.test(c)
    if c == nil then
        return true
    end
    local op = c[1]
    if op == "and" then
        return M.test(c[2]) and M.test(c[3])
    elseif op == "or" then
        return M.test(c[2]) or M.test(c[3])
    elseif op == "not" then
        return not M.test(c[2])
    elseif op == "has" then
        return M.has(c[2])
    elseif op == "flag" then
        return M.s.flags[c[2]] == true
    elseif op == "seen" then
        return M.s.seen[c[2]] == true
    elseif op == "made" then
        return M.s.made[c[2]] == true
    elseif op == "var" then
        return COMPARE[c[3]](M.var(c[2]), c[4])
    end
    error("unknown condition " .. tostring(op))
end

local FUND = 31137

-- The shares of living, verified Lives split the fund. See docs/story/02-truth.md, section 11.
function M.compute_fund()
    local f = M.s.flags
    local function one(flag)
        return f[flag] and 1 or 0
    end
    local ruth = 1 + one("lucy_alive") + one("will_alive")
    local others = (f.lyle_guilty and 0 or 1)
    if not f.pereira_hanged then
        others = others + one("kitty_alive") + one("ned_alive")
    end
    local total = ruth + others
    M.s.vars.fund_shares = ruth
    M.s.vars.fund_total = total
    M.s.vars.fund_ruth = FUND * ruth // total
    M.s.vars.fund_each = FUND // total
end

-- Replaces {name} in a line with that variable.
function M.fill(text)
    return (text:gsub("{([%w_]+)}", function(name)
        return tostring(M.var(name))
    end))
end

-- Effects return nothing; a card gained is reported through on_card so screens can show it.
M.on_card = nil

function M.apply(e)
    local op = e[1]
    if op == "card" then
        if M.give(e[2]) and M.on_card then
            M.on_card(e[2])
        end
    elseif op == "flag" then
        M.s.flags[e[2]] = true
    elseif op == "clear" then
        M.s.flags[e[2]] = nil
    elseif op == "add" then
        M.s.vars[e[2]] = M.var(e[2]) + e[3]
    elseif op == "set" then
        M.s.vars[e[2]] = e[3]
    elseif op == "time" then
        M.s.slot = M.s.slot + e[2]
    elseif op == "chapter" then
        M.s.chapter = e[2]
        M.s.slot = 0
    elseif op == "ending" then
        M.s.ending = e[2]
    elseif op == "fund" then
        M.compute_fund()
    else
        error("unknown effect " .. tostring(op))
    end
end

return M
