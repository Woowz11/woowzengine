local d = require("data")

local r = {}

r.CameraMovement = function()
	local PK = PressedKeys()
    
    local CameraSpeed = 0.05
    
    if(PK["shift"])then
        CameraSpeed = 0.2
    end
    
    if(PK["ctrl"])then
        CameraSpeed = 0.005
    end
    
    local X = 0
    local Y = 0
    
    if(PK["w"])then
        Y = Y + CameraSpeed
    end
    if(PK["s"])then
        Y = Y - CameraSpeed
    end
    
    if(PK["d"])then
        X = X + CameraSpeed
    end
    if(PK["a"])then
        X = X - CameraSpeed
    end
    
    if (X~=0 or Y~=0) then
        local CameraPosition = GetCameraPosition("Scene")
        SetCameraPosition("Scene",Vector2.new(CameraPosition.x + (X/GetCameraZoom("Scene")),CameraPosition.y + (Y/GetCameraZoom("Scene"))))
    end
    
    local ZoomSpeed = 0.05
    local Zoom = 1
    
    if(PK["plus"])then
        Zoom = Zoom + ZoomSpeed
    end
    if(PK["minus"])then
        Zoom = Zoom - ZoomSpeed
    end
    
    if(Zoom~=1)then
        SetCameraZoom("Scene",GetCameraZoom("Scene") * Zoom)
    end
end

r.Render = function()CycleRender(function() 

	Pairs(d.SceneElements,function(v,k)
		local vis = ""
		if k.visible == false then
			vis = " (Невидимый)"
		end
		SetImGuiElementText("ELEMENT: "..k.cout,k.type.." | "..k.id..vis)
		if v == d.SelectedElement then
			SetImGuiElementText("SelectedE","Выбран: "..k.type.." | "..k.id..vis)
		end
		if k.type == "Спрайт" then
			SetSpriteVisible("Scene",k.cout,k.visible)
			SetSpriteMirrorX("Scene",k.cout,k.flipx)
			SetSpriteMirrorY("Scene",k.cout,k.flipy)
			SetSpriteHeight("Scene",k.cout,k.height)
			SetSpritePosition("Scene",k.cout,k.pos)
			SetSpriteCenter("Scene",k.cout,k.center)
			SetSpriteSize("Scene",k.cout,k.size)
			SetSpriteColor("Scene",k.cout,k.color)
			SetSpriteRotation("Scene",k.cout,ToNumber(k.rot,0))
			SetSpriteLayer("Scene",k.cout,k.zindex)
		end
		
		if k.type == "Текст" then
		
		end
	end)
	
	r.CameraMovement()

end)end

r.StopRender = function()CycleRender(function()end)end

return r