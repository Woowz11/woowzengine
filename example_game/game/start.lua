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

local add = 2.5

local make = false
CycleRender(function() 
	CameraMovement() 
end)
Cycle(function() SetWindowTitle("window","New Window! ("..GetFPS()..")") end,1000)
 
--[[local x = 0
 local y = 0
 local height = 33
 local count = height*height
 local addthis = 2
 for i=-1,count do
 
	x = x + 1
	if(x>height)then
		x = 0
		y = y + 1
	end
 
	CreateSprite("sprite"..i,"scene")
	SetSpriteColor("scene","sprite"..i,Color.new(Round((x/height)*255),Round(((height-x)/height)*255),Round((y/(count/height))*255)))
	SetSpritePosition("scene","sprite"..i,Vector2.new(x*addthis,y*addthis))
	SetSpriteRotation("scene","sprite"..i,FRRandom(0,380))
	
	local texturemap = {"default","error","test"}
	
	SetSpriteTexture("scene","sprite"..i,Texture.new("F:/woowzengine/example_game/game/engine/"..texturemap[FRRandom(1,#texturemap)]..".png"))
 end]]
 
 --[[Cycle(function()
    Pairs(GetSprites("scene"),function(key,value,index) 
        SetSpriteColor("scene",value,GetSpriteColor("scene",value):Invert())
		local pos = GetSpritePosition("scene",value)
		SetSpritePosition("scene",value,Vector2.new(pos.x + FRandom(-0.25,0.25),pos.y + FRandom(-0.25,0.25)))
    end)
 end,1000)]]
 
CreateSprite("sprite","scene")
SetSpriteTexture("scene","sprite",Texture.new("F:/woowzengine/example_game/game/engine/error.png"))

Print(GetVolume())
--SetVolume(0)