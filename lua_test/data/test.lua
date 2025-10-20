package.path = package.path .. ";./data/?.lua"

-- モジュールを読み込む
local json = require("dkjson")

function generateText()
    -- このLuaファイルと同じ場所のcard_test.jsonを読む
    local script_path = debug.getinfo(1, "S").source:match("@(.*[\\/])")
    local file = io.open(script_path .. "card_test.json", "r")
    if not file then
        return "カードデータが見つかりません。(" .. tostring(script_path .. "card_test.json") .. ")"
    end

    local jsonText = file:read("*a")
    file:close()

    local obj, pos, err = json.decode(jsonText)
    if err then
        return "JSONの解析に失敗: " .. err
    end

    local action = obj.action
    local card = obj.card
    local effectMap = { ["攻撃"]="ダメージ", ["回復"]="回復" }
    local effectText = effectMap[action["do"]] or action["do"]

    return string.format("%sが%sで%sに%d%sの%s", 
        action.who, card.name, action.target, action.many, effectText, action["do"])
end