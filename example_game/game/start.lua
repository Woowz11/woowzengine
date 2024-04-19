CreateWindow("window","Двигающеся окно")
SetWindowMain("window")

Cycle(function()

if(HasWindow("window"))then

local Keys = PressedKeys()
local newx = 0 local newy = 0
local speed = 5

local test_ = Length(Keys)

if(Keys["s"])then
	newy = speed
end
if(Keys["w"])then
	newy = newy - speed
end
if(Keys["d"])then
	newx = speed
end
if(Keys["a"])then
	newx = newx - speed
end

if(Keys["shift"])then
	newx = newx*2.5
	newy = newy*2.5
end

if(newx~=0)then SetWindowX("window",Round(GetWindowX("window")+newx)) end
if(newy~=0)then SetWindowY("window",Round(GetWindowY("window")+newy)) end

end

 end,1)