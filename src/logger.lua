local base = require("wbase")

local logger = {}

logger.Log = nil

logger.Write = function(base_,text)
    local Date = sys.Datetime()
    logger.Log:open("append")
    local starttext = "["..Date.milliseconds..":"..Date.second..":"..Date.minute..":"..Date.hour.."] "
    logger.Log:writeln(starttext..tostring(text))
    logger.Log:close()
end

logger.StartLog = function()
    local Date = sys.Datetime()
    local logname = "woowzengine-log-"..Date.second.."-"..Date.minute.."-"..Date.hour.."-"..Date.day.."-"..Date.month.."-"..Date.year
    logger.Log = sys.File("log/"..logname..".txt")
    logger.Write(nil,"Start woowzengine...")
end

logger.EndLog = function()
    logger.Write(nil,"Close woowzengine...")
end

return logger