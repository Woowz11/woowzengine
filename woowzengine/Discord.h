#pragma once
#include <string>
#include <sol/sol.hpp>
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
bool DiscordLaunched();
void NotFoundUser(string userid);
void GetDiscordUserInfo(string userid_, sol::function func);
discord::UserId StringToUserId(string str);
string GetDiscordCurrentUser();