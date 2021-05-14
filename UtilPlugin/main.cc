#include "DynRPG/Character/AnimationType.hpp"
#include "DynRPG/CommonEvent/CommonEvent.hpp"
#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include "DynRPG/Map/Map.hpp"
#include "DynRPG/Switch/Switches.hpp"
#include "DynRPG/Terrain/Terrain.hpp"
#include <DynRPG/DynRPG.hpp>
#include <fmt/core.h>

#include <ratio>
#include <string>
#include <string_view>
#include <charconv>
#include <chrono>
#include <thread>

void change_anim(std::string_view command) {
    fmt::print("Change Anim String: {}\n", command);
    auto pos = command.find(",");
    if (pos == std::string::npos) return;
    int event_id;
    uchar anim_type;
    std::from_chars(command.data(), command.data() + pos, event_id);
    std::from_chars(command.data() + pos + 1, command.data() + command.size(), anim_type);
    fmt::print("ints: {}, {}\n", event_id, anim_type);
    // RPG::map->events.get(event_id)->animationType = RPG::AnimationType(anim_type);
    RPG::map->events.get(event_id)->setAnimationType(RPG::AnimationType(anim_type));
}

bool onComment(const char *text, const RPG::ParsedCommentData *parsedData, RPG::EventScriptLine *nextScriptLine, RPG::EventScriptData *scriptData, int eventId, int pageId, int lineId, int *nextLineId) {
    std::string_view comment = text;

    if ((comment.starts_with("@change_anim(") and comment.ends_with(")"))) {
        change_anim(comment.substr(13, comment.length() - 14));
        return true;
    }

    return false;
}

// usefull for measuring frametimes
// std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();

// void onDrawScreen() {
//     std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
//     fmt::print("DrawScreen Time Diff = {:6}μs\n", std::chrono::duration_cast<std::chrono::microseconds>(now - last).count());
//     last = now;
// }

// void onFrame(RPG::Scene scene) {
//     std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
//     fmt::print("OnFrame Time Diff    = {:6}μs\n", std::chrono::duration_cast<std::chrono::microseconds>(now - last).count());
//     last = now;
// }

bool onStartup(char *pluginName) {
    fmt::print("Utility Plugin loaded.\n");

    // ::CoUninitialize();
    return true;
}
