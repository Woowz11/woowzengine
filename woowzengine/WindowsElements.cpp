#pragma warning(disable : 4996)

#define _HAS_STD_BYTE 0
#include "Windows.h"

#include <iostream>
#include <string>
#include <map>
#include <Lmcons.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include "WindowsElements.h"
#include "Easyer.h"
#include "Base.h"
#include "Logger.h"
#include "ShlObj.h"
#include "vector"
#include "fstream"
#include <mmdeviceapi.h>
#include <endpointvolume.h>

using namespace std;

bool Fatal = false;
bool GetFatal() {
	return Fatal;
}
void MessageBoxFatal(string Error, string Code, bool BrokenLogs) {
	Fatal = true;
	SetConsoleTitle(StringToLPCWSTR("WoowzEngine Console [Fatal error!]"));
	if (!BrokenLogs) { LogsFatal(); }
	string LookAtLog = "Look at log!";
	if (BrokenLogs) { LookAtLog = "Logs corrupted!"; }
	int MBID = MessageBox(NULL, StringToLPCWSTR(Error+"\n"+LookAtLog+" ("+Code+")"), (LPCWSTR)L"WoowzEngine fatal error!", MB_OK | MB_ICONERROR);

	if (MBID == IDOK) {
		Exit();
	}
	Exit();
}

bool SupportedWindowsVersion() {
	OSVERSIONINFOEX info;
	ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((LPOSVERSIONINFO)&info);
	int minor = DWORDToInt(info.dwMinorVersion);
	int major = DWORDToInt(info.dwMajorVersion);

	bool result = false;

	if (minor > 1) {
		result = true;
	}

	if (major != 6) {
		result = false;
	}

	return result;
}

string GetUserName_() {
	TCHAR name[UNLEN + 1];
	DWORD size = UNLEN + 1;

	if (GetUserName(name, &size)) {
		return TCHARToString(name);
}
	else {
		return "Unnamed User!";
	}
}

bool CheckInternet() {
#ifdef _WIN32
	int result = system("ping -n 1 google.com > nul");
#else
	int result = system("ping -c 1 google.com > /dev/null");
#endif

	return result == 0;
}

bool ProgramLaunched(string name) {
	bool result = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry)) {
		do {
			if (wcscmp(entry.szExeFile, ConstCharToConstWChar(StringToConstChar(name))) == 0) {
				result = true;
			}
		} while (Process32Next(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return result;
}

void StopProgram(string name) {
	for (const auto& pair : ProgramsLaunched()) {
		if (pair.second == name) {
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE, FALSE, pair.first);
			TerminateProcess(hProcess,0);
		}
	}
}

void StopPC() {
	system("shutdown -s");
}

map<int,string> ProgramsLaunched() {
	map<int,string> result = {};
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(snapshot, &entry)) {
		do {
			result[DWORDToInt(entry.th32ProcessID)] = ConstWCharToString(entry.szExeFile);
		} while (Process32Next(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return result;
}

void OpenFile(string path_) {
	wstring path = StringToWString(path_);
	ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

string GetSystemLanguage() {
	LCID langId = GetUserDefaultLCID();
	WCHAR language[6];
	GetLocaleInfo(langId, LOCALE_SISO639LANGNAME, language, 6);
	return ConstWCharToString(language);
}

string GetLanguage() { /*Я без понятия хули оно не обновляется*/
	HKL keyboardLayout = GetKeyboardLayout(0);
	LANGID langId = LOWORD(keyboardLayout);
	WCHAR language[6];
	GetLocaleInfo(langId, LOCALE_SISO639LANGNAME, language, 6);
	return ConstWCharToString(language);
}

void SetDesktopBackground(string img) {
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, StringToLPWSTR(img), SPIF_UPDATEINIFILE);
	P("DESKTOP","New background desktop image installed!");
}

void SetVolume(int volume) {
	IMMDeviceEnumerator* deviceEnumerator = nullptr;
	CoInitialize(nullptr);
	CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&deviceEnumerator));

	IMMDevice* device = nullptr;
	deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);

	IAudioEndpointVolume* audioEndpointVolume = nullptr;
	device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&audioEndpointVolume));

	audioEndpointVolume->SetMasterVolumeLevel(-64*(1-(float(volume)/100)), nullptr);

	audioEndpointVolume->Release();
	device->Release();
	deviceEnumerator->Release();
	CoUninitialize();
}

int GetVolume() {
	IMMDeviceEnumerator* deviceEnumerator = nullptr;
	CoInitialize(nullptr);
	CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&deviceEnumerator));

	IMMDevice* device = nullptr;
	deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);

	IAudioEndpointVolume* audioEndpointVolume = nullptr;
	device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&audioEndpointVolume));

	float volumeLevel;
	audioEndpointVolume->GetMasterVolumeLevel(&volumeLevel);

	audioEndpointVolume->Release();
	device->Release();
	deviceEnumerator->Release();
	CoUninitialize();

	float volumePercentage = volumeLevel; /*Я короче пока-что хз как децибелы нормальн в проценты конвертировать*/
	return int(round(volumePercentage));
}