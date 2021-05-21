#include "DynRPG/Character/AnimationType.hpp"
#include "DynRPG/CommonEvent/CommonEvent.hpp"
#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include "DynRPG/Map/Map.hpp"
#include "DynRPG/Scene/Scene.hpp"
#include "DynRPG/Switch/Switches.hpp"
#include "DynRPG/Terrain/Terrain.hpp"
#include "DynRPG/Tileset/Tileset.hpp"
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

void onFrame(RPG::Scene scene) {
    if (scene != RPG::Scene::Map) return;
    auto width = RPG::map->getWidth();
    auto height = RPG::map->getHeight();

    // auto graphics = RPG::map->properties->graphics;
    // auto name = graphics->filename.s_str();
    // fmt::print("Using tile set: {}", name);

    // for (int x = 0; x <  width;  width++)
    // for (int y = 0; y < height; height++) {
    //     auto tile_id = RPG::map->getLowerLayerTileId(x, y);
    //     auto terrain_id = RPG::map->getTerrainId(tile_id);
        
    // }
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

    return true;
}
