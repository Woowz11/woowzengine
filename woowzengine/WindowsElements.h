#pragma once
#include <map>
using namespace std;

void MessageBoxFatal(string Error,string Code,bool = false);
bool ProgramLaunched(string name);
map<int, string> ProgramsLaunched();
bool CheckInternet();
string GetUserName_();
void StopProgram(string name);
void StopPC();
bool GetFatal();