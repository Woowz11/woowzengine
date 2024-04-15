CreateWindow("glew","GLEW")
local scene = Scene.new("glew")
scene:SetBackgroundColor(Color.new(0,125,125))
SetWindowScene("glew",scene)

CreateWindow("notglew","Not GLEW")
local scene2 = Scene.new("notglew")
scene2:SetBackgroundColor(Color.new(0,255,0))
SetWindowScene("notglew",scene2)


--[[CreateWindow("green","GREEN")
local scene_green = Scene.new("green")
SetWindowScene("green",scene_green)

CreateWindow("blue","BLUE")
local scene_blue = Scene.new("blue")
SetWindowScene("blue",scene_blue)
scene_blue:SetBackgroundColor(Color.new(0,0,255))

scene_green:SetBackgroundColor(Color.new(0,255,0))]]


--[[local scene = Scene.new("scene")
Print(GetSceneWindow(scene))
SetWindowScene("window",scene)
Print(GetSceneWindow(scene))

local i = 0
Cycle(function() 

i = i + 1
local element = Element.new("element"..i)
Print(RRandom(0,255))
element:SetType("triangle",{
    Vertex.new(Vector2.new(Random(-1,1),Random(-1,1)),Color.new(RRandom(0,255),RRandom(0,255),RRandom(0,255))),--RRandom(0,255),RRandom(0,255),RRandom(0,255))),
    Vertex.new(Vector2.new(Random(-1,1),Random(-1,1)),Color.new(255,0,0)),--RRandom(0,255),RRandom(0,255),RRandom(0,255))),
    Vertex.new(Vector2.new(Random(-1,1),Random(-1,1)),Color.new(255,0,0))--RRandom(0,255),RRandom(0,255),RRandom(0,255)))
})
scene:AddElement(element)

end,2000)]]