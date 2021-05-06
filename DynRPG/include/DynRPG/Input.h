namespace RPG {
	/*! \brief %RPG Maker key names
		\note The \c Shift key is handled differently by the %RPG Maker, thus
		it can't be used the way other keys can be used by DynRPG.
		\sa RPG::Input
		\sa RPG::Input::pressed
		\note <b>Not all enumeration values are documented here</b>, scroll to the top
		to see all possible values (in the general enum overview)!
	*/
	enum Key {
		KEY_DOWN, //!< Keys which are used for moving down
		KEY_LEFT, //!< Keys which are used for moving left
		KEY_RIGHT, //!< Keys which are used for moving right
		KEY_UP, //!< Keys which are used for moving up
		KEY_DECISION, //!< Keys which are used to confirm an action 
		KEY_CANCEL, //!< Keys which are used to cancel an action or open the menu
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_ADD,
		KEY_SUBTRACT,
		KEY_MULTIPLY,
		KEY_DIVIDE,
		KEY_DECIMAL
	};
	
	/*! \brief Detectable keys to determine whether one is pressed, tapped, or held
		\sa RPG::input::isPressed
		\sa RPG::input::isHeld
		\sa RPG::input::isTapped
	*/
	enum KeyDown {
		KEYD_NONE,
		KEYD_DOWN,
		KEYD_LEFT,
		KEYD_RIGHT = 4,
		KEYD_UP = 8,
		KEYD_DECISION = 16,
		KEYD_CANCEL = 32,
		KEYD_MOUSE_LBUT = 64,
		KEYD_MOUSE_RBUT = 128
	};
	
	//! One-byte version of RPG::KeyDown
	typedef unsigned char KeyDown_T;
	
	/*! \brief Special keys to determine whether one is pressed, tapped, or held
		\sa RPG::input::isShiftCtrlPressed
		\sa RPG::input::isShiftCtrlHeld
		\sa RPG::input::isShiftCtrlTapped
	*/
	enum KeyDownAlt {
		KEYH_NONE,
		KEYH_SHIFT,
		KEYH_CTRL
	};
	
	//! One-byte version of RPG::KeyDownAlt
	typedef unsigned char KeyDownAlt_T;
	
	/*! \brief Used for key input
		\sa RPG::input
	*/
	class Input {
		public:
			void **vTable;
			int joystickId; //!< The ID of the first joystick
				char _unknown_08;
				char _unknown_09;
				char _unknown_0A;
				char _unknown_0B;
			/*! \brief Array of key codes assigned to the different RPG::Key
				values (part 1)
				
				This array contains the key assignments for those key functions
				which exist both in %RPG Maker 2000 and %RPG Maker 2003. Use
				the \ref key method for easy access.
				
				Indexes:
		
					  0-7: Down
					 8-15: Left
					16-23: Right
					24-31: Up
					32-39: Decision
					40-47: Cancel
				
				\sa keys2k3
				\sa RPG::Key
				\sa key
			*/
			int keys[48];
				int _unknown_CC[32];
			/*! \brief Array of key codes assigned to the different RPG::Key
				values (part 2)
				
				This array contains the key assignments for those key functions
				which exist only in %RPG Maker 2003. Use the \ref key method
				for easy access. Specifically these are the key assignments for
				numpad keys or otherwise that are detected via the 
				"Key Input Processing" event command
				
				Indexes:
		
					    0-7: 0 (Detected via Key Input Processing normally)
					   8-15: 1 "
					  16-23: 2 "
					  24-31: 3 "
					  32-39: 4 "
					  40-47: 5 "
					  48-55: 6 "
					  56-63: 7 "
					  64-71: 8 "
					  72-79: 9 "
					  80-87: + "
					  88-95: - "
					 96-103: * "
					104-111: / "
					112-119: . "
				
				\sa keys
				\sa RPG::Key
				\sa key
			*/
			int keys2k3[120];
			int resolutionTime; //!< In milliseconds
				int _unknown_330;
			int uTimerId; //!< 0 if timerEvent for input not existing
			KeyDown_T isPressed; //!< Are a combination of keys pressed & released (See RPG::KeyDown)
			KeyDownAlt_T isShiftCtrlPressed; //!< Are the Shift or Ctrl keys & released (See RPG::KeyDownAlt)
				char _unknown_33A;
				char _unknown_33B;
				int _unknown_33C;
			KeyDown_T isHeld; //!< Are a combination of keys held (See RPG::KeyDown)
			KeyDownAlt_T isShiftCtrlHeld; //!< Are the Shift or Ctrl keys held (See RPG::KeyDownAlt)
				char _unknown_342;
				char _unknown_343;
			KeyDown_T isTapped; //!< Are a combination of keys tapped (See RPG::KeyDown)
			KeyDownAlt_T isShiftCtrlTapped; //!< Are the Shift or Ctrl keys tapped (See RPG::KeyDownAlt)
				char _unknown_34A;
				char _unknown_34B;
			int joyInfo[4]; //!< JOYINFO button state?
				int _unknown_35C;
			int keyHoldTimer; //!< Amount of time a key is held?
			char leftRight;
				char _unknown_365;
				char _unknown_366;
				char _unknown_367;
			int directionNumpad; //!< down (2), left (4), right (6), up (8)
			bool joyRepsonseFlag; //!< If 1, response for button status of joy device took too long & don't process JoyInput
			
			/*! \brief Provides easy access to the key assignments
				
				This function returns a pointer to the "block" of 8 key codes
				in the \c keys or \c keys2k3 array which corresponds to the
				given RPG::Key value. The result should be used with the
				<tt>[]</tt> syntax with an index from \c 0 to \c 7.
				
				Example:
				\code
RPG::input->key(RPG::KEY_DECISION)[3] = VK_TAB;
				\endcode
				This example would assign the \c TAB key as a new decision key
				(the default decision keys are \c Enter, \c Space and \c Z,
				indexes \c 3 to \c 7 are not assigned by default).
				
				The values used for key assignments are
				<a href="http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx">virtual key codes</a>,
				zero meaning "not assigned".
				\param keyId RPG::Key for which the key codes should be read or
				changed
				\return Pointer to the first key code used for this RPG::Key
				\sa RPG::Key
			*/
			int *key(RPG::Key keyId);
			
			/*! \brief Checks whether a RPG::Key is pressed
				
				This function checks if one of the physical keys asiigned to a
				RPG::Key is currently held down. This function does not use the
				%RPG Maker's internal key cache.
				\param keyId RPG::Key to check
				\return \c true if the key is pressed, \c false otherwise
			*/
			bool pressed(RPG::Key keyId);
			
			/*! \brief Updates the %RPG Maker's internal key cache
				
				The %RPG Maker uses an internal key cache which is normally
				updated every frame. This function updates the key cache.
			*/
			void update();
	};
	
	/*! \ingroup game_objects
		\brief Object used for key assignments and key input checking.
	*/
	static RPG::Input *&input = (**reinterpret_cast<RPG::Input ***>(0x4CDAEC));
}
