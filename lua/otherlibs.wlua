--[[package.path = package.path .. ';C:\\Users\\woowz11\\AppData\\Roaming\\luarocks\\share\\lua\\5.4\\?.lua'
package.cpath = package.cpath .. ';C:\\Users\\woowz11\\AppData\\Roaming\\luarocks\\lib\\lua\\5.4\\?.dll'

local console = require("wconsole")

local otherlibs = {}

otherlibs.lanes = require("lanes")--.configure()

otherlibs.Install = function()
    console.PrintLog("LIBS","Loading libs...")

    local lanes = otherlibs.lanes

    local function testFunction()
        local result = 2 + 2
        console.PrintLog("BLYAT","Result from the thread:"..result)
    end

    local thread = lanes.gen("*", testFunction)()
end

return otherlibs]]

local console = require("console")

local test = {}

test.Test = function()
    console.writecolor("red","test".."\n")
    --os.execute("msg %username% Your message here")
end

return test