#include <string>
#pragma once
using namespace std;

void LoggerInstall();
void CreateLog(string LogName);
void PrintToLog(string Text); 
void LogsFatal();
void LogsErrors();