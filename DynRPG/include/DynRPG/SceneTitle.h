namespace RPG {
	//! Not implemented yet
	typedef void AuroraBoard;

	/*! \brief Access to the title screen parameters. 
		\sa RPG::title
	*/
	class SceneTitle {
		public:
			void **vTable;
				AuroraBoard *AuroraBoard1; //!< Not even a little bit implemented...
				AuroraBoard *AuroraBoard2; //!< Not even a little bit implemented...
			bool initialized; //!< Has the title screen initialized?
			bool isLoadNotDefault; //!< Set to false to make Load the default option;
			bool saveFilesPresent; //!< Are save files present (controls whether load can be selected). Determined DURING drawing of the title screen.
				bool _unknown_0F;
			Image *backgroundImage; //!< The title screen's background image. Can only be called after it's been drawn.
			Window *winStartLoadEnd; //!< The window object for Save/Load/End. Can only be called after it's been drawn.

	};

	/*! \ingroup game_objects
		\brief Access to the title screen parameters. 
		
		\warning Most of the parameters can't be used in onInitTitleScreen, because they haven't
			been created yet. Use onFrame instead to catch them when they have.
		
		Example:
		\code
if (RPG::title->winStartLoadEnd) { // Moves the title screen window to position 0,32.
	RPG::title->winStartLoadEnd->x = 0;
	RPG::title->winStartLoadEnd->y = 32;
}
		\endcode
	*/
	static RPG::SceneTitle *&title = (**reinterpret_cast<RPG::SceneTitle ***>(0x4CDB94));
}
