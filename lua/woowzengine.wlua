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

local test = 0

--local update = lanes.require("update")
--local test_ = lanes.require("update")
--local c_ = require("console")
--local console = lanes.require("wconsole")

--lanes.require("json")
--lanes.require("wbase")
--lanes.require("console")
--lanes.require("sys")

local test_ = lanes.require("wconsole")
--lanes.require("update")

local thread = lanes.gen("*",{package = {}, required = {"update","test","wconsole","wbase","json"}, priority = max_prio},function()
    while true do
        --test_.Test()
        test_.Error("test")
    end
end)()


while (test < 500) do
    test = test + 1
    console.Print("test "..test)
end