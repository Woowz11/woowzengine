@echo off
REM run.bat file WoowzEngine

woowzengine.exe run %~dp0

for /F "skip=2 tokens=2*" %%a in ('REG QUERY "HKEY_CURRENT_USER\Control Panel\International" /v "LocaleName"') do set "OSLanguage=%%b"
if "%OSLanguage%"=="ru-RU" || "%OSLanguage%"=="uk-UA" || "%OSLanguage%"=="be-BY" || "%OSLanguage%"=="kk-KZ" || "%OSLanguage%"=="tt-RU" || "%OSLanguage%"=="ky-KG" || "%OSLanguage%"=="tk-TM" || "%OSLanguage%"=="uz-Latn-UZ" || "%OSLanguage%"=="az-Latn-AZ" || "%OSLanguage%"=="th-Cyrl-TJ" || "%OSLanguage%"=="ba-RU" || "%OSLanguage%"=="sah-RU" || "%OSLanguage%"=="uz-Cyrl-UZ" || "%OSLanguage%"=="sr-Cyrl-ME" (
	if errorlevel 1 echo "�᫨ �� ����� ��� �訡��, ��ᬮ��� ���㬥���� (https://woowz11.github.io/woowzsite/woowzengine.html), ��� ᮮ��� Woowz11"
) else (
	if errorlevel 1 echo "If you see an error, look at the documentation (https://woowz11.github.io/woowzsite/woowzengine.html) or tell Woowz11"
)
	
if errorlevel 1 pause