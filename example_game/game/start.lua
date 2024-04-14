local id = 0
CreateWindow("main","MAIN")
SetWindowMain("main")

Cycle(function() 

local keys = PressedKeys()

for i = 1,#keys do
	Print("Now pressed "..keys[i])
end

end,1)