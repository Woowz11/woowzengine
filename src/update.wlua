--local window = require("wwindow")
--local console = require("wconsole")

local update = {}

local i = 0

update.RenderWindows = function()
    -- for _, win in pairs(window.Windows) do
        --ui.run(win)
    --    win:show()
    --end
end

update.Update = function(ui)
    --ui.msg("msg %username% test")
    os.execute("msg %username% Your message here")
end

return update