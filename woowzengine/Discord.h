#pragma once
#include <string>
#include "discord-files/discord.h"

using namespace std;

void DiscordStart(string ApplicationID);
void DiscordUpdate();
void DiscordEnd();
void UpdateActivity();
const char* StringToDiscordString(string Str);
void SetDiscordActivityTitle(string s);
void SetDiscordActivityDescription(string s);
void DiscordErrors(discord::LogLevel level, const char* message);
void DiscordTest();
string DiscordStirngToString(const string& utf8Str);
bool DiscordLaunched();