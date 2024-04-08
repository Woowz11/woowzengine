local ui = require("ui")
local base = require("wbase")
local log = require("logger")

local wwindow = {}

--Все окна
wwindow.Windows = {}

--Главное окно
wwindow.MainWindow = nil

--Делает окно главным
wwindow.SetMainWindow = function(win)
    wwindow.MainWindow = win
    function win:onClose()
        log.EndLog()
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