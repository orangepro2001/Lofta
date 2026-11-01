-- The clue board: pick cards, then combine them.
local cards = require("cards")
local flow = require("flow")
local ui = require("ui")

local M = { overlay = false, picked = {}, result = nil }

local COLUMNS = 3
local CARD_W, CARD_H = 196, 58

local KIND_NAMES = { fact = "事实", rule = "规律", inference = "推论", testimony = "证词", fallacy = "推论" }
local KIND_COLORS = { fact = "paper", rule = "rule", inference = "good", testimony = "paper_dim", fallacy = "warn" }

function M.enter()
    M.picked = {}
    M.result = nil
end

local function is_picked(id)
    for _, p in ipairs(M.picked) do
        if p == id then
            return true
        end
    end
    return false
end

local function toggle(id)
    for i, p in ipairs(M.picked) do
        if p == id then
            table.remove(M.picked, i)
            return
        end
    end
    M.picked[#M.picked + 1] = id
end

local MISS_TEXT = {
    none = "这两张牌说的不是一回事。",
    half = "这条规律用得上，但这件事不是它说的情形。",
}

-- Deduction from the board: a rule and one case. The full interface with the other two verbs
-- comes in milestone M1; docs/story/04-systems.md describes it.
local function combine()
    if #M.picked ~= 2 then
        M.result = "演绎要一条规律和一件事。"
        return
    end
    local a, b = M.picked[1], M.picked[2]
    local rule, case = a, b
    if cards.card(b).kind == "rule" then
        rule, case = b, a
    end
    local recipe, miss = cards.deduce(rule, case)
    M.picked = {}
    M.result = recipe and recipe.explain or MISS_TEXT[miss]
end

function M.draw()
    ui.begin()
    core.draw_rect(0, 0, core.fb_w, core.fb_h, "panel")
    core.draw_text(12, 8, "线索板", "hilite")
    core.draw_text(70, 8, "选一条规律和一件事，点“合成”", "muted")
    local list = cards.owned()
    for i, id in ipairs(list) do
        local card = cards.card(id)
        local col = (i - 1) % COLUMNS
        local row = (i - 1) // COLUMNS
        local x = 12 + col * (CARD_W + 10)
        local y = 28 + row * (CARD_H + 8)
        local on = is_picked(id)
        ui.button(x, y, CARD_W, CARD_H, "", function()
            toggle(id)
        end, on)
        core.draw_text(x + 6, y + 4, KIND_NAMES[card.kind], KIND_COLORS[card.kind])
        core.draw_text(x + 36, y + 4, card.title, "paper")
        core.text_box(x + 6, y + 20, CARD_W - 12, card.text, "paper_dim", 40)
    end
    ui.button(12, 322, 96, 28, "合成", combine)
    ui.button(116, 322, 96, 28, "返回", function()
        flow.pop()
    end)
    if M.result then
        core.text_box(224, 318, 404, M.result, "paper", -1)
    end
end

function M.tap(x, y)
    ui.tap(x, y)
end

function M.key(key)
    if key == "escape" or key == "c" or key == "tab" then
        flow.pop()
    end
end

return M
