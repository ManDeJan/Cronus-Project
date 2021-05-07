#ifndef RPG_SCREEN_HPP
#define RPG_SCREEN_HPP

#include <DynRPG/Graphics/Canvas.hpp>
#include <DynRPG/Graphics/ScreenEffect.hpp>
#include <DynRPG/Scene/Scene.hpp>
#include <DynRPG/System/System.hpp>
#include <DynRPG/Utility/Win32.hpp>

namespace RPG
{
//! Not implemented yet
using AuroraDrawMain = void;

/*! \brief Used for the screen, including window properties and FPS
	\sa RPG::screen
*/
struct Screen
{
	// clang-format off
	// TAuroraDraw
	void** vTable;
	AuroraDrawMain* auroraDrawMain; //!< Not implemented yet
		int _unknown_8;
		int _unknown_C;
		int _unknown_10;
		int _unknown_14;
		int _unknown_18;
		int _unknown_1C;
		int _unknown_20;
		int _unknown_24;
		int _unknown_28;
		int _unknown_2C;
		int _unknown_30;
		int _unknown_34;
		int _unknown_38;
	bool fullScreen; //!< Is fullscreen active?
	bool largeWindow; //!< Is the large window (640x480) active?
	Canvas* canvas; //!< RPG::Canvas which should be used for drawing on the screen
	RECT canvasRect;
		int _unknown_54;

	// Cast these after including directx sdk in your project
	// IVideoWindow *videoContext = reinterpret_cast<IVideoWindow>(RPG::screen->IVideoWindow);
	void* FilgraphManager;
	void* IGraphBuilder;
	void* IVideoWindow;
	void* IMediaControl;
	void* IMediaEvent;
	void* IMediaSeeking;
	RECT movieRect;
		//int _unknown_7C;
		int _unknown_80;
		int _unknown_84;
		int _unknown_88;
		int _unknown_8C;
		int _unknown_90;
		int _unknown_94;
		int _unknown_98;
		int _unknown_9C;
		int _unknown_A0;
		int _unknown_A4;
		int _unknown_A8;
		int _unknown_AC;
		//int _unknown_B0;
	bool movieIsPlaying;
	bool isIdle; // movieIsIdle?
	bool wasScreenSaver;
		bool _unknown_B3; // stub. needed?
		int _unknown_B4;
		int _unknown_B8;
		int _unknown_BC;
	double millisecondsPerFrame; //!< Milliseconds a frame should take
	int maxFPS; //!< Maximum FPS
	// clang-format on

	/*! \brief Returns the HWND of the window control on which the
		screen is drawn
		\note In windowed mode, this is not the window itself, but a
		child of it.
	*/
	HWND getCanvasHWND();

	/*! \brief Changes the framerate (and thus the overall speed of the
		game)

		This function changes the speed of the whole game by changing
		the number of frames rendered per second. The default is \c 60.
		\param fps New framerate
	*/
	void setFPS(int fps);

	/*! \brief Redraws the screen for a certain scene

		This function completely redraws the screen and uses the
		drawing method of the specified scene. Normally, you would use
		the current scene for that. This function will also wait for
		the next frame without using 100% CPU, so that you can use this
		function to do something which takes longer than one frame.
		\param scene Scene to draw
	*/
	void update(Scene scene);

	/*! \brief Built-in RM2k3 function that changes between fullScreen and windowed mode
		\param fullScreen Change to windowed mode if false or change to fullScreen mode if true
	*/
	void toggleFullScreen(bool fullScreen);

	/*! \brief Built-in RM2k3 function that changes game window size mode
		\param largeWindow Change to small mode if false or change to large mode if true
	*/
	void toggleLargeWindow(bool largeWindow);

	/*! \ingroup game_objects
		\brief Pointer to the handler for various screen effects (WIP)
	*/
	static ScreenEffect*& effect;
};

/*! \ingroup game_objects
	\brief The screen, including window properties, FPS and the drawing
	canvas
*/
static Screen*& screen = (**reinterpret_cast<Screen***>(0x4CDB24));
}

#endif // RPG_SCREEN_HPP