local console = nil
local base = require("wbase")
--local ui = require("ui")

local wconsole = {}

wconsole.Install = function()
    if base.Settings.Console then
        console = require("console")
        console.title = "Woowzengine ["..base.Settings.Game.Name.."]"
    end
    wconsole.StartLog()
end

wconsole.GetStartLog = function(base_)
    local base__ = ""
    local base_len = 7
    if not base.String.IsEmpty(base_) then base__ = "["..base.String.Fill(base_,base_len," ").."]" end
    return "["..base.GetDate("h")..":"..base.GetDate("m")..":"..base.GetDate("s")..":"..base.GetDate("ms").."]"..base__.." "
end

wconsole.PrintLog = function(base_,text,color,presymbol)
    if presymbol == nil then presymbol = " " end
    local color_ = color
    if base.String.IsEmpty(color) then color_ = "gray" end
    
    wconsole.Log:open("append")
    local starttext = wconsole.GetStartLog(base_)
    local textresult = starttext..tostring(text)
    wconsole.Log:writeln(presymbol..textresult)
    wconsole.Log:close()
    if base.Settings.Console then console.writecolor(color_,textresult.."\n") end
end

wconsole.DebugMessage = function(text)
    --ui.msg(text,"Debug Message")
end

wconsole.Print = function(text)
    wconsole.PrintLog("PRINT",tostring(text),"white","*")
end

wconsole.Error = function(text)
    wconsole.PrintLog("ERROR",tostring(text),"red","!")
end

wconsole.Warn = function(text)
    wconsole.PrintLog("WARN",tostring(text),"yellow","?")
end

--[[=============================[Логи]===================================]]

wconsole.Log = nil

wconsole.StartLog = function()
    local logname_game = "woowzengine"
    if base.Settings.Game.Name ~= "New Game" then logname_game = base.String.Replace(base.String.Lower(base.Settings.Game.Name)," ","-") end
    local logname = logname_game.."-log-"..base.GetDate("y").."-"..base.GetDate("mn").."-"..base.GetDate("d").."-"..base.GetDate("h").."-"..base.GetDate("m").."-"..base.GetDate("s")
    wconsole.Log = sys.File(base.PathToGame.."log/"..logname..".txt")
    wconsole.PrintLog("MAIN","Start woowzengine...")
    wconsole.PrintLog("MAIN","Version["..base.Settings.Version.."] Game["..base.Settings.Game.Name.."] GameVersion["..base.Settings.Game.Version.."] GameAuthor["..base.Settings.Game.Author.."]")

    if base.Settings.Console then
        wconsole.PrintLog("CONSOLE","Console enabled!")
    end
end

return wconsole