local json = require("json")

local wbase = {}

wbase.Time = 0
wbase.StartDate = sys.Datetime()

wbase.Settings = {
    Console = true,
    Game = {
        Name = "New Game",
        Version = "0.0.0",
        Author = "Unknown"
    },
    Version = "0"
}

wbase.Sleep = function(n)
    --local t0 = sys.clock()
    --while sys.clock() - t0 <= n do end
    sys.cmd("sleep "..tonumber(n))

end

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
    ToTable = function(str)
        return json.decode(str)
    end

}

wbase.Json = {
    --Конвертировать в json
    ToJson = function(tab)
        return json.encode(tab)
    end,
    --Json конвертировать в таблицу
    ToTable = function(str)
        return json.decode(str)
    end
}

--Получить содержимое файла
wbase.ReadFile = function(path)
    
end

return wbase