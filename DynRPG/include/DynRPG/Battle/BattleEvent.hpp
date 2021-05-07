#ifndef RPG_BATTLE_EVENT_HPP
#define RPG_BATTLE_EVENT_HPP

#include <DynRPG/Utility/Types.hpp>

namespace RPG
{
/*! \brief Used to access battle event parameters (NOT the scriptlines).

	\sa RPG::battleEvents
*/
struct BattleEvent
{
	// clang-format off
	void** vTable;
	StubPtr eventData; //!< Stub. Use RPG::getBattleEventLine(monsterGroup,battleEventPage,lineId) to access battle event data
		char _unknown_08;
		char _unknown_09;
		char _unknown_0A;
		char _unknown_0B;
		int _unknown_0C;
		char _unknown_10;
		char _unknown_11;
		char _unknown_12;
		int _unknown_14;
		char _unknown_18;
		char _unknown_19;
		int _unknown_1C;
		int _unknown_20;
		char _unknown_24;
		char _unknown_25;
		char _unknown_26;
		char _unknown_27;
		char _unknown_28;
		char _unknown_29;
		char _unknown_2A;
		char _unknown_2B; // unused?
	bool eventPageDone[100]; //!< Determines whether a particular battle event page has finished executing
	// clang-format on
};

/*! \ingroup game_objects
	\brief Battle event data.

	Example:
	\code
std::string temp = RPG::battleEvents->eventPageDone[0]);
	\endcode
*/
static BattleEvent*& battleEvents = (**reinterpret_cast<BattleEvent***>(0x4CDB10));
}

#endif // RPG_BATTLE_EVENT_HPP
