local base = require("wbase")
base.Settings.Install()
local console = require("wconsole")
console.Install()

--local otherlibs = require("otherlibs")
--otherlibs.Install()

local game = require("start")
game.start()

--local update = require("update")
--[[while base.Time < math.huge do
    update.Update()
    base.Time = base.Time + 1
end]]

--[[local lanes = require("lanes").configure({
    verbose_errors = true
})
local update = lanes.require("update")
local thread = lanes.gen("*",function(upd)
    while true do
        upd.Update()
    end
end)(upd)]]

local lanes = require("lanes").configure({
    verbose_errors = true
})

local update = lanes.require("update")
--local test = lanes.require("ui")
local thread = lanes.gen("*",function()
    while true do
        update.Update()
        --test.msg("test")
    end
end)()


local test = 0
while (test < 300) do
    test = test + 1
    console.Print("test "..test)
end