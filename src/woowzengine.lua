local base = require("wbase")
base.Settings.Install()
local console = require("wconsole")
console.Install()

--local otherlibs = require("otherlibs")
--otherlibs.Install()

local game = require("start")
game.start()

local update = require("update")
--[[while base.Time < math.huge do
    update.Update()
    base.Time = base.Time + 1
end]]

local lanes = require("lanes").configure()
local thread = lanes.gen("*",function()--(test2)
    while true do
        os.execute("sleep 2")
        os.execute("msg %username% Your message here")
    end
    --[[local test1 = 0
    while (test1 < 300) do
        test1 = test1 + 1
        test2("!!!!!!test "..test1)
    end]]
    --console.Print("FFFFFFFFFFFFFFFFFFFFFFFFFF")
end)()--(console.Print)

local test = 0
while (test < 300) do
    test = test + 1
    console.Print("test "..test)
end