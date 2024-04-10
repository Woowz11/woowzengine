--local window = require("wwindow")
local console = require("wconsole")
--local console = require("console")

--local test = require("otherlibs")

local update = {}

local i = 0

update.RenderWindows = function()
    -- for _, win in pairs(window.Windows) do
        --ui.run(win)
    --    win:show()
    --end
end

update.Update = function()
    --ui.msg("msg %username% test")
    --test.Test()
    console.Error("test")
    --os.execute("msg %username% "..sys.Datetime())
    --c.writecolor("green","test".."\n")
end

return update