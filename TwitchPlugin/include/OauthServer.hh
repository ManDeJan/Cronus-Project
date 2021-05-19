#pragma once
#include <httplib.h>
#include <memory>
#include <string>
#include <thread>

struct TwitchOAUTHServer {
    httplib::Server svr;
    std::unique_ptr<std::thread> http_thread;
    std::string oauth_token;

    TwitchOAUTHServer();
    void run();
    void stop();
    void openLoginBrowser();
};
