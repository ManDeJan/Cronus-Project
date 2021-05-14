#include "DynRPG/Character/AnimationType.hpp"
#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include "DynRPG/Map/Map.hpp"
#include <DynRPG/DynRPG.hpp>
#include <sapi.h>
#include <objbase.h>
#include <fmt/core.h>

#include <iostream>
#include <string>
#include <string_view> 

ISpVoice *pVoice = NULL;

bool global_speech_enabled = false;
bool local_speech_enabled = false;

bool female_voice = false;
std::string speed = "0";
std::string pitch = "0";

// Jank code, shhhh 😘
bool onComment(const char *text, const RPG::ParsedCommentData *parsedData, RPG::EventScriptLine *nextScriptLine, RPG::EventScriptData *scriptData, int eventId, int pageId, int lineId, int *nextLineId) {
    std::string_view comment = text;
    if (!(comment.starts_with("@tts(") and comment.ends_with(")"))) return true;
    fmt::print("Comment {}\n", comment);

    if (comment.find("@tts(global_off)") != std::string::npos) { fmt::print("tts disabled\n"); global_speech_enabled = false; return false; }
    if (comment.find("@tts(global_on)")  != std::string::npos) { fmt::print("tts enabled\n");  global_speech_enabled = true;  return false; }
    if (comment.find("@tts(off)")        != std::string::npos) { fmt::print("tts off\n");      local_speech_enabled = false;  return false; }
    else                                                       { fmt::print("tts on\n");       local_speech_enabled = true;                 }

    auto pos1 = comment.find_first_of(",", 5);
    if (pos1 == std::string::npos) {
        female_voice = false;
        return false;
    }
    female_voice = comment.substr(5, pos1 - 5).find("F") != std::string::npos; // logica f yeah
    fmt::print("Female command substr: {}, pos1: {}\n", comment.substr(5, pos1 - 5), pos1);
    auto pos2 = comment.find_first_of(",", pos1 + 1);
    fmt::print("Pos2: {}\n", pos2);
    if (pos2 == std::string::npos) {
        speed = "0";
        return false;
    }
    speed = comment.substr(pos1 + 1, pos2 - (pos1 + 1));

    auto pos3 = comment.find_first_of(")", pos2 + 1);
    fmt::print("Pos3: {}\n", pos3);
    if (pos3 == std::string::npos) {
        pitch = "0";
        return false;
    }

    pitch = comment.substr(pos2 + 1, pos3 - (pos2 + 1));
    fmt::print("Pos1: {} Pos 2: {} Pos3: {}\n", pos1, pos2, pos3);
    fmt::print("Female: {}\nSpeed: {}\nPitch: {}\n", female_voice, speed, pitch);
    return false;
}

bool onEventCommand(RPG::EventScriptLine *scriptLine, RPG::EventScriptData *scriptData, int eventId, int pageId, int lineId, int *nextLineId) {
    auto command_type = scriptLine->command;

    if (!(command_type == RPG::EventCommand::ShowMessage ||
          command_type == RPG::EventCommand::AddLineToMessage))
        return true;

    fmt::print("Msg: {}\n", std::string(scriptLine->stringParameter));

    if (!global_speech_enabled) return true;
    if (!local_speech_enabled)  return true;

    std::string speech_xml;
    speech_xml += std::string()
                + "<voice required=\"Gender=" + (female_voice ? "fe" : "") 
                + "male\"><rate absspeed=\"" + speed
                + "\"><pitch middle = '" + pitch + "'/>";
    speech_xml += scriptLine->stringParameter.s_str();
    fmt::print("XML:\n{}\n", speech_xml);
    pVoice->Speak(std::wstring(speech_xml.begin(), speech_xml.end()).c_str(), SPF_ASYNC | SPF_PURGEBEFORESPEAK | SPF_IS_XML, NULL );

    return true;
}

bool onStartup(char *pluginName)
{
    if (FAILED(::CoInitialize(NULL))) {
        fmt::print("TTS Plugin failed to load\n");
        return false;
    }

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
    if( SUCCEEDED( hr ) ) {
        fmt::print("TTS Plugin loaded.\n");

    }
    // ::CoUninitialize();
    return true;
}
