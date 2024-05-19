CreateWindow("window","")
SetWindowMain("window")
CreateScene("scene")
SetSceneBackgroundColor("scene",Color.new(128,0,0,0))
SetWindowScene("window","scene")
 
function CameraMovement()
	local PK = PressedKeys()
	
	local CameraSpeed = 0.05
	
	if(PK["shift"])then
		CameraSpeed = 0.2
	end
	
	local X = 0
	local Y = 0
	
	if(PK["w"])then
		Y = Y + CameraSpeed
	end
	if(PK["s"])then
		Y = Y - CameraSpeed
	end
	
	if(PK["d"])then
		X = X + CameraSpeed
	end
	if(PK["a"])then
		X = X - CameraSpeed
	end
	
	if (X~=0 or Y~=0) then
		local CameraPosition = GetCameraPosition("scene")
		SetCameraPosition("scene",Vector2.new(CameraPosition.x + X,CameraPosition.y + Y))
	end
end
SetCameraZoom("scene",0.25)

CycleRender(function() 
	CameraMovement() 
end)
Cycle(function() SetWindowTitle("window","New Window! ("..GetFPS()..")") end,1000)

local add = 2.5
local x = -add
local y = 0
function test(i)
	local sprite = "sprite"..i
	CreateSprite(sprite,"scene")

	--[[x = x + add
	if(x>(10*add))then
		x = add
		y = y + add
	end

	SetSpritePosition("scene",sprite,Vector2.new(x,y))
	SetCameraPosition("scene",Vector2.new(x,y))]]
	PrintFast(i.." | "..x.." | "..y)
end

 Repeat(function(i,total)
	test(i)
 end,1000,50)