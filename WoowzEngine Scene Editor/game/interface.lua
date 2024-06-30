local f = require("functions")
local d = require("data")

local intf = {}

intf.CreateSceneExplorerWindow = function()

	CreateImGuiWindow("Explorer","Сцена",false)

end

intf.CreateCreateSceneWindow = function()

	CreateImGuiWindow("CreateScene","Создание сцены",false)
	
	CreateImGuiElement("NameCS","CreateScene","Input Text")
	SetImGuiElementText("NameCS","Название")
	SetImGuiElementEvent("NameCS",function(t)
		d.SceneName = t
	end)
	CreateImGuiElement("AuthorCS","CreateScene","Input Text")
	SetImGuiElementText("AuthorCS","Автор")
	SetImGuiElementEvent("AuthorCS",function(t)
		d.SceneAuthor = t
	end)
	CreateImGuiElement("DescCS","CreateScene","Input Text Multiline")
	SetImGuiElementText("DescCS","Описание")
	SetImGuiElementEvent("DescCS",function(t)
		d.SceneDesc = t
	end)
	
	CreateImGuiElement("CreateSceneErrorCS","CreateScene","Text")
	SetImGuiElementColor("CreateSceneErrorCS",Red)
	SetImGuiElementText("CreateSceneErrorCS","")
	
	CreateImGuiElement("CreateSceneCS","CreateScene","Button")
	SetImGuiElementText("CreateSceneCS","Создать...")
	SetImGuiElementEvent("CreateSceneCS",function()
	
		if d.SceneName ~= "" and d.SceneAuthor ~= "" then
		
			d.LockButtons = false
			d.HasScene = true
			PrintCustom("WESCENE","Created new scene ('"..d.SceneName.."') by ('"..d.SceneAuthor.."')")
			SetImGuiElementText("SelectedScene","Сцена: "..d.SceneName)
			SetImGuiElementText("SelectedSceneAuthor","Автор: "..d.SceneAuthor)
		
			SetImGuiWindowVisible("CreateScene",false)
		
		else
			if d.SceneName == "" then
				SetImGuiElementText("CreateSceneErrorCS","Нужно указать название сцены!")
			elseif d.SceneAuthor == "" then
				SetImGuiElementText("CreateSceneErrorCS","Нужно указать автора сцены!")
			end
		end
	
	end)
	CreateImGuiElement("BackCS","CreateScene","Button")
	SetImGuiElementText("BackCS","Вернуться...")
	SetImGuiElementEvent("BackCS",function()
	
		d.LockButtons = false
		SetImGuiWindowVisible("CreateScene",false)
	
	end)
	
end

intf.CreateMainWindow = function()
CreateImGuiElement("Name","M","Text")
	SetImGuiElementText("Name","WoowzEngine Scene Editor")
	CreateImGuiElement("Version","M","Text")
	SetImGuiElementText("Version","Версия: "..Version)
	CreateImGuiElement("SelectedScene","M","Text")
	SetImGuiElementText("SelectedScene","Сцена не выбрана...")
	CreateImGuiElement("SelectedSceneAuthor","M","Text")
	SetImGuiElementText("SelectedSceneAuthor","")
	
	intf.CreateSceneExplorerWindow()
	
	CreateImGuiElement("CreateB","M","Button")
	SetImGuiElementText("CreateB","Создать сцену")
	SetImGuiElementTooltip("CreateB","Создать новый файл сцены")
	intf.CreateCreateSceneWindow()
	SetImGuiElementEvent("CreateB",function() if not d.LockButtons then
	
		d.LockButtons = true
		SetImGuiElementText("CreateSceneErrorCS","")
		SetImGuiWindowVisible("CreateScene",true)
		
	end end)
	
	CreateImGuiElement("SaveB","M","Button")
	SetImGuiElementText("SaveB","Сохранить сцену")
	SetImGuiElementTooltip("SaveB","Сохранить текущую сцену")
	SetImGuiElementEvent("SaveB",function() if not d.LockButtons then
		PlayBeep()
	end end)
	
	CreateImGuiElement("SaveAsB","M","Button")
	SetImGuiElementText("SaveAsB","Сохранить как... сцену")
	SetImGuiElementTooltip("SaveAsB","Сохранить текущую сцену на новое место")
	SetImGuiElementEvent("SaveAsB",function() if not d.LockButtons then
		PlayBeep()
	end end)
	
	CreateImGuiElement("OpenB","M","Button")
	SetImGuiElementText("OpenB","Открыть сцену")
	SetImGuiElementTooltip("OpenB","Открыть для изменения файл сцены")
	SetImGuiElementEvent("OpenB",function() if not d.LockButtons then
		PlayBeep()
	end end)
	
	CreateImGuiElement("SettingsB","M","Button")
	SetImGuiElementText("SettingsB","Настройки")
	SetImGuiElementTooltip("SettingsB","Настройки программы")
	SetImGuiElementEvent("SettingsB",function() if not d.LockButtons then
		PlayBeep()
	end end)
	
	CreateImGuiElement("QuitB","M","Button")
	SetImGuiElementText("QuitB","Выйти")
	SetImGuiElementTooltip("QuitB","Выйти из программы")
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