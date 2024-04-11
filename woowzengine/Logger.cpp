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

/*��������� �����*/
void LoggerInstall() {
	string LogsType_ = GetEngineInfo("LogType");
	if (NameWindowsAccept(LogsType_, true)) {
		LogsType = LogsType_;
	}
	else {
		MessageBoxFatal("LogsType (engine.json) contains illegal characters in windows (\\ / : * ? \" < > | + .)", "C0011", true);
	}
	string LogFormat = GetEngineInfo("LogFormat");
	if(StringEmpty(LogFormat)){ MessageBoxFatal("LogsFormat (engine.json) can't be empty!", "C0012", true); }
	if(!NameWindowsAccept(LogFormat)){ MessageBoxFatal("LogsFormat (engine.json) contains illegal characters in windows (\\ / : * ? \" < > | + .)", "C0013", true); }
	string LogName = ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(LogFormat, "%w", GetTime("w")), "%y", GetTime("y")), "%mn", GetTime("mn")), "%d", GetTime("d")), "%h", GetTime("h")), "%m", GetTime("m")), "%s", GetTime("s"));
	CreateLog(LogName);
}

/*������� ���*/
void CreateLog(string LogName) {
	NowLog = LogName;
	if (HasDirectory(GetSessionInfo("GamePath") + LogsPath + LogName + "." + LogsType)) { PF("Log file with that name already exists! Check LogFormat (engine.json), it could be because of that!", "C0003", true); }
	else {
		GetOrCreateFile(GetSessionInfo("GamePath") + LogsPath + LogName + "." + LogsType);
	}
}

/*�������� � ���*/
void PrintToLog(string Text) {
	if (GetSessionInfo("GamePath") == "ERROR_SESSIONINFO_NOT_FOUND") { MessageBoxFatal("Can't save log, sessioninfo file not found!","C0004", true); }
	if (!HasDirectory(GetSessionInfo("GamePath") + LogsPath)) { PF("Folder logs not found!","C0005", true); }
	if (!HasDirectory(GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType)) { PF("Log file not found!","C0006", true); }
	string Log = GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType;
	WriteToFile(Log, Text, true);
}

/*���������� ��� � ����� ��������� ������*/
void LogsFatal() {
	RenameFile(GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType, GetSessionInfo("GamePath") + LogsPath + NowLog + "-FATAL" + "." + LogsType);
}