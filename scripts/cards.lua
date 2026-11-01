-- Clue cards and the three verbs of reasoning. Each verb is a different act, and a recipe says
-- which card fills which part, by what the card asserts, not by which card it is.
--   deduce: a rule and a case. The form may be sound or a fallacy; nothing here says which.
--   induce: a table of cases, and the column the player claims they all share.
--   rebut:  a rule under attack, and a case that does not fit it.
local content = require("content_data")
local story = require("story")

local M = {}

function M.card(id)
    return content.cards[id]
end

function M.owned()
    local list = {}
    for _, id in ipairs(story.s.cards) do
        if not story.s.flags["spent_" .. id] then
            list[#list + 1] = id
        end
    end
    return list
end

local function carries(card_id, tag)
    for _, t in ipairs(content.cards[card_id].tags or {}) do
        if t == tag then
            return true
        end
    end
    return false
end

local function perform(recipe)
    story.s.made[recipe.id] = true
    if recipe.output then
        story.give(recipe.output)
    end
    for _, id in ipairs(recipe.consumes or {}) do
        story.s.flags["spent_" .. id] = true
    end
    for _, id in ipairs(recipe.retires or {}) do
        story.s.flags["unproved_" .. id] = true
    end
    for _, e in ipairs(recipe.effects or {}) do
        story.apply(e)
    end
    return recipe
end

-- Feedback when no recipe fits. "none": unrelated. "half": this rule is the right tool, this
-- case is not its situation. The count only feeds playtest logs; it changes nothing in the game.
local function miss(rule)
    story.s.vars.failed_syntheses = story.var("failed_syntheses") + 1
    for _, recipe in ipairs(content.recipes) do
        if (recipe.major == rule or recipe.target == rule) and story.test(recipe.cond) then
            return nil, "half"
        end
    end
    return nil, "none"
end

local function find_deduce(rule, case)
    for _, recipe in ipairs(content.recipes) do
        if recipe.verb == "deduce" and recipe.major == rule and carries(case, recipe.minor)
            and story.test(recipe.cond) then
            return recipe
        end
    end
    return nil
end

-- Either card may be the rule. Trying both orders means the player does not have to remember
-- which one to click first.
function M.deduce(rule, case)
    local recipe = find_deduce(rule, case) or find_deduce(case, rule)
    if recipe then
        return perform(recipe)
    end
    return miss(rule)
end

local function find_rebut(rule, case)
    for _, recipe in ipairs(content.recipes) do
        if recipe.verb == "rebut" and carries(case, recipe.counter)
            and story.test(recipe.cond) then
            if recipe.target_tag and carries(rule, recipe.target_tag) then
                return recipe
            end
            if recipe.target and recipe.target == rule then
                return recipe
            end
        end
    end
    return nil
end

function M.rebut(rule, case)
    local recipe = find_rebut(rule, case) or find_rebut(case, rule)
    if recipe then
        return perform(recipe)
    end
    return miss(rule)
end

-- The rows of a case table the player holds cards for.
function M.table_rows(table_id)
    local rows = {}
    for _, row in ipairs(content.tables[table_id].row) do
        if story.has(row.card) then
            rows[#rows + 1] = row
        end
    end
    return rows
end

-- The player circles a column. "short": the idea is right but there are too few cases yet.
function M.induce(table_id, column)
    local count = #M.table_rows(table_id)
    for _, recipe in ipairs(content.recipes) do
        if recipe.verb == "induce" and recipe.table == table_id and recipe.column == column
            and story.test(recipe.cond) then
            if count >= (recipe.min or 3) then
                return perform(recipe)
            end
            return nil, "short"
        end
    end
    return nil, "none"
end

-- What "examine" shows: the form only, never a verdict.
function M.form(recipe)
    if recipe.form then
        return recipe.form
    end
    if recipe.verb == "induce" then
        return "这几例都有此项；所以凡此类都有此项。"
    elseif recipe.verb == "rebut" then
        return "规律说凡此类皆然；这一例不然；所以规律要改。"
    end
    return "若甲则乙；……；所以……"
end

return M
