namespace RPG {
	/*! \brief Possible values for RPG::Condition::actionRestriction
	
		The type of action restriction as defined in the condition tab of the database.
	*/
	enum ActionRestriction {
		AR_NONE,
		AR_NO_ACTION_ALLOWED,
		AR_ATTACK_ENEMIES_RANDOMLY,
		AR_ATTACK_ALLIES_RANDOMLY
	};
	
	//! One-byte version or RPG::ActionRestriction
	typedef unsigned char ActionRestriction_T;
	
	/*! \brief Possible values for RPG::Condition::statAlteration
	
		The base stat alteration as defined for the condition
	*/
	enum BaseStatAlteration {
		BSA_HALF,
		BSA_DOUBLE,
		BSA_NONE
	};
	
	/*! \brief Possible values for RPG::Condition::hpAlterationType and RPG::Condition::mpAlterationType
	
		The alteration type for hp & mp as defined for the condition
	*/
	enum AlterationType {
		AT_ALTER_DAMAGE,
		AT_ALTER_RECOVER,
		AT_ALTER_NONE,
	};

	/*! \brief Used for the data of conditions which can be accessed or changed in-game.

		\sa RPG::conditions
		\sa RPG::ActionRestriction
	*/
	class Condition {
		public:
			void **vTable;
			int id; //!< ID of the attribute
			DStringPtr name; //!< The name of the attribute
			bool persistsAfterBattle; //!< Does the condition persist after battle?
			int color; //!< The condition's assigned color
			int priority; //!< The condition's priority value
			ActionRestriction_T actionRestriction; //!< The condition's action restriction (see RPG::ActionRestriction)
			int susA; //!< The A value of the condition
			int susB; //!< The B value of the condition
			int susC; //!< The C value of the condition
			int susD; //!< The D value of the condition
			int susE; //!< The E value of the condition
			int recoveryTurns; //!< Condition abates after this number of turns
			int recoveryTurnsPercent; //!< Percentage to recover after each turn
			int recoveryPhysicalPercent; //!< Percentage to abate after taking damage
			bool alterStr; //!< Does the condition alter Attack?
			bool alterDef; //!< Does the condition alter Defense? (3rd field - Defined incorrectly as "Intelligence" in RM2003 Advocate Translation)
			bool alterInt; //!< Does the condition alter Intelligence? (2nd field - Defined incorrectly as "Defense" in RM2003 Advocate Translation)
			bool alterAgi; //!< Does the condition alter Agility?
			int hitPercentage; //!< The condition's hit percentage
			bool preventSkillByAtk; //!< Prevent skills with a certain attack influence?
			int atkInfluence; //!< The attack influence value to prevent skills with
			bool preventSkillByInt; //!< Prevent skills with a certain intelligence influence?
			int intInfluence; //!< The attack influence value to prevent skills with
		// Can't believe these next 5 exist...
			DStringPtr msg2kAllyState; //!< "Ally State Message" in RM2000
			DStringPtr msg2kEnemyState; //!< "Enemy State Message" in RM2000
			DStringPtr msg2kAlreadyIn; //!< "Already in This State Message" in RM2000
			DStringPtr msg2kRegular; //!< "State is Regular for One Message" in RM2000
			DStringPtr msg2kRecovery; //!< "Recovery State Message" in RM2000
			int hpDamagePercentage; //!< "Damage per Turn in Battle" HP percent field
			int hpDamageUnits; //!< "Damage per Turn in Battle" plus HP units field
			int hpDamageStepNum; //!<  "Field Map" HP damage every number of steps field
			int hpDamageEachStep; //!< "Field Map" HP damage field
			int mpDamagePercentage; //!< "Damage per Turn in Battle" MP percent field
			int mpDamageUnits; //!< "Damage per Turn in Battle" plus MP units field
			int mpDamageStepNum; //!<  "Field Map" MP damage every number of steps field
			int mpDamageEachStep; //!< "Field Map" MP damage field
			BaseStatAlteration statAlteration; //!< Base statistic alteration (See RPG::BaseStatAlteration)
			bool evadeAttacks; //!< "Evade All Attacks" field
			bool reflectSkills; //!< "Reflect Skills" field
			bool lockEquipment; //!< "Equipment Cannot be Changed" field
			int poseId; //!< The Pose ID for the condition MINUS ONE
			AlterationType hpAlterationType; //!< HP alteration type (See RPG::AlterationType)
			AlterationType mpAlterationType; //!< MP alteration type (See RPG::AlterationType)
	};
	
	/*! \ingroup game_objects
		\brief Array of conditions from the database, used for default values and
		properties which are not supposed to be changed in-game.

		Use the database ID as index for accessing a condition.

		Example:
		\code
int condPercentage = RPG::conditions[9]->susC;
		\endcode
	*/
	static RPG::NamedCatalogPtr<RPG::Condition *> &conditions = (**reinterpret_cast<RPG::NamedCatalogPtr<RPG::Condition *> **>(0x4CDE84));
}
