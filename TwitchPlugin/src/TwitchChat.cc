#include "TwitchChat.hh"
#include <cassert>
#include <fmt/core.h>

void TwitchChat::connect() { cs.connect(); }

void TwitchChat::run() {
    cs.running.store(true);
    ChatThread = std::thread([&]() {
        try {
            cs.run();
        } catch (std::exception &e) {
            fmt::print(stderr, "Error {}", e.what());
            throw;
        }
    });
}

void TwitchChat::stop() {
    cs.running.store(false);
    cs.disconnect();
    ChatThread.join();
}

void TwitchChat::login(std::string_view oauth_token, std::string_view user) {
    assert(oauth_token.find("\n") == std::string_view::npos);
    assert(user.find("\n") == std::string_view::npos);

    auto login_msg = fmt::format("PASS oauth:{}\nNICK {}\n", oauth_token, user);
    fmt::print("Trying to login with msg: \n{}\n", login_msg);
    cs.send(login_msg);
    std::string msg;
    for (int i = 0; i < 7; i++) { msg = message_queue.pop_blocking(); }
    if (msg.find(" 376 ") == std::string::npos) { throw std::runtime_error("Login failed"); };
}

void TwitchChat::join(std::string_view channel_name) {
    assert(channel_name.find("\n") == std::string_view::npos);
    fmt::print("Joining channel {}", channel_name);
    cs.send(fmt::format("JOIN #{}\n", channel_name));
    std::string msg;
    do {
        msg = message_queue.pop_blocking();
        fmt::print("{}", msg);
    } while (msg.find("End of /NAMES list") == std::string::npos);
}

void TwitchChat::sendMessage(std::string_view channel_name, std::string_view msg) {
    assert(channel_name.find("\n") == std::string_view::npos);
    assert(msg.find("\n") == std::string_view::npos);
    fmt::print("Sending msg \"{}\" to channel {}", msg, channel_name);
    cs.send(fmt::format("PRIVMSG #{} : {}\n", channel_name, msg));
}
