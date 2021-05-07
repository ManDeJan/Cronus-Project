#ifndef RPG_COMMON_EVENT_HPP
#define RPG_COMMON_EVENT_HPP

#include <DynRPG/Catalog/NamedCatalogPtr.hpp>
#include <DynRPG/CommonEvent/CommonEventTrigger.hpp>
#include <DynRPG/Delphi/DStringPtr.hpp>
#include <DynRPG/Event/EventScriptList.hpp>
#include <DynRPG/Utility/Types.hpp>

namespace RPG
{
/*! \brief Access to the common events.
	\sa RPG::commonEvents
	\sa RPG::CommonEventTrigger
*/
struct CommonEvent
{
	// clang-format off
	void** vTable;
	int id; //!< The ID of the common event
	DStringPtr name; //!< The name of the common event
		int _unknown_0C;
		int _unknown_10;
	EventScriptList* scriptLines; //!< The event script lines
	CommonEventTrigger trigger; //!< The trigger condition for the common event
	bool switchActivated; //!< Is the common event activated via switch?
	int switchId; //!< The switch ID if activated via switch
	// clang-format on
};

/*! \ingroup game_objects
	\brief Access to the common events.
*/
static NamedCatalogPtr<CommonEvent*>& commonEvents = (**reinterpret_cast<NamedCatalogPtr<CommonEvent*>**>(0x4CDF40));

/*! \brief Easily returns a common event line.

	Allows you to write <tt>RPG::getCommonEventLine(12,1)->command</tt> instead of <tt>RPG::commonEvents[12]->scriptLines->list->items[1]->command</tt> which gets the 2nd line of common event 12.

	\param id The common event ID (one-based)
	\param lineId The line ID (zero-based)

	\sa RPG::CommonEvent
*/
EventScriptLine* getCommonEventLine(int eventId, int lineId);
}

#endif // RPG_COMMON_EVENT_HPP