#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include "DynRPG/Map/Map.hpp"
#include "DynRPG/Scene/Scene.hpp"
#include "DynRPG/Scene/SceneTitle.hpp"
#include "DynRPG/System/System.hpp"
#include <DynRPG/DynRPG.hpp>
#include <corecrt.h>
#include <cstring>
#include <fmt/core.h>

#include <charconv>
#include <chrono>
#include <ratio>
#include <string>
#include <string_view>
#include <thread>
#include <iostream>
#include <fstream>
#include <iomanip>

bool onComment(const char *text, const RPG::ParsedCommentData *parsedData, RPG::EventScriptLine *nextScriptLine,
               RPG::EventScriptData *scriptData, int eventId, int pageId, int lineId, int *nextLineId) {
    // std::string_view comment = text;
    // TODO: apparantly I don't need to parse everything by hand like an idiot cause there is already parsedData
    // available If only I read past the first argument of this function 🤷🏼‍♂️
    // if ((comment.starts_with("@change_anim(") and comment.ends_with(")"))) {
    //     change_anim(comment.substr(13, comment.length() - 14));
    //     return true;
    // }

    return false;
}
int rgb_to_bgr(const int rgb) {
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = (rgb >> 0) & 0xFF;
    return b << 16 | g << 8 | r;
}

std::vector<char> title_screen_data;

void init_titlescreen() {
    auto *img = RPG::title->backgroundImage;
    int palette_bgr[256] = {
        0x9C6000, 0x9B6100, 0x9E6101, 0x9E6302, 0xA26405, 0xA3650C, 0x9F640D, 0x9B6312, 0xA46612, 0xA56913, 0xA86914,
        0xA36A1A, 0x9F661B, 0xA3671C, 0xA86B1C, 0x9C651F, 0x9D681F, 0xA46B20, 0x9E6721, 0x9E6921, 0xA96E22, 0x9F6823,
        0xA46A24, 0xAA6E25, 0xAB7126, 0xA06A28, 0x9D692A, 0xA36A2A, 0xA86C2A, 0xA86F2A, 0xAC6F2A, 0xA26D2B, 0xAC722B,
        0xA56F2C, 0xAF732C, 0xAE7332, 0xA97233, 0xA67137, 0xB37837, 0xA97438, 0xA47239, 0xB17639, 0xB07939, 0xAC753B,
        0xAC783B, 0xA1723C, 0xAD7A3C, 0xA6763D, 0x9D713E, 0xA9753E, 0xB37B41, 0xB37F41, 0xB07C42, 0xB78042, 0xAC7A43,
        0xA47849, 0xA97A49, 0xB07E49, 0xAC7C4B, 0xB4814C, 0xB8834D, 0x99754F, 0xB0804F, 0xAD8050, 0xA97C51, 0xA47C54,
        0xB18554, 0xB08156, 0xA48057, 0xB48558, 0xB78658, 0xBE8D58, 0xAA8259, 0xAC8259, 0xB98B59, 0xB68A5A, 0xB1865B,
        0x977A5E, 0xA38260, 0xAC8460, 0xB68B61, 0xBC8F61, 0xB18864, 0xAC8967, 0x9D8469, 0xA68769, 0xA98969, 0xB68C69,
        0xBA9069, 0xBF9369, 0xB18D6A, 0xB6906B, 0xC69B6E, 0xBF9970, 0xBC9371, 0xAD8D73, 0xB69273, 0xA98E74, 0xAA8E74,
        0xA88E75, 0xAE9175, 0xBA9975, 0xC69E76, 0x9C8977, 0xA48D77, 0xBB9577, 0xBF9A77, 0xAB927A, 0xAE927B, 0xAE947B,
        0xBA9A7C, 0xC39E7C, 0xB5957D, 0xBF9B7D, 0xC9A27D, 0xB1967F, 0xB79A80, 0xAF9782, 0xBF9D83, 0xB29884, 0xB69A85,
        0xC0A285, 0xC4A285, 0xC8A585, 0xCCA785, 0xD3AB85, 0xB69A88, 0xB79D88, 0xAF9A8C, 0xB89E8C, 0xB69F8D, 0xBAA08D,
        0xC1A28D, 0xB59F8E, 0xC5A68E, 0xBDA18F, 0xC0A58F, 0xB8A090, 0xBBA291, 0xC6AA91, 0xCAAC92, 0xCDAE92, 0xB0A193,
        0xC0A693, 0xD1B093, 0xBBA596, 0xD3B396, 0xC0A797, 0xC7AE97, 0xC2AA98, 0xC5AB98, 0xBEAA99, 0xC0AB99, 0xDBB899,
        0xC2AD9B, 0xC6AF9B, 0xCDB19C, 0xD6B79C, 0xCAB09D, 0xCAB39E, 0xC0AD9F, 0xC4B09F, 0xD0B4A0, 0xC7B1A1, 0xCEB6A1,
        0xD1B8A1, 0xD3B9A2, 0xE4C3A2, 0xC1AFA3, 0xCBB5A3, 0xC3B3A5, 0xC7B5A5, 0xCBB7A7, 0xD2BAA7, 0xCEBAA9, 0xDAC0A9,
        0xCBBAAB, 0xD1BFAB, 0xE4C6AB, 0xC2B6AD, 0xC8B8AD, 0xD2BBAD, 0xCEBCAF, 0xD0BFAF, 0xD6C1AF, 0xCBBBB0, 0xDDC4B0,
        0xD9C3B1, 0xE8CDB1, 0xD1C0B2, 0xDCC6B5, 0xCEC0B6, 0xD3C1B6, 0xD7C4B6, 0xD8C7B6, 0xE3CDB6, 0xD3C4B7, 0xD7C5BA,
        0xD7C9BA, 0xDAC9BA, 0xDECABA, 0xDECDBA, 0xF1D6BA, 0xD2C5BE, 0xD7C8BE, 0xDAC9BE, 0xDACCBF, 0xDDCEC0, 0xF0D8C3,
        0xE0CFC4, 0xD7CAC5, 0xDDCEC5, 0xDDD2C6, 0xDACEC7, 0xE8D6C7, 0xE0D2C8, 0xE2D3C8, 0xE4D4C8, 0xE5D5C9, 0xF3DEC9,
        0xDED3CA, 0xE0D6CB, 0xE2D6CB, 0xE3D7CE, 0xFDECCE, 0xDED5CF, 0xFEF4D0, 0xE5D9D1, 0xE2D9D2, 0xE7DAD2, 0xE6DDD3,
        0xEDDFD4, 0xE7DED6, 0xEBE0D6, 0xE6DED8, 0xFAEFD8, 0xECE1D9, 0xE9E1DA, 0xECE4DA, 0xECE1DC, 0xEEE6DC, 0xF8EEDC,
        0xECE4DD, 0xEEE5DE, 0xEDE5E0, 0xEFE8E3, 0xF1E8E3, 0xFEF8E6, 0xF2EBE7, 0xF5EDE9, 0xF1EEE9, 0xF8F0EA, 0xF6F1EF,
        0xF9F6F1, 0xFBF7F5, 0xFFFFFF};

    img->setPalette(palette_bgr);
    img->applyPalette();

    std::ifstream file("Title/alwaysraw", std::ios::binary);
    title_screen_data = std::vector<char>(std::istreambuf_iterator<char>(file), {});

}

bool &showingLogo = (**reinterpret_cast<bool **>(0x4CDADC));
int frame_counter = 0;
bool title_init_complete = false;

void onFrame(RPG::Scene scene) {
    // std::chrono::steady_clock::time_point then = std::chrono::steady_clock::now();
    if (scene != RPG::Scene::Title) return;

    if (!title_init_complete and !showingLogo) {
        init_titlescreen();
        title_init_complete = true;
    }
    if (title_init_complete) {                     // no way to see if we are past the intro logo so i just count frames yolo
    } else { return; }

    auto *img = RPG::title->backgroundImage;
    memcpy(img->pixels, title_screen_data.data() + 320 * 240 * frame_counter, 320 * 240);
    frame_counter = (frame_counter + 1) % 482;
    // std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    // fmt::print("Cpy time = {:6}μs\n", std::chrono::duration_cast<std::chrono::microseconds>(now - then).count());
}

void onInitTitleScreen() {
    title_init_complete = false;
}

// usefull for measuring frametimes
// std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();

// void onDrawScreen() {
//     std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
//     fmt::print("DrawScreen Time Diff = {:6}μs\n", std::chrono::duration_cast<std::chrono::microseconds>(now -
//     last).count()); last = now;
// }

// void onFrame(RPG::Scene scene) {
//     std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
//     fmt::print("OnFrame Time Diff    = {:6}μs\n", std::chrono::duration_cast<std::chrono::microseconds>(now -
//     last).count()); last = now;
// }

bool onStartup(char *pluginName) {
    fmt::print("Always has been Plugin loaded.\n");

    return true;
}
