-- Screen stack. The top screen gets input and ticks; every screen draws, bottom first, down
-- to the nearest one that covers the whole frame.
local M = { stack = {} }

function M.reset()
    M.stack = {}
end

function M.push(screen)
    M.stack[#M.stack + 1] = screen
    if screen.enter then
        screen.enter()
    end
end

function M.pop()
    local screen = M.stack[#M.stack]
    M.stack[#M.stack] = nil
    if screen and screen.leave then
        screen.leave()
    end
end

function M.top()
    return M.stack[#M.stack]
end

function M.tick()
    local screen = M.top()
    if screen and screen.tick then
        screen.tick()
    end
end

function M.draw()
    local first = 1
    for i = #M.stack, 1, -1 do
        if not M.stack[i].overlay then
            first = i
            break
        end
    end
    for i = first, #M.stack do
        M.stack[i].draw()
    end
end

function M.tap(x, y)
    local screen = M.top()
    if screen and screen.tap then
        screen.tap(x, y)
    end
end

function M.key(key)
    local screen = M.top()
    if screen and screen.key then
        screen.key(key)
    end
end

return M
