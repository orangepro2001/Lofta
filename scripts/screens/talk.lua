-- A conversation over the world: one line at a time, then choices. Tap anywhere to go on.
local cards = require("cards")
local dialogue = require("dialogue")
local flow = require("flow")
local story = require("story")
local ui = require("ui")

local M = { overlay = true, step = nil, shown = 0, notice = nil, notice_time = 0 }

local BOX_X, BOX_Y, BOX_W, BOX_H = 16, 238, 608, 110
local REVEAL_PER_TICK = 2
local NOTICE_TICKS = 150

function M.begin(node)
    M.step = dialogue.start(node)
    M.shown = 0
    if M.step then
        flow.push(M)
    end
end

function M.enter()
    story.on_card = function(id)
        local card = cards.card(id)
        M.notice = "得到线索：" .. card.title
        M.notice_time = NOTICE_TICKS
    end
end

function M.leave()
    story.on_card = nil
end

local function advance(next_step)
    M.step = next_step
    M.shown = 0
    if M.step == nil then
        flow.pop()
    end
end

function M.tick()
    if M.step and M.step.kind == "line" then
        M.shown = M.shown + REVEAL_PER_TICK
    end
    if M.notice_time > 0 then
        M.notice_time = M.notice_time - 1
    end
end

function M.draw()
    ui.begin()
    core.draw_dither(0, BOX_Y - 6, core.fb_w, core.fb_h - BOX_Y + 6, "shade")
    ui.panel(BOX_X, BOX_Y, BOX_W, BOX_H)
    local s = M.step
    if s == nil then
        return
    end
    if s.kind == "line" then
        if s.speaker ~= "" then
            core.draw_text(BOX_X + 12, BOX_Y + 8, s.speaker, "hilite")
        end
        core.text_box(BOX_X + 12, BOX_Y + 26, BOX_W - 24, s.text, "paper", M.shown)
    else
        for i, option in ipairs(s.options) do
            ui.button(BOX_X + 12, BOX_Y + 6 + (i - 1) * 24, BOX_W - 24, 22, option.text, function()
                advance(dialogue.choose(option))
            end)
        end
    end
    if M.notice_time > 0 then
        local w = core.text_width(M.notice)
        ui.panel(core.fb_w // 2 - w // 2 - 10, BOX_Y - 28, w + 20, 22)
        core.draw_text(core.fb_w // 2 - w // 2, BOX_Y - 24, M.notice, "good")
    end
end

local function finish_or_next()
    local s = M.step
    if s.kind ~= "line" then
        return
    end
    if M.shown < core.text_length(s.text) then
        M.shown = core.text_length(s.text)
    else
        advance(dialogue.next())
    end
end

function M.tap(x, y)
    if M.step and M.step.kind == "choice" then
        ui.tap(x, y)
    elseif M.step then
        finish_or_next()
    end
end

function M.key(key)
    if key == "space" or key == "return" then
        if M.step and M.step.kind == "line" then
            finish_or_next()
        end
    elseif M.step and M.step.kind == "choice" then
        local i = math.tointeger(tonumber(key))
        if i and M.step.options[i] then
            advance(dialogue.choose(M.step.options[i]))
        end
    end
end

return M
