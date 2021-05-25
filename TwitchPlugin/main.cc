#include "DynRPG/Attribute/Attribute.hpp"
#include "DynRPG/Battle/BattleData.hpp"
#include "DynRPG/Character/Hero.hpp"
#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include <DynRPG/DynRPG.hpp>
#include <fmt/core.h>
#include <iterator>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "DynRPG/Map/Map.hpp"
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
        tc.sendMessage(RPG::hero->getName(), "Cronus game connected to twitch chat!");
        return true;
    } catch (std::exception &e) {
        fmt::print(stderr, "Error: {}\n", e.what());
        tc.stop();
        srv.oauth_token = "";
        return false;
    }
}

bool join_enabled = false;

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
    } else if (strcmp(parsedData->command, "enable_joins") == 0) {
        join_enabled = true;
        tc.sendMessage(RPG::hero->getName(), "You can now use !join to join the game!");
    }
    return true;
}

std::set<std::string> usernames;
std::vector<std::string> usernames_ordered;
// :thetruesaltking!thetruesaltking@thetruesaltking.tmi.twitch.tv PRIVMSG #dzpg1 :@ManDeJan I am increasingly feeling
// like this is some sort of ARG in DZ's chat
bool onStartup(char *pluginName) {
    fmt::print("Twitch Plugin loaded.\n");
    return true;
}

struct TwitchActor {
    int original_x;
    int original_y;
    int eventId;
    int charName;
};

std::vector<TwitchActor> available_actors{
    {0, 0, 8, 2},    {2, 0, 9, 3},    {4, 0, 10, 4},   {6, 0, 11, 5},   {8, 0, 12, 6},
    {10, 0, 13, 7},  {12, 0, 24, 8},  {14, 0, 25, 9},  {16, 0, 27, 10}, {18, 0, 28, 11},
    {20, 0, 29, 12}, {22, 0, 30, 13}, {24, 0, 31, 14}, {26, 0, 32, 15}, {28, 0, 33, 16},
    {30, 0, 34, 17}, {32, 0, 35, 18}, {34, 0, 36, 19}, {36, 0, 37, 20},
};
std::vector<TwitchActor> running_actors;

void addRandomSub() {
    if (available_actors.empty()) return;
    auto id = rand() % available_actors.size();
    auto actor = available_actors[id];
    available_actors.erase(available_actors.begin() + id);
    running_actors.push_back(actor);
    auto event = RPG::map->events.get(actor.eventId);
    event->x = 41;
    event->y = 21;
    // RPG::actors.get(actor.charName)->name = "This is a test message\nMultiple lines";
}

void addSub(int id) {
    if (available_actors.empty()) return;
    auto actor = available_actors[id];
    // available_actors.erase(available_actors.begin() + id);
    running_actors.push_back(actor);
    auto event = RPG::map->events.get(actor.eventId);
    event->x = 41;
    event->y = 21;
    // RPG::actors.get(actor.charName)->name = "This is a test message\nMultiple lines";
}

void returnSub(int id) {
    auto res =
        std::find_if(running_actors.begin(), running_actors.end(), [&](TwitchActor a) { return a.eventId == id; });
    if (res != running_actors.end()) {
        auto event = RPG::map->events.get(id);
        event->x = res->original_x;
        event->y = res->original_y;
        // available_actors.push_back(*res);
        // running_actors.erase(res);
    }
};

void parseMessage(std::string_view message) {
    auto pos = message.find("PRIVMSG");
    if (pos == std::string::npos) return;
    fmt::print("Parsing private message msg...\n");
    auto pos2 = message.find("!");
    std::string username{message.begin() + 1, message.begin() + pos2};
    fmt::print("Username found: {}\n", username);
    auto pos3 = message.find(":", pos2);
    std::string real_msg{message.begin() + pos3 + 1, message.end()};
    fmt::print("Real message: {}\n", real_msg);

    if (real_msg.starts_with("!join")) {
        auto pair = usernames.insert(username);
        if (pair.second) {usernames_ordered.push_back(username);}
        tc.sendMessage(RPG::hero->getName(), fmt::format("{} joined the game. Use !say to leave a message for {}",
                                                         username, RPG::hero->getName()));
    } else if (real_msg.starts_with("!list")) {
        std::string people = "";
        for (auto &user : usernames_ordered) {
            people += user;
            people += " ";
        }
        tc.sendMessage(RPG::hero->getName(), fmt::format("List of people in the game: {}", people));
    } else if (real_msg.starts_with("!say")) {
        if (not join_enabled) return;
        auto user = std::find(usernames_ordered.begin(), usernames_ordered.end(), username);
        if (user == usernames_ordered.end()) return;
        auto idx = std::distance(usernames_ordered.begin(), user);
        RPG::actors.get(idx + 2)->name = username + " says: " + std::string(real_msg.begin() + 5, real_msg.end());
        addSub(idx);
    }
}

void onFrame(RPG::Scene scene) {
    while (tc.message_queue.peek()) {
        auto msg = *tc.message_queue.pop();
        // fmt::print("Found a message in the queue!\n --- {}\n", msg);
        parseMessage(msg);
    }
}
