CreateWindow("Window","omg")
SetWindowMain("Window")
CreateScene("Scene")
SetWindowScene("Window","Scene")
SetSceneBackgroundColor("Scene",Transparent)

CreateSprite("sprite","Scene")

SetWindowImGui("Window")

CreateImGuiWindow("ImGui Window","Title")
CreateImGuiElement("ImGui Text","ImGui Window","Text")
SetImGuiElementText("ImGui Text","Hello!!!")
SetImGuiElementColor("ImGui Text",Red)

CreateImGuiElement("ImGui Text 2","ImGui Window","Text")
SetImGuiElementText("ImGui Text 2","Eto text 2")

CreateImGuiWindow("ImGui Window 2","Title 2")
CreateImGuiElement("ImGui Text_","ImGui Window 2","Text")
SetImGuiElementText("ImGui Text_","Random color...")

--CycleRender(function() SetImGuiElementColor("ImGui Text_",RandomColor()) end)

CreateImGuiElement("ImGui Text 2_","ImGui Window 2","Text")
SetImGuiElementText("ImGui Text 2_","Privet!")

CreateImGuiElement("ImGui Button","ImGui Window","Button")
SetImGuiElementText("ImGui Button","Press me!")
SetImGuiElementEvent("ImGui Button",function() Print("hello!") end)

CreateImGuiElement("ImGui Button 3","ImGui Window","Small Button")
SetImGuiElementText("ImGui Button 3","Small")
SetImGuiElementEvent("ImGui Button 3",function() Print("small hello!") end)

CreateImGuiElement("ImGui Button left","ImGui Window","Left Arrow Button")
SetImGuiElementEvent("ImGui Button left",function() Print("left hello!") end)

CreateImGuiElement("ImGui Button right","ImGui Window","Right Arrow Button")
SetImGuiElementEvent("ImGui Button right",function() Print("right hello!") end)

CreateImGuiElement("ImGui Button 2","ImGui Window","Button")
SetImGuiElementText("ImGui Button 2","Add Text")
local i = 0
SetImGuiElementEvent("ImGui Button 2",function()
	CreateImGuiElement("ImGuiText"..i,"ImGui Window 2","Text")
	SetImGuiElementText("ImGuiText"..i,"Text number #"..i)
	i = i + 1
end)

CreateImGuiElement("ImGui checkbox","ImGui Window","Checkbox")
SetImGuiElementText("ImGui checkbox","That checkbox")
SetImGuiElementEvent("ImGui checkbox",function(bool) Print(bool) end)

CreateImGuiElement("ImGui checkbox 2","ImGui Window","Checkbox")
SetImGuiElementText("ImGui checkbox 2","That checkbox 2")
SetImGuiElementEvent("ImGui checkbox 2",function(bool) Print(ToString(bool).." 2") end)

CreateImGuiElement("ImGui test","ImGui Window","Test")

CreateImGuiElement("ImGui Text point","ImGui Window","Text")
SetImGuiElementText("ImGui Text point","Text with point")
SetImGuiElementPoint("ImGui Text point",true)

CreateImGuiElement("ImGui Button point","ImGui Window","Button")
SetImGuiElementText("ImGui Button point","Button with point")
SetImGuiElementEvent("ImGui Button point",function() Print("point hello!") end)
SetImGuiElementPoint("ImGui Button point",true)

CreateImGuiElement("ImGui checkbox point","ImGui Window","Checkbox")
SetImGuiElementText("ImGui checkbox point","Checkbox with point")
SetImGuiElementEvent("ImGui checkbox point",function(bool) Print("point "..ToString(bool)) end)
SetImGuiElementPoint("ImGui checkbox point",true)

CreateImGuiElement("ImGui list","ImGui Window","List")
SetImGuiElementText("ImGui list","List")
SetImGuiElementStringValue("ImGui list","Element1,Element2,Element3,Element4,Element5,Element6")
SetImGuiElementEvent("ImGui list",function(elem,id,old,oldid) Print(elem.." "..id.." OLD:"..old.." "..oldid) end)

CreateImGuiElement("ImGui Text sl","ImGui Window","Text")
SetImGuiElementText("ImGui Text sl","sl text")

CreateImGuiElement("ImGui Text sl 2","ImGui Window","Text")
SetImGuiElementText("ImGui Text sl 2","TEXT 2")
SetImGuiElementConnect("ImGui Text sl 2",true)

CreateImGuiElement("ImGui drag float","ImGui Window","Drag Number")
SetImGuiElementText("ImGui drag float","Drag Number")
SetImGuiElementEvent("ImGui drag float",function(f) Print("new float -> "..f) end)

CreateImGuiElement("ImGui drag float 2","ImGui Window","Drag Number 2")
SetImGuiElementText("ImGui drag float 2","Drag Number (2)")
SetImGuiElementEvent("ImGui drag float 2",function(f,f2) Print("new float -> "..f.." & "..f2) end)

CreateImGuiElement("ImGui drag float 3","ImGui Window","Drag Number 3")
SetImGuiElementText("ImGui drag float 3","Drag Number (3)")
SetImGuiElementEvent("ImGui drag float 3",function(f,f2,f3) Print("new float -> "..f.." & "..f2.." & "..f3) end)

CreateImGuiElement("ImGui drag float 4","ImGui Window","Drag Number 4")
SetImGuiElementText("ImGui drag float 4","Drag Number (4)")
SetImGuiElementEvent("ImGui drag float 4",function(f,f2,f3,f4) Print("new float -> "..f.." & "..f2.." & "..f3.." & "..f4) end)

CreateImGuiElement("ImGui slider float","ImGui Window","Slider Number")
SetImGuiElementText("ImGui slider float","Slider Number")
SetImGuiElementEvent("ImGui slider float",function(f) Print("new float -> "..f) end)

CreateImGuiElement("ImGui Slider float 2","ImGui Window","Slider Number 2")
SetImGuiElementText("ImGui Slider float 2","Slider Number (2)")
SetImGuiElementEvent("ImGui Slider float 2",function(f,f2) Print("new float -> "..f.." & "..f2) end)

CreateImGuiElement("ImGui Slider float 3","ImGui Window","Slider Number 3")
SetImGuiElementText("ImGui Slider float 3","Slider Number (3)")
SetImGuiElementEvent("ImGui Slider float 3",function(f,f2,f3) Print("new float -> "..f.." & "..f2.." & "..f3) end)

CreateImGuiElement("ImGui Slider float 4","ImGui Window","Slider Number 4")
SetImGuiElementText("ImGui Slider float 4","Slider Number (4)")
SetImGuiElementEvent("ImGui Slider float 4",function(f,f2,f3,f4) Print("new float -> "..f.." & "..f2.." & "..f3.." & "..f4) end)

CreateImGuiElement("ImGui input text","ImGui Window","Input Text")
SetImGuiElementText("ImGui input text","Input Text")
SetImGuiElementEvent("ImGui input text",function(text) Print(text) end)

CreateImGuiElement("ImGui input text m","ImGui Window","Input Text Multiline")
SetImGuiElementText("ImGui input text m","Input Text Multiline")
SetImGuiElementEvent("ImGui input text m",function(text) Print(text) end)

CreateImGuiElement("ImGui edit rgb","ImGui Window","Edit RGB")
SetImGuiElementText("ImGui edit rgb","Edit RGB")
SetImGuiElementEvent("ImGui edit rgb",function(color) Print(color) end)

CreateImGuiElement("ImGui edit rgba","ImGui Window","Edit RGBA")
SetImGuiElementText("ImGui edit rgba","Edit RGBA")
SetImGuiElementEvent("ImGui edit rgba",function(color) Print(color) end)

CreateImGuiElement("ImGui edit rgb e","ImGui Window","Edit RGB Extended")
SetImGuiElementText("ImGui edit rgb e","Edit RGB Extended")
SetImGuiElementEvent("ImGui edit rgb e",function(color) Print(color) end)

CreateImGuiElement("ImGui edit rgba e","ImGui Window","Edit RGBA Extended")
SetImGuiElementText("ImGui edit rgba e","Edit RGBA Extended")
SetImGuiElementEvent("ImGui edit rgba e",function(color) Print(color) end)

CreateImGuiElement("ImGui tooltip", "ImGui Window", "Text")
SetImGuiElementText("ImGui tooltip","Tooltip Text")
SetImGuiElementTooltip("ImGui tooltip","Hello! That tooltip!")