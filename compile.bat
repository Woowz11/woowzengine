@echo off
echo Start woowzengine compile...
cd %~dp0
rtc -s -c -w src\woowzengine.lua src\*.lua game\*.lua