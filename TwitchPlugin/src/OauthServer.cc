#include "OauthServer.hh"
#include <fmt/core.h>

TwitchOAUTHServer::TwitchOAUTHServer() {
    svr.Get("/hi",
            [](const httplib::Request &, httplib::Response &res) { res.set_content("Hello World!", "text/plain"); });
    svr.Get("/oauth_redirect", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(R"(<html><head> <style>body{position: fixed; top: 0; bottom: 0; left: 0; right: 0; margin: 0; background: #212121; color: #fdfff1; font-size: 4vw; font-weight: 500; text-align: left; align-items: center; justify-content: center;}h1{margin: auto; text-align: center;}</style></head><body> <h1>Cronus</h1> <hr> Twitch Connection succesfull!<br>You can return to the game now! <script>var myHeaders=new Headers(); myHeaders.append("Content-Type", "text/plain"); var requestOptions={method: 'POST', headers: myHeaders, body: document.location.hash.replace(/#access_token=([^&]+).*/g, "$1"), redirect: 'follow'}; window.location.hash=''; fetch("http://localhost:42069/oauth_token", requestOptions) </script></body></html>)",
                        "text/html");
    });
    svr.Post("/oauth_token", [&](const httplib::Request &req, httplib::Response &res) {
        fmt::print("Oauth token is {}\n", "--REDACTED--"/*req.body*/);
        oauth_token = req.body;
        res.set_content("Ok", "text/plain");
    });
}

void TwitchOAUTHServer::run() {
    fmt::print("Starting http server\n");
    http_thread = std::make_unique<std::thread>([&]() { svr.listen("0.0.0.0", 42069); });
}

void TwitchOAUTHServer::stop() {
    if (!http_thread) {
        fmt::print(stderr, "Server not running, can't stop nothing\n");
        return;
    }
    fmt::print("Stopping http server\n");
    svr.stop();
    http_thread->join();
}

void TwitchOAUTHServer::openLoginBrowser() {
    ShellExecuteA(0, 0,
                  "https://id.twitch.tv/oauth2/authorize?client_id=xic4pienyb7kqh5hat3ona545wsmyw&redirect_uri=http://"
                  "localhost:42069/oauth_redirect&response_type=token&scope=chat:read chat:edit",
                  0, 0, SW_SHOW);
}
