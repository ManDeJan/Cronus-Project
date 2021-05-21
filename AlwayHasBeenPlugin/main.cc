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
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ratio>
#include <string>
#include <string_view>
#include <thread>

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
        0x9C6000, 0x9D6200, 0x9F6200, 0xA16301, 0x9D6002, 0x9E6303, 0xA26405, 0xA26309, 0xA4660A, 0xA3660F, 0xA86810,
        0xA36715, 0xA86915, 0xA86A19, 0xA2681A, 0x9E681E, 0xA56A1E, 0xA96B1F, 0xAB6E20, 0x9F6821, 0xA16822, 0xA16825,
        0xA46B25, 0xA96B25, 0xAC6E25, 0xAA6F25, 0xAC7026, 0xAD7029, 0xA96C2A, 0xAF722A, 0xA26B2B, 0xA86F2B, 0xAF712D,
        0xB0742E, 0xA8702F, 0xAB702F, 0xAB7133, 0xAB7533, 0xA57034, 0xB17434, 0xB37836, 0xAB7437, 0xB1773A, 0xAA753B,
        0xAD763B, 0xA4713C, 0xAE7B3D, 0xB37B3E, 0xAE773F, 0xB68040, 0xAD7B41, 0xA87643, 0xAE7743, 0xA17444, 0xAC7B46,
        0xB27C46, 0xB78446, 0xB37F47, 0xB17D4B, 0xB3824B, 0xAC7D4C, 0xB2844E, 0xB8844E, 0xB27F4F, 0xAC7E50, 0xBA8951,
        0xA57C52, 0xB78453, 0xB18553, 0x997855, 0xAC7F55, 0xB18155, 0xB28557, 0xB68657, 0xBE8D58, 0xAD8459, 0xB78B59,
        0xB3885D, 0xB98A5E, 0xA48260, 0xAE8760, 0xB88F61, 0xC09261, 0xB28A62, 0xB88B63, 0xB08A67, 0xBB9069, 0xA0856A,
        0xB68F6A, 0xAA8A6B, 0xBA946B, 0xC0956B, 0xC59A6C, 0xB08E6D, 0xBE9671, 0xB49072, 0xAA8F74, 0xAC8F74, 0xB79574,
        0xA08B75, 0xAD9175, 0xBE9776, 0xBF9C76, 0xC59D76, 0xB09479, 0xCAA379, 0xAB907A, 0xAD947A, 0xB7977B, 0xBF9C7B,
        0xB0957D, 0xB2967D, 0xC19C7F, 0xB19681, 0xB79881, 0xB99C81, 0xAB9482, 0xB29882, 0xC4A282, 0xC29E83, 0xD1AA84,
        0xB39985, 0xB89D85, 0xC9A685, 0xB79986, 0xB79C88, 0xC3A688, 0xB39B89, 0xC1A189, 0xB99C8A, 0xBA9F8A, 0xC9A78C,
        0xBAA08D, 0xCDAD8D, 0xC0A28E, 0xC2A68E, 0xB79F8F, 0xBAA090, 0xAF9E91, 0xC2A791, 0xBDA292, 0xBCA692, 0xC8AA94,
        0xCEAE94, 0xC2A795, 0xC8AE95, 0xD4B595, 0xBDA596, 0xBEA896, 0xC3AA96, 0xC3AA99, 0xC8AE9A, 0xBFAA9B, 0xCAB19B,
        0xCEB29B, 0xB9A89C, 0xC2AB9C, 0xC1AE9C, 0xC4AE9D, 0xC7AF9E, 0xC8B29F, 0xCDB3A0, 0xD5B9A0, 0xDDBDA0, 0xC2AFA1,
        0xC7B0A2, 0xCFB8A2, 0xC8B4A3, 0xCAB5A7, 0xC9B7A7, 0xCCB8A7, 0xD0B8A7, 0xD2BBA7, 0xC3B3A9, 0xD1BCAB, 0xD9C0AB,
        0xC9B9AC, 0xCDBAAC, 0xD1C0AD, 0xD0BDAE, 0xD2BFB1, 0xD6C1B1, 0xD2C2B1, 0xCABBB2, 0xCEBFB3, 0xD6C4B3, 0xDCC7B3,
        0xD5C3B6, 0xCFC1B7, 0xD7C7B8, 0xDBC7B8, 0xEBD1B8, 0xD5C4B9, 0xD6C7BB, 0xDAC8BB, 0xD0C3BC, 0xDDCCBC, 0xF6DBBD,
        0xD9C9BE, 0xE2CFBF, 0xDACAC0, 0xDCCDC0, 0xD5C8C1, 0xDCCCC2, 0xDCCEC4, 0xE0CEC4, 0xDDD0C4, 0xE0D2C4, 0xDDD1C7,
        0xE1D2C7, 0xDACDC9, 0xE4D4C9, 0xE1D6CA, 0xE6D7CA, 0xDCD2CB, 0xE1D2CB, 0xE1D7CD, 0xEDDBCD, 0xE3D5CE, 0xE4D8CE,
        0xF6E3CF, 0xE4D8D0, 0xE6DBD0, 0xFFF3D0, 0xE6DBD3, 0xE8DCD3, 0xE3D8D4, 0xE7DCD6, 0xE9DFD6, 0xF9E8D6, 0xEAE2D8,
        0xE8DED9, 0xEDE2D9, 0xEBDFDA, 0xEBE2DC, 0xFFF5DC, 0xEEE3DD, 0xEDE6DD, 0xEDE6DF, 0xF0E7DF, 0xEDE6E2, 0xF0E7E3,
        0xF0EAE3, 0xF3EAE3, 0xF2EAE6, 0xF3EDE7, 0xFFF8E7, 0xF3ECEA, 0xF5EEEA, 0xF8F0EB, 0xF5EFED, 0xF7F1EF, 0xFAF5EF,
        0xF9F4F3, 0xFCF7F5, 0xFFFEF6};

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
    if (title_init_complete) { // no way to see if we are past the intro logo so i just count frames yolo
    } else {
        return;
    }

    auto *img = RPG::title->backgroundImage;
    memcpy(img->pixels, title_screen_data.data() + 320 * 240 * frame_counter, 320 * 240);
    frame_counter = (frame_counter + 1) % 482;
    // std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    // fmt::print("Cpy time = {:6}μs\n", std::chrono::duration_cast<std::chrono::microseconds>(now - then).count());
}

void onInitTitleScreen() { title_init_complete = false; }

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
