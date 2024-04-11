#define _HAS_STD_BYTE 0
#include "Windows.h"

#include <iostream>
#include <string>
#include "WindowsElements.h"
#include "Easyer.h"
#include "Base.h"
#include "Logger.h"

using namespace std;

void MessageBoxFatal(string Error, string Code, bool BrokenLogs) {
	SetConsoleTitle(StringToLPCWSTR("WoowzEngine Console [Fatal error!]"));
	if (!BrokenLogs) { LogsFatal(); }
	string LookAtLog = "Look at log!";
	if (BrokenLogs) { LookAtLog = "Logs corrupted!"; }
	int MBID = MessageBox(NULL, StringToLPCWSTR(Error+"\n"+LookAtLog+" ("+Code+")"), (LPCWSTR)L"WoowzEngine fatal error!", MB_OK | MB_ICONERROR);

	if (MBID == IDOK) {
		Exit();
	}
}