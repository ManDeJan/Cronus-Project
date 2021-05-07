namespace RPG {
	/*! \brief Used for Window objects of all kinds.

		\sa RPG::SceneMenu
		\sa RPG::SceneShop
		\sa RPG::SceneTitle
		\sa RPG::SceneDebug
	*/
	class Window {
		public:
			void **vTable;
			/*! \brief The number of choices per row
				\note The Items window has 2.
			*/
			int choicesPerRow;
			Image *image; //!< The actual image of the window after it's been drawn.
			Image *imageFrame; //!< The window's frame & background (if redrawn).
			Image *imageText; //!< The window's text (if redrawn), but can only draw underneath.
			int x; //!< The window's x-position
			int y; //!< The window's y-position
			int width; //!< The window's width
			int height; //!< The window's height
			int jumpX; //!< Change in x-position when moving cursor
			int jumpY; //!< Change in y-position when moving cursor
			int rowsPerPage; //!< Number of text rows on the window (choices or plain text)
			int cursorX; //!< The cursor's starting x-position (upper-left choice)
			int cursorY; //!< The cursor's starting y-position (upper-left choice)
			int cursorWidth; //!< The width of the cursor
			int cursorHeight; //!< The height of the cursor
			int totalChoices; //!< The total number of choices in the window
			int currentChoice; //!< The current choice selected (zero-based)
			int currentScroll; //!< Amount of choices above current choice
			int scrollTimer; //!< The timer between cursor movements (-12 or 12, minus or plus 4 each frame until it's 0)
			bool choiceVisible; //!< Has the choice bar been drawn? (It can still be considered visible if another window is active)
			bool choiceActive; //!< Is the choice bar of the window active?
			int choiceTimer; //!< The number of frames the choice has been selected
			// bugmenot's notes:
			// Can't say what the deal with the Roll-On is. It takes on values from 0..8 and somehow
			// reflects the progress of an inflateRect() process.
			int rollOn1;
			int rollOn2;
			int winTimer; //!< The total number of frames the window has been visible before a RPG::Scene change
			DListPtr<char> *text; //!< The window's text (doesn't work yet)
				int _unknown_68;
			int fontSet;  //!< The window's font set?
				int _unknown_70;
				int _unknown_74;

			// Functions
			Window(); //!< Experimental
			~Window(); //!< Experimental

			/*! \brief Experimental
				\param window The window to be created
				\param width The width of the window
				\param height The height of the window
				\param x The X-coordinate of the window, based on the upper-left
				\param y The Y-coordinate of the window, based on the upper-left
				\param startHidden Start the window hidden?
			*/
			void create(int width, int height, int x, int y, bool startHidden);

			void redraw(); //!< Experimental

			void updateCursor(int choice); //!< Experimental

			/*! \brief Clears the specified window of any text, so that it can be drawn on again.
				\param window The window to be cleared
			*/
			void clear(); //!< Experimental

			void refresh(); //!< Experimental

			/*! \brief Removes the specified window object and all of its components
				\param window The window to be cleared
			*/
			void destroy(); //!< Experimental

			void drawString(int x, int y, std::string text, int color, bool initialize);

			/*! \brief  Gets the index of a selection within a RPG::Window object (Example: int selected = RPG::battleData->winMonTarget->getSelected())
				\return the zero-based index of the selection, or -1 if the RPG::Window object is not active
			*/
			int getSelected();
	};

	RPG::Window::Window() {
		int n = 1;
		asm volatile("call *%%esi" : : "S" (0x4C6330), "a" (0x4C5F30), "d" (n) : "ecx",  "cc", "memory");
	}

	void RPG::Window::create(int width, int height, int x, int y, bool startHidden) {
		asm volatile("push %%eax" : : "a" (width));
		asm volatile("push %%eax" : : "a" (height));
		asm volatile("push %%eax" : : "a" (startHidden));
		asm volatile("call *%%esi" : : "S" (0x4C63DC), "a" (this), "d" (x), "c" (y) : "cc", "memory"); // cc = condition codes
	}

	void RPG::Window::clear(){
		asm volatile("call *%%esi" : : "S" (0x4C63A0), "a" (this) : "edx", "ecx", "cc", "memory");
	}

	void RPG::Window::redraw(){
		asm volatile("call *%%esi" : : "S" (0x4C6CF8), "a" (this), "d" (width), "c" (height) : "cc", "memory");
	}

	void RPG::Window::updateCursor(int choice){
		asm volatile("call *%%esi" : : "S" (0x4C6810), "a" (this), "d" (choice) : "ecx", "cc", "memory");
	}

	void RPG::Window::refresh(){
		asm volatile("call *%%esi" : : "S" (0x4C6640), "a" (this) : "edx", "ecx", "cc", "memory");
	}

	void RPG::Window::destroy(){ // destroys the window and all of its members
		asm volatile("call *%%esi" : : "S" (0x4C66E4), "a" (this) : "edx", "ecx", "cc", "memory");
	}

	RPG::Window::~Window() {
		int n = 1;
		asm volatile("call *%%esi" : : "S" (0x40376), "a" (this) : "edx", "ecx", "cc", "memory");
	}



	int RPG::Window::getSelected() {
		if (this->choiceActive) {
			return this->currentChoice;
		} else return -1;
	}

	/*! \brief Used for message windows.
		\sa RPG::WindowMessage
	*/
	class WindowMessageBox {
		public:
			void **vTable;
				int _unknown_08;
				int _unknown_0C;
				int _unknown_10;
			DStringPtr *txt;
			int count;
				int _unknown_1C; // total lines?? (always seems to be 4)

			std::string getLine(int line);
			void setLine(int line, std::string text);
	};

	std::string RPG::WindowMessageBox::getLine(int line) {
		return txt[line*2].s_str();
	}
	void RPG::WindowMessageBox::setLine(int line, std::string text) {
		txt[line*2] = text;
	}

	/*! \brief Used for message windows.
		\sa RPG::SceneMenu
	*/
	class WindowMessage : public Window {
		public:
			Window *winGold; //!< Pointer to gold window
				//int _unknown_7C; // Message box text... not yet implemented // Doesn't crash: DList<DStringPtr > *text
			WindowMessageBox *text;
			int currentTextRow; //!< The current row of the text being drawn (1-4)
			int currentTextColumn; //!< The current column of the text being drawn (1-??)
			int currentTextX; //!< The x-coordinate of the text being drawn, relative to where the text started drawing
			MessagePosition messageBoxPosition; //!< The position of the message box
			int currentTextColor; //!< The color of the text currently being drawn
			int currentTextSpeed; //!< The speed of the text currently being drawn
			int textTimer; //!< Internally used to time the drawing of a single text character
			bool isWaiting; //!< Is the text box waiting? (Slash dot command maybe?)
			bool autoEnter; //!< Was the slash carrot command used?
			int cancelChoice; //!< The cancel choice option (1-4, or 5 if execute handler)
				int _unknown_90; // Pointer on choice case event code?
			int inputNumValue; //!< Current input total
			int inputNumDigits; //!< Number of input digits
			int inputNumVariableId; //!< The destination variable for the input
			bool innIsActive; //!< Is the inn active?
			int innCost; //!< The inn's cost
			bool messageOpen; //!< Is a message box currently drawn?

			WindowMessage(); //!< Experimental
			~WindowMessage(); //!< Experimental
	};

	static RPG::WindowMessage *&winMessage = (**reinterpret_cast<RPG::WindowMessage ***>(0x4CDEF4));

	RPG::WindowMessage::WindowMessage() {
		int n = 1;
		asm volatile("call *%%esi" : : "S" (0x4C712C), "a" (0x4C605C), "d" (n) : "cc", "memory"); // welp
	}

	RPG::WindowMessage::~WindowMessage() {
		int n = 1;
		asm volatile("call *%%esi" : : "S" (0x40376), "a" (this) : "cc", "memory");
	}

	/*! \brief Used for skill/item target sub-menus (when a skill/item is used on a party member)

		\sa RPG::SceneMenu
	*/
	class WindowMenuTarget : public Window {
		public:
			Window *winItemName; //!< The sub-window for the item name in the upper-left
			Window *winNumUses; //!< The sub-window for the amount owned/mp cost below the item name window
	};

	/*! \brief Used for item menu from the main menu.

		\sa RPG::SceneMenu
	*/
	class WindowMenuItem : public Window {
		public:
			int heroId; //!< Database ID of the hero selected
			Window *winInfo; //!< The sub-window for the item description
	};

	/*! \brief Used for skill menu from the main menu.

		\sa RPG::SceneMenu
	*/
	class WindowMenuSkill : public Window {
		public:
			int heroId; //!< Database ID of the hero selected
			Window *winInfo; //!< The sub-window for the skill description
			Window *winHeroInfo; //!< The sub-window for the selected hero information
				int _unknown_84;
			DList<int> *skillSubsets; //!< Zero-based Array of skill subsets. Unsure about this though...
			bool isSkillSubset; //!< Is the selected skill a subset?
			//Window *winHeroMp; // ????

			void refreshSkills();
	};

	void RPG::WindowMenuSkill::refreshSkills(){
		asm volatile("call *%%esi" : : "S" (0x4C9274), "a" (this) : "edx", "ecx", "cc", "memory");
	}

	/*! \brief Used for equip menu from the main menu.

		\sa RPG::SceneMenu
	*/
	class WindowMenuEquip : public Window {
		public:
			int heroId; //!< Database ID of the hero selected
			Window *winInfo; //!< The sub-window for the equipment's description
			Window *winLeft; //!< The sub-window for the Selected hero's name and their stats
			Window *winRight; //!< The sub-window for the equipment slots and the currently equipped items
	};

	/*! \brief Used for quit menu from the main menu.

		\sa RPG::SceneMenu
	*/
	class WindowMenuEnd : public Window {
		public:
			Window *winQuitMsg; //!< The sub-window for the "Are you sure" message
	};

	/*! \brief Used for quit menu from the main menu.

		\sa RPG::SceneMenu
	*/
	class WindowMenuStatus : public Window {
		public:
			int heroId; //!< Database ID of the hero selected
			Window *winHpMpExp; //!< The sub-window for HP/MP/Exp (Upper-right)
			Window *winMain; //!< The sub-window for  the hero's name/class/title/condition/level
			Window *winEquipment; //!< The sub-window for the quipment
	};

	/*! \brief Used for quit menu from the main menu.

		\sa RPG::SceneMenu
	*/
	class WindowMenuOrder : public Window {
		public:
			Window *winNewConfig; //!< The sub-window for the new configuration
			Window *winConfirm; //!< The sub-window for the new config confirmation
	};

	/*! \brief Used for individual save files within the Save/load menu.

		\sa RPG::SceneMenu
	*/
	class WindowSaveFile : public Window {
		public:
			int saveSlotId; //!< The ID of the save slot currently selected
			Image *partyFaceImage[4]; //!< Array for the facesets of the 4 party members
				int _unknown_8C;
			int timeStamp; //!< The timestamp of the save?
	};

	/*Window() {
		asm volatile("push %%dl" : : "a" (1));
		asm volatile("push %%eax" : : "a" (0x4C5F30));
		asm volatile("call *%%esi" : : "S" (0x4C6330) : "cc", "memory");
	}*/
}
