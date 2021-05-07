#ifndef RPG_WINDOW_SAVE_FILE_HPP
#define RPG_WINDOW_SAVE_FILE_HPP

#include <DynRPG/Graphics/Image.hpp>
#include <DynRPG/Window/Window.hpp>

namespace RPG
{
/*! \brief Used for individual save files within the Save/load menu.

	\sa RPG::SceneMenu
*/
struct WindowSaveFile : Window
{
	// clang-format off
	int saveSlotId; //!< The ID of the save slot currently selected
	Image* partyFaceImage[4]; //!< Array for the facesets of the 4 party members
		int _unknown_8C;
	int timeStamp; //!< The timestamp of the save?
	// clang-format on
};
}

#endif // RPG_WINDOW_SAVE_FILE_HPP
