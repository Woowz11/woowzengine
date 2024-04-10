@echo off
set /a starttime= %time:~-8,-6% * 60000 + %time:~-5,-3% * 1000 + %time:~-2%
for /f "tokens=*" %%a in ('powershell -Command "(Get-Content 'game\gameinfo.json' | ConvertFrom-Json).GameName"') do set gamename=%%a
title "%gamename%" compilation
for /f "tokens=*" %%c in ('powershell -Command "(Get-Content 'info.json' | ConvertFrom-Json).Version"') do set version=%%c
echo Start "%gamename%" on woowzengine [%version%] compile...
cd %~dp0
for /f "tokens=*" %%b in ('powershell -Command "(Get-Content 'game\gameinfo.json' | ConvertFrom-Json).Icon"') do set icon=%%b
for %%i in ("%~dp0*.exe") do del "%%i"
rtc -c -w -i %icon%.ico -o "%gamename%.exe" -laudio -lcore lua\woowzengine.wlua lua\*.wlua game\*.wlua
set /a endtime= %time:~-8,-6% * 60000 + %time:~-5,-3% * 1000 + %time:~-2%
set /a timeresult= %endtime% - %starttime%
echo End "%gamename%" compile [%timeresult%ms]!
pause