#include "DynRPG/Character/AnimationType.hpp"
#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include "DynRPG/Map/Map.hpp"
#include <DynRPG/DynRPG.hpp>
#include <fmt/core.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <httplib.h>
#include <thread>
#include <memory>
#include <shellapi.h>

struct TwitchOAUTHServer
{
    httplib::Server svr;
    std::unique_ptr<std::thread> http_thread;
    std::string oauth_token;

    TwitchOAUTHServer()
    {
        svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
            res.set_content("Hello World!", "text/plain");
        });
        svr.Get("/oauth_redirect", [](const httplib::Request &, httplib::Response &res) {
            res.set_content(R"(<h1>success</h1>
            <script>
            var myHeaders = new Headers();
            myHeaders.append("Content-Type", "text/plain");

            var requestOptions = {
            method: 'POST',
            headers: myHeaders,
            body: document.location.hash.replace(/#access_token=([^&]+).*/g,"$1"),
            redirect: 'follow'
            };

            fetch("http://localhost:42069/oauth_token", requestOptions)
            .then(result => window.close())
            </script>
            )",
                            "text/html");
        });
        svr.Post("/oauth_token", [&](const httplib::Request &req, httplib::Response &res) {
            fmt::print("Oauth token is {}\n", req.body);
            oauth_token = req.body;
            res.set_content("Ok", "text/plain");
        });
    }
    void run()
    {
        fmt::print("Starting http server\n");
        http_thread = std::make_unique<std::thread>([&]() { svr.listen("0.0.0.0", 42069); });
    };
    void stop()
    {
        if (!http_thread)
        {
            fmt::print(stderr, "Server not running, can't stop nothing\n");
            return;
        }
        fmt::print("Stopping http server\n");
        svr.stop();
        http_thread->join();
    };

    void openLoginBrowser()
    {
        ShellExecuteA(
            0,
            0,
            "https://id.twitch.tv/oauth2/authorize?client_id=xic4pienyb7kqh5hat3ona545wsmyw&redirect_uri=http://localhost:42069/oauth_redirect&response_type=token&scope=chat:read chat:edit",
            0,
            0,
            SW_SHOW);
    }
};

TwitchOAUTHServer srv;

void onInitFinished()
{
    srv.run();
    srv.openLoginBrowser();
    fmt::print("Twitch plugin loaded.\n");
}

void onNewGame()
{
    srv.stop();
}
void onLoadGame(int id, char *data, int length)
{
    srv.stop();
}