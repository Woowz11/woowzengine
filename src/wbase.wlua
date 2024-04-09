local json = require("json")

local wbase = {}

wbase.Time = 0
wbase.StartDate = sys.Datetime()

wbase.PathToGame = ""

wbase.Settings = {
    Console = false,
    Game = {
        Name = "New Game",
        Version = "0.0.0",
        Author = "Unknown"
    },
    Version = "0",

    Install = function()
        if wbase.Programms.RunnedCount("woowzengine")>1 then sys.exit() end
        wbase.PathToGame = wbase.String.Replace(sys.File("woowzengine.exe").fullpath, "woowzengine.exe","")

        local info = wbase.Json.Decode(wbase.File.Read("info.json"))
        wbase.Settings.Version = info.Version

        local settings = wbase.Json.Decode(wbase.File.Read("game/settings.json"))
        wbase.Settings.Console = settings.Console

        local game = wbase.Json.Decode(wbase.File.Read("game/gameinfo.json"))
        wbase.Settings.Game.Name = game.GameName
        wbase.Settings.Game.Version = game.GameVersion
        wbase.Settings.Game.Author = game.Author
    end
}

wbase.GetDate = function(what_get)
    local date = sys.Datetime()
    local results = {
        ms = wbase.String.Fill(date.milliseconds,3,"0",true),
        s  = wbase.String.Fill(date.second,      2,"0",true),
        m  = wbase.String.Fill(date.minute,      2,"0",true),
        h  = wbase.String.Fill(date.hour,        2,"0",true),
        d  = wbase.String.Fill(date.day,         2,"0",true),
        mn = wbase.String.Fill(date.month,       2,"0",true),
        y  = wbase.String.Fill(date.year,        4,"0",true),
        w  = date.weekday
    }
    if results[what_get] ~= nil then return results[what_get] end
    return "ERROR_GET_DATE_("..tostring(what_get)..")"
end

wbase.String = {
    --Получить длину строки
    Length = function(str)
        return string.len(str)
    end,
    --Заполняет пустоту определёнными символами
    Fill = function(str,dist,symbol,invert)
        if invert == nil then invert = false end
        if dist == nil then dist = 10 end
        if symbol == nil then symbol = "#" end
        local len = wbase.String.Length(str)
        if dist <= len then
            return str
        else
            for i = len+1, dist do
                if invert then
                    str = symbol..str
                else
                    str = str..symbol
                end
            end
        end
        return str
    end,
    --Уберает пробелы с начала и с конца
    Trim = function(str)
        return str:match"^%s*(.*)":match"(.-)%s*$"
    end,
    --Проверяет пустая строка или нет
    IsEmpty = function(str)
        local result = false
        if str == nil then result = true else
            if wbase.String.Trim(str) == "" then result = true end
        end
        return result
    end,
    --Делает символы большими
    Upper = function(str)
        return string.upper(str)
    end,
    --Делает символы маленькими
    Lower = function(str)
        return string.lower(str)
    end,
    --Делает символы маленькими
    Replace = function(str,that,tothat)
        return str:gsub(that,tothat)
    end,
    --Ищет в строке строку
    Find = function(str,whatfind)
        return select(2, string.gsub(str, whatfind, ""))
    end,
    --Превращает строку в таблицу разделяя её на части
    Split = function(str, pattern)
        local Table = {}
        local fpat = "(.-)" .. pattern
        local last_end = 1
        local s, e, cap = str:find(fpat, 1)
        while s do
            if s ~= 1 or cap ~= "" then
            table.insert(Table,cap)
            end
            last_end = e+1
            s, e, cap = str:find(fpat, last_end)
        end
        if last_end <= #str then
            cap = str:sub(last_end)
            table.insert(Table, cap)
        end
        return Table
    end
}

wbase.Programms = {
    --Получить кол-во запущеных приложений
    RunnedCount = function(programmname)
        return wbase.String.Find(wbase.CMD('tasklist /FI "IMAGENAME eq woowzengine.exe"'),programmname..".exe")
    end
}

wbase.Json = {
    --Конвертировать в json
    Encode = function(tab)
        return json.encode(tab)
    end,
    --Json конвертировать в таблицу
    Decode = function(str)
        return json.decode(str)
    end
}

wbase.File = {
    --Получить содержимое файла
    Read = function(path)
        path = wbase.PathToGame..path
        local file = sys.File(path)
        file:open("read")
        local result = file:read()
        file:close()
        return tostring(result)
    end
}

--Запустить команду cmd и получить результат
wbase.CMD = function(cmd, raw)
    local f = assert(io.popen(cmd, 'r'))
    local s = assert(f:read('*a'))
    f:close()
    if raw then return s end
    s = string.gsub(s, '^%s+', '')
    s = string.gsub(s, '%s+$', '')
    s = string.gsub(s, '[\n\r]+', ' ')
    return s
end

return wbase