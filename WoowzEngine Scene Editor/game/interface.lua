local f = require("functions")
local d = require("data")
local r = require("render")

local intf = {}

intf.CreateEditorsWindow = function()
	
	CreateImGuiWindow("Editor-Sprite","Редактор (Спрайт)",false)
	CreateImGuiElement("EDIT-S,id","Editor-Sprite","Input Text")
	SetImGuiElementText("EDIT-S,id","ID")
	SetImGuiElementTooltip("EDIT-S,id","ID объекта, не должно совпадать с другими!")
	SetImGuiElementEvent("EDIT-S,id",function(t)
		d.SceneElements[d.SelectedElement].id = t
	end)
	
	CreateImGuiElement("EDIT-S,pos","Editor-Sprite","input text 2")
	SetImGuiElementText("EDIT-S,pos","Позиция")
	SetImGuiElementTooltip("EDIT-S,pos","Позиция объекта")
	SetImGuiElementEvent("EDIT-S,pos",function(x,y)
		d.SceneElements[d.SelectedElement].pos = Vector2.new(ToNumber(x,0),ToNumber(y,0))
	end)
	
	CreateImGuiElement("EDIT-S,size","Editor-Sprite","input text 2")
	SetImGuiElementText("EDIT-S,size","Размер")
	SetImGuiElementTooltip("EDIT-S,size","Размер объекта")
	SetImGuiElementEvent("EDIT-S,size",function(x,y)
		d.SceneElements[d.SelectedElement].size = Vector2.new(Max(ToNumber(x,0),0.001),Max(ToNumber(y,0),0.001))
	end)
	
	CreateImGuiElement("EDIT-S,sizeauto","Editor-Sprite","button")
	SetImGuiElementText("EDIT-S,sizeauto","Сделать размер под текстуру")
	SetImGuiElementTooltip("EDIT-S,sizeauto","Делает автоматический размер под текстуру")
	SetImGuiElementEvent("EDIT-S,sizeauto",function()
		SetSpriteSizeByTexture("Scene",d.SceneElements[d.SelectedElement].cout)
		d.SceneElements[d.SelectedElement].size = GetSpriteSize("Scene",d.SceneElements[d.SelectedElement].cout)
		SetImGuiElementValue("EDIT-S,size",d.SceneElements[d.SelectedElement].size.x,d.SceneElements[d.SelectedElement].size.y)
	end)
	
	CreateImGuiElement("EDIT-S,color","Editor-Sprite","Edit RGBA")
	SetImGuiElementText("EDIT-S,color","Цвет")
	SetImGuiElementTooltip("EDIT-S,color","Цвет спрайта, а так же прозрачность")
	SetImGuiElementEvent("EDIT-S,color",function(c)
		d.SceneElements[d.SelectedElement].color = c
	end)
	
	CreateImGuiElement("EDIT-S,rot","Editor-Sprite","input text 2")
	SetImGuiElementText("EDIT-S,rot","Поворот")
	SetImGuiElementTooltip("EDIT-S,rot","Поворот объекта")
	SetImGuiElementEvent("EDIT-S,rot",function(deg,rad,first)
		if first then
			d.SceneElements[d.SelectedElement].rot = deg
			SetImGuiElementValue("EDIT-S,rot",deg,RemoveMagicalNumber(ToRad(ToNumber(deg,0))))
		else
			d.SceneElements[d.SelectedElement].rot = RemoveMagicalNumber(ToDeg(ToNumber(rad,0)))
			SetImGuiElementValue("EDIT-S,rot",RemoveMagicalNumber(ToDeg(ToNumber(rad,0))),rad)
		end
	end)
	
	CreateImGuiElement("EDIT-S,zindex","Editor-Sprite","input text")
	SetImGuiElementText("EDIT-S,zindex","Слой")
	SetImGuiElementTooltip("EDIT-S,zindex","Очередь отображения")
	SetImGuiElementEvent("EDIT-S,zindex",function(r)
		d.SceneElements[d.SelectedElement].zindex = ToNumber(r,0)
	end)
	
	CreateImGuiElement("EDIT-S,visible","Editor-Sprite","checkbox")
	SetImGuiElementText("EDIT-S,visible","Видимый")
	SetImGuiElementTooltip("EDIT-S,visible","Будет ли рендерится на сцене")
	SetImGuiElementEvent("EDIT-S,visible",function(r)
		d.SceneElements[d.SelectedElement].visible = r
	end)
	
	CreateImGuiElement("EDIT-S,flipx","Editor-Sprite","checkbox")
	SetImGuiElementText("EDIT-S,flipx","Отразить по X")
	SetImGuiElementTooltip("EDIT-S,flipx","Горизонтально отзеркалить текстуру")
	SetImGuiElementEvent("EDIT-S,flipx",function(r)
		d.SceneElements[d.SelectedElement].flipx = r
	end)
	
	CreateImGuiElement("EDIT-S,flipy","Editor-Sprite","checkbox")
	SetImGuiElementText("EDIT-S,flipy","Отразить по Y")
	SetImGuiElementTooltip("EDIT-S,flipy","Вертикально отзеркалить текстуру")
	SetImGuiElementEvent("EDIT-S,flipy",function(r)
		d.SceneElements[d.SelectedElement].flipy = r
	end)
	
	CreateImGuiElement("EDIT-S,height","Editor-Sprite","input text")
	SetImGuiElementText("EDIT-S,height","Высота")
	SetImGuiElementTooltip("EDIT-S,height","Эффект позиции элемента по Z координате")
	SetImGuiElementEvent("EDIT-S,height",function(r)
		d.SceneElements[d.SelectedElement].height = ToNumber(r,0)
	end)
	
	CreateImGuiElement("EDIT-S,center","Editor-Sprite","input text 2")
	SetImGuiElementText("EDIT-S,center","Центр")
	SetImGuiElementTooltip("EDIT-S,center","Отвечает за цент спрайта")
	SetImGuiElementEvent("EDIT-S,center",function(x,y)
		d.SceneElements[d.SelectedElement].center = Vector2.new(Max(ToNumber(x,0),0.001),Max(ToNumber(y,0),0.001))
	end)
	
	CreateImGuiWindow("Editor-Text","Редактор (Текст)",false)
	
end

local oldeditors = ""
intf.SelectElement = function(editor,cout)
	if oldeditors ~= "" then
		SetImGuiWindowVisible(oldeditors,false)
	end
	SetImGuiWindowVisible(editor,true)
	
	local el = d.SceneElements[cout]
	if editor == "Editor-Sprite" then
		SetImGuiElementValue("EDIT-S,id",el.id)
		SetImGuiElementValue("EDIT-S,pos",el.pos.x,el.pos.y)
		SetImGuiElementValue("EDIT-S,size",el.size.x,el.size.y)
		SetImGuiElementValue("EDIT-S,center",el.center.x,el.center.y)
		SetImGuiElementValue("EDIT-S,color",el.color)
		SetImGuiElementValue("EDIT-S,rot",el.rot,ToRad(el.rot))
		SetImGuiElementValue("EDIT-S,zindex",el.zindex)
		SetImGuiElementValue("EDIT-S,visible",el.visible)
		SetImGuiElementValue("EDIT-S,flipx",el.flipx)
		SetImGuiElementValue("EDIT-S,flipy",el.flipy)
		SetImGuiElementValue("EDIT-S,height",el.height)
	end
	
	oldeditors = editor
	d.SelectedElement = cout
end

intf.CreateSceneExplorerWindow = function()

	CreateImGuiWindow("Explorer","Сцена",false)
	CreateImGuiElement("CreateObjectListE","Explorer","List")
	local selectedobject = "Спрайт"
	SetImGuiElementStringValue("CreateObjectListE","Спрайт,Текст")
	SetImGuiElementText("CreateObjectListE","Тип объекта")
	SetImGuiElementEvent("CreateObjectListE",function(n) selectedobject = n SetImGuiElementText("CreateObjectE","Создать ["..selectedobject.."]") end)
	CreateImGuiElement("CreateObjectE","Explorer","Button")
	SetImGuiElementText("CreateObjectE","Создать ["..selectedobject.."]")
	CreateImGuiElement("SelectedE","Explorer","Text")
	SetImGuiElementText("SelectedE","Никакой объект не выбран")
	SetImGuiElementEvent("CreateObjectE",function()
	
		local result = {}
		result.cout = d.TotalObjs
		local id = "ELEMENT: "..result.cout
		
		result.id = "New Object ("..d.TotalObjs..")"
		result.type = selectedobject
		result.pos = Vector2Zero
		if selectedobject=="Спрайт" then
			result.sprite = "default"
			result.rot = 0
			result.size = Vector2One
			result.shader = "default"
			result.color = White
			result.zindex = result.cout
			result.flipx = false
			result.flipy = false
			result.visible = true
			result.height = 0
			result.center = Vector2Zero
			result.corners = {Vector2Zero,Vector2Zero,Vector2Zero,Vector2Zero}
			result.uv = {Right,Vector2Zero,Vector2One,Left}
			
			CreateSprite(result.cout,"Scene")
			
			CreateImGuiElement(id,"Explorer","Button")
			SetImGuiElementText(id,result.type.." | "..result.id)
			SetImGuiElementEvent(id,function() intf.SelectElement("Editor-Sprite",result.cout) end)
		end
		if selectedobject=="Текст" then
			result.font = "default"
			result.height = 0
			result.mono = false
			result.text = "New Text!"
			result.visible = true
			result.shader = "default"
			result.color = White
			result.size = 1
			
			CreateText(result.cout,"Scene")
			SetTextText("Scene",result.cout,result.text)
			
			CreateImGuiElement(id,"Explorer","Button")
			SetImGuiElementText(id,result.type.." | "..result.id)
			SetImGuiElementEvent(id,function() intf.SelectElement("Editor-Text",result.cout) end)
		end
		d.SceneElements[result.cout] = result
		f.Print("Object ["..selectedobject.."] created on scene! Cout:"..d.TotalObjs)
		d.TotalObjs = d.TotalObjs + 1
	
	end)
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
			f.Print("Created new scene ('"..d.SceneName.."') by ('"..d.SceneAuthor.."')")
			SetImGuiElementText("SelectedScene","Сцена: "..d.SceneName)
			SetImGuiElementText("SelectedSceneAuthor","Автор: "..d.SceneAuthor)
		
			SetImGuiWindowVisible("CreateScene",false)
			SetImGuiWindowVisible("Explorer",true)
			
			r.Render()
		
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
	intf.CreateEditorsWindow()
	
	CreateImGuiElement("CreateB","M","Button")
	SetImGuiElementText("CreateB","Создать сцену")
	SetImGuiElementTooltip("CreateB","Создать новый файл сцены")
	intf.CreateCreateSceneWindow()
	SetImGuiElementEvent("CreateB",function() if not d.LockButtons then
	
		d.LockButtons = true
		SetImGuiElementText("CreateSceneErrorCS","")
		SetImGuiElementValue("NameCS",d.DefaultSceneName)
		d.SceneName = d.DefaultSceneName
		SetImGuiElementValue("AuthorCS",d.DefaultSceneAuthor)
		d.SceneAuthor = d.DefaultSceneAuthor
		SetImGuiElementValue("DescCS",d.DefaultSceneDesc)
		d.SceneDesc = d.DefaultSceneDesc
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
	
	CreateImGuiElement("CompileB","M","Button")
	SetImGuiElementText("CompileB","Компилировать")
	SetImGuiElementTooltip("CompileB","Компилировать текущую сцену")
	SetImGuiElementEvent("CompileB",function() if not d.LockButtons then
		
		if d.HasScene then
			local compiledscene = ""
			
			compiledscene = compiledscene .. "--[[\nСцена ("..d.SceneName..") создана ("..d.SceneAuthor..") с помощью WoowzEngine Scene Editor ("..Version..")\nОписание: "..d.SceneDesc.."\n]]\n\n"
			local function T(str)
				return "\""..Replace(Replace(str,'\\','\\\\'),'"','\\"').."\""
			end
			local s = T(d.SceneName)
			compiledscene = compiledscene .. "CreateScene("..s..")\nSetWindowScene(--[[Укажите окно]],"..s..")\nSetSceneBackgroundColor("..s..",Black)\n\n"
			
			compiledscene = compiledscene .. "--Компоненты на сцене\n"
			Pairs(d.SceneElements,function(v,el)
				local id = T(el.id)
				compiledscene = compiledscene .. "--Компонент ("..el.type.." | "..el.id..")\n"
			
				if el.type == "Спрайт" then
					compiledscene = compiledscene .. "CreateSprite("..id..","..s..")\n"
					compiledscene = compiledscene .. "SetSpriteLayer("..s..","..id..","..RemoveMagicalNumber(el.zindex)..")\n"
					
					if el.visible == false then
						compiledscene = compiledscene .. "SetSpriteVisible("..s..","..id..",false)\n"
					end
					
					if el.flipx == true then
						compiledscene = compiledscene .. "SetSpriteMirrorX("..s..","..id..",true)\n"
					end
					
					if el.flipy == true then
						compiledscene = compiledscene .. "SetSpriteMirrorY("..s..","..id..",true)\n"
					end
					
					if el.pos ~= Vector2Zero then
						compiledscene = compiledscene .. "SetSpritePosition("..s..","..id..",Vector2.new("..RemoveMagicalNumber(el.pos.x)..","..RemoveMagicalNumber(el.pos.y).."))\n"
					end
					
					if el.size ~= Vector2One then
						compiledscene = compiledscene .. "SetSpriteSize("..s..","..id..",Vector2.new("..RemoveMagicalNumber(el.size.x)..","..RemoveMagicalNumber(el.size.y).."))\n"
					end
					
					if el.center ~= Vector2Zero then
						compiledscene = compiledscene .. "SetSpriteCenter("..s..","..id..",Vector2.new("..RemoveMagicalNumber(el.center.x)..","..RemoveMagicalNumber(el.center.y).."))\n"
					end
					
					if ToNumber(el.rot,0) ~= 0 then
						compiledscene = compiledscene .. "SetSpriteRotation("..s..","..id..","..RemoveMagicalNumber(el.rot)..")\n"
					end
					
					if el.height ~= 0 then
						compiledscene = compiledscene .. "SetSpriteHeight("..s..","..id..","..RemoveMagicalNumber(el.height)..")\n"
					end
					
					if el.color ~= White then
						local colorresult = ""
						
						local r = el.color:GetR()
						local g = el.color:GetG()
						local b = el.color:GetB()
						local a = el.color:GetA()
						
						if a == 255 then
							colorresult = "Color.new("..ToString(el.color:GetR())..","..ToString(el.color:GetG())..","..ToString(el.color:GetB())..")"
						else
							colorresult = "Color.new("..ToString(el.color:GetR())..","..ToString(el.color:GetG())..","..ToString(el.color:GetB())..","..ToString(el.color:GetA())..")"
						end
						
						if r == 255 and g == 0 and b == 0 and a == 255 then
							colorresult = "Red"
						end
						if r == 255 and g == 128 and b == 0 and a == 255 then
							colorresult = "Orange"
						end
						if r == 255 and g == 255 and b == 0 and a == 255 then
							colorresult = "Yellow"
						end
						if r == 0 and g == 255 and b == 0 and a == 255 then
							colorresult = "Green"
						end
						if r == 0 and g == 255 and b == 255 and a == 255 then
							colorresult = "Cyan"
						end
						if r == 0 and g == 0 and b == 255 and a == 255 then
							colorresult = "Blue"
						end
						if r == 255 and g == 0 and b == 255 and a == 255 then
							colorresult = "Purple"
						end
						if r == 128 and g == 128 and b == 128 and a == 255 then
							colorresult = "Gray"
						end
						if r == 0 and g == 0 and b == 0 and a == 255 then
							colorresult = "Black"
						end
						if r == 0 and g == 0 and b == 0 and a == 0 then
							colorresult = "Transparent"
						end
						if r == 255 and g == 0 and b == 255 and a == 254 then
							colorresult = "ErrorColor"
						end
						
						compiledscene = compiledscene .. "SetSpriteColor("..s..","..id..","..colorresult..")\n"
					end
					
					compiledscene = compiledscene .. "\n"
				end
			
			end)
			
			WriteFile(SourcePath.."/result.txt",compiledscene)
		else
			f.Warn("Can't compile! Scene not found...");
		end
		
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
		
end

return intf