#pragma warning(disable : 4101)

#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <sol/sol.hpp>
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <future>
#include "Discord.h"
#include "discord-files/discord.h"
#include "Base.h"
#include "OpenGame.h"
#include "Easyer.h"
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

discord::UserId StringToUserId(string str) {
    long long id = 495215150165524481;
    try {
        id = stoll(str);
    }
    catch (const std::invalid_argument& e) {
        PE("Failed to convert a string to a Discord user id! StringToUserId('"+str+"')","E0033");
    }
    return discord::UserId(id);
}

void NotFoundUser(string userid) {
    PE("Discord user (" + userid + ") not found!", "E0032");
}

void GetDiscordUserInfo(string userid_, sol::function func) {
    if (DiscordLaunched_) {
        discord::UserId userid = StringToUserId(userid_);

        core->UserManager().GetUser(userid, [func, userid, userid_](discord::Result result, discord::User user) {
            if (result == discord::Result::Ok) {
                string username = user.GetUsername();
                string avatarid = user.GetAvatar();
                bool thatbot = user.GetBot();
                string discriminator = user.GetDiscriminator();
                func(username, thatbot, avatarid, discriminator);
            }
            else {
                NotFoundUser(userid_);
            }
        });
    }
    else {
        PE("Discord not found or not running! GetDiscordUserInfo('"+userid_+"')","E0034");
    }
}

string GetDiscordCurrentUser() {
    if (DiscordLaunched_) {
        discord::User user;
        core->UserManager().GetCurrentUser(&user);
        string result = to_string(user.GetId());
        DebugPrint_("function not work. WIP");
        return result;
    }
    else { PE("Discord not found or not running! GetDiscordCurrentUser()", "E0035"); return "ERROR_E0035"; }
}

void DiscordTest() {
  
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