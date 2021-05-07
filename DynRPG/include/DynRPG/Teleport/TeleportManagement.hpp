#ifndef RPG_TELEPORT_MANAGEMENT_HPP
#define RPG_TELEPORT_MANAGEMENT_HPP

namespace RPG
{
/*! \brief Same as the "Teleport Target Management" event command.
	\sa RPG::teleportManagement
	\sa RPG::teleport
	\sa RPG::Map::teleportHero()
*/
struct TeleportManagement
{
	// clang-format off
	void** vTable;
	int fromMapId = 0; //!< The current Map ID
		int _unknown_08;
	int toMapId; //!< The Map ID to teleport to
	int mapX; //!< The Map's X-Coordinate
	int mapY; //!< The Map's Y-Coordinate
	bool switchFlag; //!< Turns on a switch AFTER teleporting
	/*
		char _unknown_19;
		char _unknown_1A;
		char _unknown_1B;
	*/
	int switchId; //!< The ID of the switch to be turned on
	// clang-format on
};

/*! \ingroup game_objects
	\brief Teleport Management. It's the same as the "Teleport Target Management" event command.
*/
static TeleportManagement*& teleportManagement = (**reinterpret_cast<TeleportManagement***>(0x4CDB50));
}

#endif // RPG_TELEPORT_MANAGEMENT_HPP