CreateWindow("Window","omg")
SetWindowMain("Window")
CreateScene("Scene")
SetWindowScene("Window","Scene")
SetSceneBackgroundColor("Scene",Transparent)

CreateSprite("sprite","Scene")

--[[SetWindowImGui("Window")

CreateImGuiWindow("ImGui Window","Title")
CreateImGuiElement("ImGui Text","ImGui Window","Text")
SetImGuiElementText("ImGui Text","Hello!!!")
SetImGuiElementColor("ImGui Text",Red)

CreateImGuiElement("ImGui Text 2","ImGui Window","Text")
SetImGuiElementText("ImGui Text 2","Eto text 2")

CreateImGuiWindow("ImGui Window 2","Title 2")
CreateImGuiElement("ImGui Text_","ImGui Window 2","Text")
SetImGuiElementText("ImGui Text_","Random color...")

CycleRender(function() SetImGuiElementColor("ImGui Text_",RandomColor()) end)

CreateImGuiElement("ImGui Text 2_","ImGui Window 2","Text")
SetImGuiElementText("ImGui Text 2_","Privet!")

CreateImGuiElement("ImGui Button","ImGui Window","Button")
SetImGuiElementText("ImGui Button","Press me!")
SetImGuiElementEvent("ImGui Button",function() Print("hello!") end)

CreateImGuiElement("ImGui Button 2","ImGui Window","Button")
SetImGuiElementText("ImGui Button 2","Add Text")
local i = 0
SetImGuiElementEvent("ImGui Button 2",function()
	CreateImGuiElement("ImGuiText"..i,"ImGui Window 2","Text")
	SetImGuiElementText("ImGuiText"..i,"Text number №"..i)
	i = i + 1
end)

Print(EngineChars)]]

CreateText("text","Scene","TEXT █▓▒░卍卐◀▶▲▼▞▌▐▀▄▖▗▘▝▙▟▛▜ ТЕКСТ")