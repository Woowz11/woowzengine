local scene = Scene.new("scene")
local sprite = Sprite.new("sprite")
scene:AddSprite(sprite);

local spriteUI = Sprite.new("ui")
spriteUI.ThatUI = true
spriteUI.AutoSize = true
spriteUI.Position.x = 0.5
spriteUI.Size.x = 0.25
spriteUI.Size.y = 0.25
spriteUI.Color = Color.new(255,0,0)
scene:AddSprite(spriteUI)

CreateWindow("window","Camera movement")
SetWindowScene("window",scene)
SetWindowMain("window")

Cycle(function()
local Keys = PressedKeys()
local VelX = 0
local VelY = 0

local CameraSpeed = 0.005

if(Keys["shift"])then
	CameraSpeed = 0.05
end

if(Keys["w"])then
	VelY = VelY - CameraSpeed
end
if(Keys["s"])then
	VelY = VelY + CameraSpeed
end
if(Keys["d"])then
	VelX = VelX - CameraSpeed
end
if(Keys["a"])then
	VelX = VelX + CameraSpeed
end

if(VelX~=0 or VelY~=0)then
	SetCameraX(scene,GetCameraX(scene)+VelX)
	SetCameraY(scene,GetCameraY(scene)+VelY)
end

sprite.Position.y = Sin(ActiveTime()/250)
UpdateSprite(scene,sprite)

 end,1)