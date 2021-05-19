#include "DynRPG/Character/AnimationType.hpp"
#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include "DynRPG/Map/Map.hpp"
#include <DynRPG/DynRPG.hpp>
#include <fmt/core.h>

#include "TwitchChat.hh"
#include "OauthServer.hh"


TwitchOAUTHServer srv;
TwitchChat tc;

void onInitFinished() {
    srv.run();
    srv.openLoginBrowser();
    fmt::print("Twitch plugin loaded.\n");
}

void onNewGame() { srv.stop(); }
void onLoadGame(int id, char *data, int length) { srv.stop(); }

int main(int argc, char *argv[]) {
    assert(argc == 3);
    try {
        tc.connect();
        tc.run();
        tc.login(argv[2], argv[1]);
        tc.join("vagabonddog");
        tc.sendMessage("vagabonddog", "This was sent from my c++ plugin :D");
        while (true) { fmt::print("Msg: {}", tc.message_queue.pop_blocking()); };
    } catch (std::exception &e) {
        tc.stop();
        fmt::print(stderr, "Exception: {}", e.what());
    }
    while (true) {}
}