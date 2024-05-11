CreateWindow("window","Window")
SetWindowMain("window")
CreateScene("scene")
CreateSprite("sprite","scene")

CreateScene("scene2")

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

end)

Cycle(function()

--Print(Round(DSin(ActiveTime()/250)*255))
SetSceneBackgroundColor("scene",Color.new(Round(DSin(ActiveTime()/250)*255),0,0))
SetSceneBackgroundColor("scene2",Color.new(0,Round(DSin(ActiveTime()/250)*255),0))

SetSpritePosition("scene","sprite",Vector2.new(0,Sin(ActiveTime()/100)))

end,1)
