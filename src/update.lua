local window = require("wwindow")
local console = require("wconsole")
local ui = require("ui")

local update = {}

local i = 0

update.RenderWindows = function()
    for _, win in pairs(window.Windows) do
        --ui.run(win)
        win:show()
    end
end

update.Update = function()
    --update.RenderWindows()
    console.Print(i)
    i = i + 1
end

return update