local console = require("console")
local base = require("wbase")

local wconsole = {}

wconsole.Install = function()
    wconsole.StartLog()
end

wconsole.GetStartLog = function(base_)
    local base__ = ""
    if not base.String.IsEmpty(base_) then base__ = "["..base.String.Fill(base_,6," ").."]" end
    return "["..base.GetDate("ms")..":"..base.GetDate("s")..":"..base.GetDate("m")..":"..base.GetDate("h").."]"..base__.." "
end

wconsole.PrintLog = function(base_,text,color)
    local color_ = color
    if base.String.IsEmpty(color) then color_ = "gray" end
    
    wconsole.Log:open("append")
    local starttext = wconsole.GetStartLog(base_)
    local textresult = starttext..tostring(text)
    wconsole.Log:writeln(textresult)
    wconsole.Log:close()
    if base.Settings.Console then console.writecolor(color_,textresult.."\n") end
end

wconsole.Print = function(text)
    wconsole.PrintLog("PRINT",tostring(text),"white")
end

wconsole.Error = function(text)
    wconsole.PrintLog("ERROR",tostring(text),"red")
end

wconsole.Warn = function(text)
    wconsole.PrintLog("WARN",tostring(text),"yellow")
end

--[[=============================[Логи]===================================]]

wconsole.Log = nil

wconsole.StartLog = function()
    local logname = "woowzengine-log-"..base.GetDate("y").."-"..base.GetDate("mn").."-"..base.GetDate("d").."-"..base.GetDate("h").."-"..base.GetDate("m").."-"..base.GetDate("s")
    wconsole.Log = sys.File("log/"..logname..".txt")
    wconsole.PrintLog("MAIN","Start woowzengine...")
    wconsole.PrintLog("MAIN","Version["..base.Settings.Version.."] Game["..base.Settings.Game.Name.."] GameVersion["..base.Settings.Game.Version.."] GameAuthor["..base.Settings.Game.Author.."]")

    wconsole.Print("print")
    wconsole.Warn("warning")
    wconsole.Error("error")
end

return wconsole