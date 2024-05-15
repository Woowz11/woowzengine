local test = {

["run"] = function()


CreateWindow("window","",500,500)
SetWindowMain("window")
CreateScene("scene")
CreateSprite("sprite","scene")
SetSpritePosition("scene","sprite",Vector2.new(-1,1))

CreateScene("scene2")

function SetCameraPosition(x,y,zoom)
	SetCameraX("scene",x)
    SetCameraY("scene",y)
	
	SetCameraX("scene2",x)
    SetCameraY("scene2",y)
	
	SetCameraZoom("scene",zoom)
	SetCameraZoom("scene2",zoom)
end

local selectedscene = "scene"
SetWindowScene("scene","window")
SetWindowEventKeyPress("window",function(key) 
if key=="c" then
	if selectedscene == "scene" then 
        selectedscene = "scene2" 
    else 
        selectedscene = "scene" 
    end
	SetWindowScene(selectedscene,"window")
end

function setsize(size)
	SetWindowSizeX("window",size)
	SetWindowSizeY("window",size)
end

if key=="1" then
	setsize(200)
end
if key=="2" then
	setsize(300)
end
if key=="3" then
	setsize(400)
end
if key=="4" then
	setsize(500)
end
if key=="5" then
	setsize(600)
end
if key=="6" then
	setsize(700)
end
if key=="7" then
	setsize(800)
end
if key=="8" then
	setsize(900)
end
if key=="9" then
	setsize(1000)
end
if key=="0" then
	SetCameraPosition(0,0,0.5)
end
if key=="q" then
	SetCameraPosition(0,0,0.25)
end
if key=="e" then
	SetCameraPosition(0,0,0.125)
end
if key=="r" then
	SetCameraPosition(0,0,0)
end
end)

local x = 0
local y = 1
for i=1,100 do
	CreateSprite("sprite"..i,"scene2")
	
	x = x + 1
	if(x>10)then
		y = y + 1
		x = x - 10
	end
	
	SetSpritePosition("scene2","sprite"..i,Vector2.new(x*2,y*2.5))
end

local mousesprites = 0
SetWindowEventMousePress("window",function(mousetype)

if mousetype==1 then
	CreateSprite("mousesprite"..mousesprites,"scene")
	SetSpritePosition("scene","mousesprite"..mousesprites,GetMouseWPosition("window"))
	mousesprites = mousesprites + 1
end

 end)

CycleRender(function()

-------------------------------------

local Keys = PressedKeys()
local VelX = 0
local VelY = 0
local VelZoom = 0

local CameraSpeed = 0.05

if(Keys["shift"])then
    CameraSpeed = 0.2
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

if(Keys["plus"])then
	VelZoom = 1
elseif(Keys["minus"])then
	VelZoom = -1
end

if(VelX~=0 or VelY~=0 or VelZoom~=0)then
	SetCameraPosition(GetCameraX("scene")+VelX,GetCameraY("scene")+VelY, GetCameraZoom("scene")+(VelZoom/100))
end

if(Keys["home"])then
	SetCameraPosition(0,0,1)
end

-------------------------------------

SetSceneBackgroundColor("scene",Color.new(Round(DSin(ActiveTime()/250)*255),0,0))
SetSceneBackgroundColor("scene2",Color.new(0,Round(DSin(ActiveTime()/250)*255),0))

--SetSpritePosition("scene","sprite",Vector2.new(0,Sin(ActiveTime()/100)))

end)

Cycle(function() SetWindowTitle("window","Window ("..GetFPS()..")")

 end,1000)



end

}

return test