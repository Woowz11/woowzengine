@echo off
echo Start woowzengine compile...
cd %~dp0
for /f "tokens=*" %%b in ('powershell -Command "(Get-Content 'game\gameinfo.json' | ConvertFrom-Json).Icon"') do set icon=%%b
rtc -s -c -i %icon%.ico -w src\woowzengine.lua src\*.lua src\lib\*.lua game\*.lua
for /f "tokens=*" %%a in ('powershell -Command "(Get-Content 'game\gameinfo.json' | ConvertFrom-Json).GameName"') do set gamename=%%a
for %%i in ("%~dp0*.lnk") do del "%%i"
powershell.exe -Command "$s=(New-Object -COM WScript.Shell).CreateShortcut('%~dp0%gamename%.lnk');$s.TargetPath='%~dp0\woowzengine.exe';$s.IconLocation='%~dp0%icon%.ico';$s.Save()"