local f = {}

f.Print = function(text)
	PrintCustom("WSCENE",text)
end

f.Warn = function(text)
	PrintCustom("WSCENE",text,"?",14)
end

return f