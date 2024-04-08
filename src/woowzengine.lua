local log = require("logger")
log.StartLog()

local game = require("start")
game.start()


local update = require("update")
local base = require("wbase")
--[[async(function(to)
    while base.Time <= to do
        sleep(1)
        base.Time = base.Time + 1
        update.Update()
    end
    return nil
end,math.huge)]]