CreateWindow("window","(◕‿◕)")
SetWindowMain("window")
CreateScene("scene")
SetSceneBackgroundColor("scene",Transparent)
SetWindowScene("window","scene")
 
function CameraMovement()
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
        local CameraPosition = GetCameraPosition("scene")
        SetCameraPosition("scene",Vector2.new(CameraPosition.x + (X/GetCameraZoom("scene")),CameraPosition.y + (Y/GetCameraZoom("scene"))))
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
        SetCameraZoom("scene",GetCameraZoom("scene") * Zoom)
    end
end
SetCameraZoom("scene",1)

CycleRender(function() 
    CameraMovement() 
end)

CreateText("text","scene","█░░████\n█░░█░░░\n█░░█░░░\n███████\n░░░█░░█\n░░░█░░█\n████░░█\n")
SetTextColor("scene","text",Color.new(128,0,0,255))