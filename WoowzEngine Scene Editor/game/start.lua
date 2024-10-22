local f = require("functions")

f.Print("Welcome to WoowzEngine Scene Editor ("..Version..")!")

local CreateWindows = require("createwindows")

CreateWindows.CreateWindows()
CreateWindows.ApplyImGui()
CreateWindows.CreateScenes()

local Interface = require("interface")

Interface.CreateInterface()