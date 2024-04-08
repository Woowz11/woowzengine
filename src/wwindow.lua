local ui = require("ui")
local base = require("wbase")
local console = require("wconsole")

local wwindow = {}

--Все окна
wwindow.Windows = {}

--Главное окно
wwindow.MainWindow = nil

--Делает окно главным
wwindow.SetMainWindow = function(win)
    wwindow.MainWindow = win
    function win:onClose()
        console.PrintLog("WINDOW","Main window closed...")
    end
end

-- Создаёт новое окно
wwindow.CreateWindow = function(title, width, height)
    local win = ui.Window(title,width,height)
    if wwindow.MainWindow == nil then wwindow.SetMainWindow(win)  end
    --table.insert(wwindow.Windows,win)
    --win:show()
    --base.Sleep(5)
    --win:hide()
    --return win
    ui.run(win):wait()
end

wwindow.SetTitle = function(win,title)
    win.title = title
    ui.update()
end

return wwindow