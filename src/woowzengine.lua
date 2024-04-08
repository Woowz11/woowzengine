local base = require("wbase")
base.Settings.Install()
local console = require("wconsole")
console.Install()

local game = require("start")
game.start()


local update = require("update")
--[[async(function(to)
    while base.Time <= to do
        sleep(1)
        base.Time = base.Time + 1
        update.Update()
    end
    return nil
end,math.huge)]]