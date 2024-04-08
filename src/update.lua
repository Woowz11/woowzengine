local window = require("wwindow")
local ui = require("ui")

local update = {}

update.UpdateWindows = function()
    for win in pairs(window.Windows) do
        win:show()
    end
end

update.Update = function()
    update.UpdateWindows()
end

return update