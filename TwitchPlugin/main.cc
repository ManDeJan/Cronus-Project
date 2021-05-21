#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include <DynRPG/DynRPG.hpp>
#include <fmt/core.h>

#include "OauthServer.hh"
#include "TwitchChat.hh"

#include <cassert>

TwitchOAUTHServer srv;
TwitchChat tc;

void twitchAuth() {
    srv.run();
    srv.openLoginBrowser();
}

void twitchChatJoin() {
    fmt::print("Join chat!\n");
    try {
        srv.stop();
        tc.connect();
        tc.run();
        tc.login(srv.oauth_token, "thisisanusername");
        tc.join(RPG::hero->getName());
        tc.sendMessage(RPG::hero->getName(), "Cronus bot joined the chat!");
    } catch (std::exception &e) {
        fmt::print(stderr, "ERRR {}\n", e.what());
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
            twitchChatJoin();
            nextScriptLine->command = RPG::EventCommand::ShowMessage;
            nextScriptLine->stringParameter = "Success";
        }
        return false;
    }
    return true;
}

// int main(int argc, char *argv[]) {
//     srv.run();
//     srv.openLoginBrowser();
//     srv.run();
//     srv.openLoginBrowser();
//     while(srv.oauth_token == ""){}
//     tc.connect();
//     tc.run();
//     tc.login(srv.oauth_token, "asdf");
//     tc.join(RPG::Hero::getName());
//     while (true) { fmt::print("Msg: {}", tc.message_queue.pop_blocking()); };
// }