@echo off
echo Start woowzengine compile...
cd %~dp0
rtc -s -c -w src\woowzengine.lua luart\*.lua src\*.lua game\*.lua