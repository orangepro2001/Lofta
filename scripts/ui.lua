-- Interface pieces. Buttons register while drawing, so what is on screen is what can be hit.
local M = { hits = {} }

function M.begin()
    M.hits = {}
end

function M.panel(x, y, w, h)
    core.draw_rect(x, y, w, h, "panel")
    core.draw_frame(x, y, w, h, "edge")
end

function M.button(x, y, w, h, label, action, active)
    local hover = pointer and pointer.x >= x and pointer.x < x + w and pointer.y >= y and pointer.y < y + h
    core.draw_rect(x, y, w, h, active and "edge" or "panel")
    core.draw_frame(x, y, w, h, (hover or active) and "hilite" or "edge")
    core.draw_text(x + (w - core.text_width(label)) // 2, y + (h - 12) // 2, label, "paper")
    M.hits[#M.hits + 1] = { x = x, y = y, w = w, h = h, action = action }
end

-- Runs the action under a tap. Returns true when a button took it.
function M.tap(x, y)
    for i = #M.hits, 1, -1 do
        local b = M.hits[i]
        if x >= b.x and x < b.x + b.w and y >= b.y and y < b.y + b.h then
            b.action()
            return true
        end
    end
    return false
end

return M
