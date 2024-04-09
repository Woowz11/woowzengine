@echo off
for /f "tokens=*" %%a in ('powershell -Command "(Get-Content 'game\gameinfo.json' | ConvertFrom-Json).GameName"') do set gamename=%%a
title "%gamename%" compilation
echo Start "%gamename%" on woowzengine compile...
cd %~dp0
for /f "tokens=*" %%b in ('powershell -Command "(Get-Content 'game\gameinfo.json' | ConvertFrom-Json).Icon"') do set icon=%%b
for %%i in ("%~dp0*.exe") do del "%%i"
rtc -c -w -i %icon%.ico -o "%gamename%.exe" -laudio -lcore src\woowzengine.wlua src\*.wlua src\lib\*.wlua game\*.wlua
echo End "%gamename%" compile!
pause