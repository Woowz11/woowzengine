CreateWindow("glew","GLEW")
local scene = Scene.new("glew")
scene:SetBackgroundColor(Color.new(0,125,125))
SetWindowScene("glew",scene)
local sprite = Sprite.new("sprite1")

scene:AddSprite(sprite)

CreateWindow("test","NO GLEW")
local scene2 = Scene.new("test")
scene2:SetBackgroundColor(Color.new(0,0,0))
SetWindowScene("test",scene2)
local sprite2 = Sprite.new("sprite2")
sprite2.Blur = true
sprite2.Texture = "F:/woowzengine/example_game/game/engine/test.png"

scene2:AddSprite(sprite2)
