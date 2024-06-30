local cw = {}

cw.CreateWindows = function()

	CreateWindow("Window","WoowzEngine Scene Editor",1600,900)
	SetWindowMain("Window")
	SetWindowImGui("Window")

end

cw.CreateScenes = function()

	CreateScene("Scene")
	SetSceneBackgroundColor("Scene",Black)
	SetWindowScene("Window","Scene")
	
end

return cw