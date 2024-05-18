--[[local test = require("test")
local i = 0
test.run(function() 

local sin = DSin(ActiveTime()/25)
local cos = DCos(ActiveTime()/25)

PrintFast(ActiveTime())

 end)

CreateSprite("sprite","scene")
SetSpritePosition("scene","sprite",Vector2.new(0,0))

local tbl = {"hi","bye","omg","idk"}

]]
--Cycle(function() Print(tbl[RRandom(1,Length(tbl))]) end,1)

CreateWindow("window","Transparency window")
SetWindowTransparency("window",50)