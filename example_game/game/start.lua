CreateWindow("Window")
SetWindowMain("Window")
SetWindowImGui("Window")
CreateScene("Scene")
SetWindowScene("Window","Scene")

--https://woowz11.github.io/woowzsite/woowzengine.html#fonts
for i=16,EngineChars do
	Print(i.." - "..GetEngineChar(i))
end