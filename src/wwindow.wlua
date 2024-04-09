local ui = require("ui")
local base = require("wbase")
local console = require("wconsole")

local wwindow = {}

--Окна
wwindow.Windows = {}

--Главное окно
wwindow.MainWindow = nil

--Делает окно главным
wwindow.SetMainWindow = function(win)
    if wwindow.MainWindow ~= nil then
        console.Error("Main window alredy exists! ["..win.title.."]")
    else
        wwindow.MainWindow = win
        function win:onClose()
            console.PrintLog("WINDOW","Main window closed...")
            sys.exit()
        end
    end
end

-- Создаёт новое окно
wwindow.CreateWindow = function(title, width, height)
    if width == nil then width = 500 end
    if height == nil then height = 500 end
    if title == nil then title = base.Settings.Game.Name end
    local win = ui.Window(title,width,height)
    console.PrintLog("WINDOW","Window ["..title.."] created!")
    if wwindow.MainWindow == nil then wwindow.SetMainWindow(win)  end
    table.insert(wwindow.Windows,win)
end

return wwindow