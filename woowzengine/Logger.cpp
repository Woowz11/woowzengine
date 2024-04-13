#include <iostream>
#include <string>
#include "Logger.h"
#include "Base.h"
#include "WindowsElements.h"
#include "Files.h"

using namespace std;

string NowLog = "";
string LogsPath = "woowzengine/log/";
string LogsType = "log";

/*Установка логов*/
void LoggerInstall() {
	string LogsType_ = GetEngineInfo("LogType");
	if (NameWindowsAccept(LogsType_, true)) {
		LogsType = LogsType_;
	}
	else {
		MessageBoxFatal("LogType (engine.json) contains illegal characters in windows (\\ / : * ? \" < > | + .)", "C0011", true);
	}

	if (!NameWindowsAccept(GetEngineInfo("LogFatal"))) {
		MessageBoxFatal("LogFatal (engine.json) contains illegal characters in windows (\\ / : * ? \" < > | + .)", "C0015", true);
	}

	string LogFormat = GetEngineInfo("LogFormat");
	if(StringEmpty(LogFormat)){ MessageBoxFatal("LogsFormat (engine.json) can't be empty!", "C0012", true); }
	if(!NameWindowsAccept(LogFormat)){ MessageBoxFatal("LogFormat (engine.json) contains illegal characters in windows (\\ / : * ? \" < > | + .)", "C0013", true); }
	string LogName = ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(LogFormat, "%w", GetTime("w")), "%y", GetTime("y")), "%mn", GetTime("mn")), "%d", GetTime("d")), "%h", GetTime("h")), "%m", GetTime("m")), "%s", GetTime("s"));
	CreateLog(LogName);
}

/*Создать лог*/
void CreateLog(string LogName) {
	NowLog = LogName;
	string log = GetSessionInfo("GamePath") + LogsPath + LogName + "." + LogsType;
	if (HasDirectory(log)) { PF("Log file with that name ["+log+"] already exists! Check LogFormat (engine.json), it could be because of that!", "C0003", true); }
	else {
		GetOrCreateFile(log);
	}
	SetSessionInfo("Log",log);
}

/*Записать в лог*/
void PrintToLog(string Text) {
	if (GetSessionInfo("GamePath") == "ERROR_SESSIONINFO_NOT_FOUND") { MessageBoxFatal("Can't save log, sessioninfo file not found!","C0004", true); }
	if (!HasDirectory(GetSessionInfo("GamePath") + LogsPath)) { PF("Folder logs not found!","C0005", true); }
	if (!HasDirectory(GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType)) { PF("Log file not found!","C0006", true); }
	string Log = GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType;
	WriteToFile(Log, Text, true);
}

/*Обозначает что в логах фатальная ошибка*/
void LogsFatal() {
	RenameFile(GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType, GetSessionInfo("GamePath") + LogsPath + NowLog + GetEngineInfo("LogFatal") + "." + LogsType);
}