-- Assertions and input for scenario scripts. Failures are reported and counted; finish()
-- returns the count to the host, which turns it into the exit code.
local M = { failures = 0, checks = 0 }

function M.check(condition, message)
    M.checks = M.checks + 1
    if not condition then
        M.failures = M.failures + 1
        host.report("FAIL " .. tostring(message))
    end
end

function M.eq(actual, expected, message)
    M.check(actual == expected, tostring(message) .. ": got " .. tostring(actual) .. ", expected " .. tostring(expected))
end

function M.ticks(n)
    for _ = 1, n do
        main.on_tick()
        core.tick()
    end
end

function M.draw()
    host.frame_begin()
    main.on_draw()
end

function M.shot(name, scale)
    M.draw()
    host.shot(name, scale or 1)
end

function M.tap(x, y)
    M.draw() -- buttons exist only once drawn
    main.on_tap(x, y)
end

function M.key(k)
    main.on_key_down(k)
end

function M.finish(script)
    host.print(string.format("%s: %d checks, %d failed", script, M.checks, M.failures))
    return M.failures
end

return M
