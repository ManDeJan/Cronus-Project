#include "DynRPG/Attribute/Attribute.hpp"
#include "DynRPG/Battle/BattleData.hpp"
#include "DynRPG/Character/Hero.hpp"
#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include <DynRPG/DynRPG.hpp>
#include <fmt/core.h>
#include <string_view>
#include <vector>
#include <set>
#include <string>

#include "DynRPG/System/System.hpp"
#include "DynRPG/Variable/Variables.hpp"
#include "OauthServer.hh"
#include "TwitchChat.hh"

#include <cassert>

TwitchOAUTHServer srv;
TwitchChat tc;

void twitchAuth() {
    srv.run();
    srv.openLoginBrowser();
}

bool twitchChatJoin() {
    fmt::print("Join chat!\n");
    try {
        srv.stop();
        tc.connect();
        tc.run();
        tc.login(srv.oauth_token, "thisisanusername"); // haha anus
        tc.join(RPG::hero->getName());
        tc.sendMessage(RPG::hero->getName(), "...");
        return true;
    } catch (std::exception &e) {
        fmt::print(stderr, "ERRR {}\n", e.what());
        tc.stop();
        srv.oauth_token = "";
        return false;
    }
}

bool onComment(const char *text, const RPG::ParsedCommentData *parsedData, RPG::EventScriptLine *nextScriptLine,
               RPG::EventScriptData *scriptData, int eventId, int pageId, int lineId, int *nextLineId) {
    fmt::print("cmd? {}\n", parsedData->command);
    if (strcmp(parsedData->command, "twitchlogin") == 0) {
        twitchAuth();
        return false;
    } else if (strcmp(parsedData->command, "twitchauthcheck") == 0) {
        if (!srv.oauth_token.empty()) {
            if (twitchChatJoin()) {
                nextScriptLine->command = RPG::EventCommand::ShowMessage;
                nextScriptLine->stringParameter = "Success";
            };
        }
        return false;
    }
    return true;
}

std::set<std::string> usernames;
// :thetruesaltking!thetruesaltking@thetruesaltking.tmi.twitch.tv PRIVMSG #dzpg1 :@ManDeJan I am increasingly feeling like this is some sort of ARG in DZ's chat
bool onStartup(char *pluginName) {
    fmt::print("Twitch Plugin loaded.\n");
    return true;
}

void parseMessage(std::string_view message) {
    auto pos = message.find("PRIVMSG");
    if (pos == std::string::npos) return;
    fmt::print("Parsing private message msg...\n");
    auto pos2 = message.find("!");
    std::string username{message.begin()+1, message.begin() + pos2};
    fmt::print("Username found: {}\n", username);
    auto pos3 = message.find(":", pos2);
    std::string real_msg{message.begin() + pos3 + 1, message.end()};
    fmt::print("Real message: {}\n", real_msg);

    if (real_msg.starts_with("!join")) {
        usernames.insert(username);
        tc.sendMessage(RPG::hero->getName(), fmt::format("{} joined the game. But what game tho?", username));
    } else if (real_msg.starts_with("!list")) {
        std::string people = "";
        for (auto &user : usernames) {
            people += user;
            people += " ";
        }
        tc.sendMessage(RPG::hero->getName(), fmt::format("List of people in the game: {}", people));
    }
}

void onFrame(RPG::Scene scene) {
    while (tc.message_queue.peek()) {
        auto msg = *tc.message_queue.pop();
        // fmt::print("Found a message in the queue!\n --- {}\n", msg);
        parseMessage(msg);
    }
}
