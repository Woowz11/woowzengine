CreateWindow("window","")
SetWindowMain("window")
CreateScene("scene")
SetSceneBackgroundColor("scene",Color.new(0,0,0,255))
SetWindowScene("window","scene")
 
function CameraMovement()
	local PK = PressedKeys()
	
	local CameraSpeed = 0.05
	
	if(PK["shift"])then
		CameraSpeed = 0.2
	end
	
	if(PK["ctrl"])then
		CameraSpeed = 0.005
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
		SetCameraPosition("scene",Vector2.new(CameraPosition.x + (X/GetCameraZoom("scene")),CameraPosition.y + (Y/GetCameraZoom("scene"))))
	end
	
	local ZoomSpeed = 0.05
	local Zoom = 1
	
	if(PK["plus"])then
		Zoom = Zoom + ZoomSpeed
	end
	if(PK["minus"])then
		Zoom = Zoom - ZoomSpeed
	end
	
	if(Zoom~=1)then
		SetCameraZoom("scene",GetCameraZoom("scene") * Zoom)
	end
end
SetCameraZoom("scene",1)

CycleRender(function() 
	CameraMovement() 
	--SetSpriteRotation("scene","sprite",ToDeg(ActiveTime()/10))
end)

local add = 2.5

local make = false
 
local x = 0
local y = 0
local height = 20
local count = height*height
local addthis = 2

LoadTexture("texture","F:/woowzengine/example_game/game/test.png")
SetTextureBlur("texture",true)
LoadTexture("texture1","F:/woowzengine/example_game/game/test1.png")
SetTextureBlur("texture1",true)
LoadTexture("texture2","F:/woowzengine/example_game/game/test2.png")
SetTextureBlur("texture2",true)

local textures = {"texture","texture1","texture2"}

for i=0,count do
 
	x = x + 1
	if(x>height)then
		x = 0
		y = y + 1
	end
 
	CreateSprite("sprite"..i,"scene")
	SetSpritePosition("scene","sprite"..i,Vector2.new(FRandom(-1,1)*10000000,FRandom(-1,1)*10000000))
	
	--if(FRRandom(0,1)==1)then
		--SetSpriteShader("scene","sprite"..i,"test")
		--SetSpriteLayer("scene","sprite"..i,2)
		SetSpriteTexture("scene","sprite"..i,textures[FRRandom(1,#textures)])
		SetSpriteRotation("scene","sprite"..i,FRRandom(0,360))
		local size = FRandom(1000,100000)
		SetSpriteColor("scene","sprite"..i,Color.new(FRRandom(128,255),FRRandom(128,255),FRRandom(128,255)))
		SetSpriteSize("scene","sprite"..i,Vector2.new(size,size))
		SetSpriteHeight("scene","sprite"..i,FRandom(-5,5))
	--end
end
 
--[[LoadTexture("texturebase","F:/woowzengine/example_game/game/engine/test.png",true)

local colors = GetTextureColors("texturebase")
for i=1,#colors do
	colors[i] = colors[i]:Invert()
end
 
CreateSprite("sprite","scene")

local size = GetTextureSize("texturebase")

CreateTexture("texture",size.x,size.y,colors)
SetSpriteTexture("scene","sprite","texture")]]