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
#include "WindowsElements.h"

using namespace std;

discord::Core* core{};

bool Launched = false;
bool ActivityShow = true;

string ActTitle = "Error ActTitle";
string ActDescription = "Error ActDescription";

bool DiscordLaunched_ = false;

bool DiscordLaunched() {
    return ProgramLaunched("Discord.exe") && CheckInternet();
}

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

void DiscordStart(string ApplicationID) {
    DiscordLaunched_ = DiscordLaunched();
    if (DiscordLaunched_) {
        char* endPtr;
        long long id = strtoll(ApplicationID.c_str(), &endPtr, 10);
        if (*endPtr != '\0') {
            id = 1240635259221970954;
            PE("Failed to get Discord application ID! DiscordStart('" + ApplicationID + "')", "E0017");
        }
        discord::Core::Create(id, DiscordCreateFlags_Default, &core);
        core->SetLogHook(discord::LogLevel::Debug, &DiscordErrors);
        ActivityShow = StringToBool(GetEngineInfo("DiscordActivities"));
        ActTitle = GetGameInfo("Name");
        ActDescription = GetGameInfo("Author") + " " + GetGameInfo("Version");
        Launched = true;
        P("DISCORD", "Discord Loaded!");
    }
    else {
        P("DISCORD", "Discord is not running!");
    }
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
    if (DiscordLaunched_) {
        delete core;
    }
}