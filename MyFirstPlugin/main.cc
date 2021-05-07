#include <DynRPG/DynRPG.hpp>

// Handler called on startup
bool onStartup(char *pluginName) {
    // don't mind this thanks
    // Do not remove or game won't launch.
    volatile short *magic_address = reinterpret_cast<volatile short *>(0x48FA57);
    *magic_address = 0x9090; 

    if (MessageBox(NULL, // We don't need a window handle
                   "This is a totally normal RPG Maker game, continue?",                 // Text
                   "He cometh.", // Title
                   MB_YESNO |
                       MB_ICONQUESTION // Flags (yes/no buttons and question icon)
                   ) == IDYES) {
        // The user clicked "Yes", we may continue
        return true;
    } else {
        // The user clicked "No", so we need to abort
        return false;
    }

}

// this doesn't work
// void onInitFinished() {
//     RPG::screen->toggleFullScreen(false);
// }
// void onFrame(RPG::Scene scene) {
//     RPG::screen->toggleFullScreen(false);
// }
