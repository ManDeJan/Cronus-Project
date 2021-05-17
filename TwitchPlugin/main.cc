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

std::unique_ptr<std::thread> http_thread;
httplib::Server svr;

void onInitFinished()
{
    svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Hello World!", "text/plain");
    });

    http_thread = std::make_unique<std::thread>([&]() { svr.listen("0.0.0.0", 42069); });

    fmt::print("Twitch plugin loaded.\n");
}