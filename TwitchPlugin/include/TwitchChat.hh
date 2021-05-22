#pragma once
#include "ChatSocket.hh"
#include <thread>

class TwitchChat {
private:
    std::thread ChatThread;
    ChatSocket cs;

public:
    decltype(cs.messages) &message_queue = cs.messages;

    void connect();
    void run();

    void stop();

    void login(std::string_view oauth_token, std::string_view user);
    void join(std::string_view channel_name);
    void sendMessage(std::string_view channel_name, std::string_view msg);
};
