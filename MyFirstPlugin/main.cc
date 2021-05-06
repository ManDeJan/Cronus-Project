#define AUTO_DLLMAIN
#include <DynRPG/DynRPG.h>

// Handler called on startup
bool onStartup(char *pluginName) {
               if(MessageBox(
                              NULL, // We don't need a window handle
                              "This is such a haaaaard game. Are you SURE you want to play it now?", // Text
                              "The Are You Sure Plugin", // Title
                              MB_YESNO | MB_ICONQUESTION // Flags (yes/no buttons and question icon)
               ) == IDYES) {
                              // The user clicked "Yes", we may continue
                              return true;
               } else {
                              // The user clicked "No", so we need to abort
                              return false;
               }
}
