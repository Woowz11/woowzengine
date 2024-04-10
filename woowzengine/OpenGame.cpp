#include <iostream>
#include <string>
#include "OpenGame.h"
#include "Base.h"
#include "Files.h"

using namespace std;

string GamePath = "";

void CheckFiles() {
	GetOrCreateFolder(GamePath + "woowzengine");
	GetOrCreateFolder(GamePath + "woowzengine/log");
	GetOrCreateFolder(GamePath + "woowzengine/temporary");
	GetOrCreateFile  (GamePath + "woowzengine/temporary/sessioninfo.json");
}
void Install() {
	CheckFiles();

}

void OpenGame(string GamePath_) {
	GamePath = GamePath_;
	Install();

	string file = GamePath + "woowzengine/temporary/sessioninfo.json";
	WriteToFile(file, "START", false);
	WriteToFile(file, "a1", true);
	WriteToFile(file, "b2", true);
	WriteToFile(file, "c3", true);
	WriteToFile(file, "d4", true);
	WriteToFile(file, "e5", true);
	WriteToFile(file, "f6", true);
	WriteToFile(file, "g&", true);
	WriteToFile(file, "eND", true);

}