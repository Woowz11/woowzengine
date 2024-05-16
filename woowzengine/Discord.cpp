#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include "Discord.h"
#include "discord-files/discord.h"
#include "Base.h"

#include "fcntl.h"
#include "io.h"

using namespace std;
discord::ClientId ApplicationID = 1240635259221970954;

discord::Core* core{};

bool Launched = false;
bool ActivityShow = true;

string ActTitle = "Error ActTitle";
string ActDescription = "Error ActDescription";

void SetDiscordActivityTitle(string s) {
    ActTitle = s;
}

void SetDiscordActivityDescription(string s) {
    ActDescription = s;
}

const char* StringToDiscordString(string Str) { /*Точная копия StringToConstChar*/
    char* result = new char[Str.length() + 1];
    for (int i = 0; i < Str.length(); i++) {
        result[i] = Str[i];
    }
    result[Str.length()] = '\0';
    return result;
}

string DiscordStirngToString(const string& utf8Str) {
    return "буба";
}

void DiscordStart() {
	discord::Core::Create(ApplicationID, DiscordCreateFlags_Default, &core);
    core->SetLogHook(discord::LogLevel::Debug, &DiscordErrors);
    ActivityShow = StringToBool(GetEngineInfo("DiscordActivities"));
    ActTitle = GetGameInfo("Name");
    ActDescription = GetGameInfo("Author") + " " + GetGameInfo("Version");
    Launched = true;
}

void DiscordErrors(discord::LogLevel level, const char* message)
{
    PE(message,"DISCORD");
}

void UpdateActivity() {
    discord::Activity activity{};
    activity.SetDetails(StringToDiscordString(ActTitle));
    activity.SetState(StringToDiscordString(ActDescription));

    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        if (result != discord::Result::Ok) {
            PW("Error updating activity!", "DISCORD");
        }
    });
}

void DiscordTest() {
    //discord::User um;
    //core->UserManager().GetCurrentUser(&um);
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //cout << um.GetUsername() << "\n";
    //string username = DiscordStirngToString(um.GetUsername());
    //PP(username);
}

void DiscordUpdate() {
    if (Launched) {
        if (ActivityShow) { UpdateActivity(); }
        core->RunCallbacks();
    }
}

void DiscordEnd() {
   delete core;
}