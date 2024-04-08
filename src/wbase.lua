local wbase = {}

wbase.Time = 0

wbase.Sleep = function(n)
    --local t0 = sys.clock()
    --while sys.clock() - t0 <= n do end
    sys.cmd("sleep "..tonumber(n))

end

return wbase