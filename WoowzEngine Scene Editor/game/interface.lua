local f = require("functions")
local d = require("data")

local intf = {}

intf.CreateCreateSceneWindow = function()
	d.LockButtons = true

	CreateImGuiWindow("CreateScene","Создание сцены")
	CreateImGuiElement("NameCS","CreateScene","Input Text")
	SetImGuiElementText("NameCS","Название")
	CreateImGuiElement("AuthorCS","CreateScene","Input Text")
	SetImGuiElementText("AuthorCS","Автор")
	CreateImGuiElement("DescCS","CreateScene","Input Text Multiline")
	SetImGuiElementText("DescCS","Описание")
	
	CreateImGuiElement("CreateSceneCS","CreateScene","Button")
	SetImGuiElementText("CreateSceneCS","Создать...")
	CreateImGuiElement("BackCS","CreateScene","Button")
	SetImGuiElementText("BackCS","Вернуться...")
	
	d.HasScene = true
end

intf.CreateMainWindow = function()
CreateImGuiElement("Name","M","Text")
	SetImGuiElementText("Name","WoowzEngine Scene Editor")
	CreateImGuiElement("Version","M","Text")
	SetImGuiElementText("Version","Версия: "..Version)
	
	CreateImGuiElement("CreateB","M","Button")
	SetImGuiElementText("CreateB","Создать сцену")
	SetImGuiElementEvent("CreateB",function() if not d.LockButtons then
	
		if not d.HasScene then intf.CreateCreateSceneWindow() end
		
	end end)
	
	CreateImGuiElement("SaveB","M","Button")
	SetImGuiElementText("SaveB","Сохранить сцену")
	SetImGuiElementEvent("SaveB",function() if not d.LockButtons then end end)
	
	CreateImGuiElement("SaveAsB","M","Button")
	SetImGuiElementText("SaveAsB","Сохранить как... сцену")
	SetImGuiElementEvent("SaveAsB",function() if not d.LockButtons then end end)
	
	CreateImGuiElement("OpenB","M","Button")
	SetImGuiElementText("OpenB","Открыть сцену")
	SetImGuiElementEvent("OpenB",function() if not d.LockButtons then end end)
	
	CreateImGuiElement("QuitB","M","Button")
	SetImGuiElementText("QuitB","Выйти")
	SetImGuiElementEvent("QuitB",function() if not d.LockButtons then Exit() end end)
end

intf.CreateInterface = function()
	
	CreateImGuiWindow("M","Главное")
	intf.CreateMainWindow()
	
	CreateImGuiWindow("ED","Редактировать")
	
	CreateImGuiWindow("S","Выбрать")
	
	CreateImGuiWindow("E","Сцена")
	
end

return intf