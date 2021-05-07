#define DYNRPG_STATIC
#include <DynRPG/Actor/Actor.hpp>

#include <DynRPG/Actor/DBActor.hpp>

namespace RPG
{
/******************************************************************************
 *
 *****************************************************************************/
std::string Actor::getDegree()
{
	if (degree.s_str() == "\x01")
		return dbActors[id]->degree;
	return degree;
}

/******************************************************************************
 *
 *****************************************************************************/
std::string Actor::getCharsetFilename()
{
	if (charsetFilename.s_str() == "")
		return dbActors[id]->charsetFilename;
	return charsetFilename;
}

/******************************************************************************
 *
 *****************************************************************************/
int Actor::getCharsetId()
{
	if (charsetFilename.s_str() == "")
		return dbActors[id]->charsetId;
	return charsetId;
}

/******************************************************************************
 *
 *****************************************************************************/
int Actor::getCharsetTransparency()
{
	if (charsetFilename.s_str() == "")
		return dbActors[id]->charsetTransparent ? 3 : 0;
	return charsetTransparency;
}

/******************************************************************************
 *
 *****************************************************************************/
std::string Actor::getFacesetFilename()
{
	if (facesetFilename.s_str() == "")
		return dbActors[id]->facesetFilename;
	return facesetFilename;
}

/******************************************************************************
 *
 *****************************************************************************/
int Actor::getFacesetId()
{
	if (facesetFilename.s_str() == "")
		return dbActors[id]->facesetId;
	return facesetId;
}

/******************************************************************************
 *
 *****************************************************************************/
int Actor::getBattleGraphicId()
{
	if (battleGraphicId == 0)
		return dbActors[id]->battleGraphicId;
	return battleGraphicId;
}

/******************************************************************************
 *
 *****************************************************************************/
int Actor::getBattleCommand(int index)
{
	if (!customBattleCommands)
		return dbActors[id]->battleCommands[index];
	return battleCommands[index];
}

/******************************************************************************
 *
 *****************************************************************************/
Actor* Actor::partyMember(int index)
{
	Actor* ret = nullptr;
	asm volatile("call *%%esi"
				 : "=a"(ret), "=d"(_edx)
				 : "S"(0x4A6014), "a"(**(void***)0x4CDB74), "d"(index)
				 : "ecx", "cc", "memory"); // GetPartyMember
	return ret;
}

/******************************************************************************
 * TODO: Should this be moved?
 *****************************************************************************/
AnimationInBattle*& Actor::animData = (**reinterpret_cast<AnimationInBattle***>(0x4CDDC8));

/******************************************************************************
 *
 *****************************************************************************/
void Actor::addSkill(int skillId)
{
	asm volatile("call *%%esi"
				 :
				 : "S"(0x4B7884), "a"(this), "d"(skillId)
				 : "ecx", "cc", "memory");
}

void Actor::removeSkill(int skillId)
{
	asm volatile("call *%%esi"
				 :
				 : "S"(0x4B7928), "a"(this), "d"(skillId)
				 : "ecx", "cc", "memory");
}

/******************************************************************************
 *
 *****************************************************************************/
bool Actor::isSkillKnown(int skillId)
{
	bool out;
	asm volatile("call *%%esi"
				 : "=a"(out)
				 : "S"(0x4B798C), "a"(this), "d"(skillId)
				 : "ecx", "cc", "memory");
	return out;
}

/******************************************************************************
 *
 *****************************************************************************/
int Actor::getTwoWeaponAttack()
{
	int out;
	asm volatile("call *%%esi"
				 : "=a"(out)
				 : "S"(0x4B7250), "a"(this)
				 : "ecx", "edx", "cc", "memory");
	return out;
}

/******************************************************************************
 *
 *****************************************************************************/
int Actor::getTwoWeaponAgility()
{
	int out;
	asm volatile("call *%%esi"
				 : "=a"(out)
				 : "S"(0x4B760C), "a"(this)
				 : "ecx", "edx", "cc", "memory");
	return out;
}

/******************************************************************************
 * Doesn't work yet
 *****************************************************************************/
/*void Actor::changeClass(RPG::DBClass *toClass)
{
	bool int1 = 1;
	bool int2 = 1;
	bool int3 = 1;
	bool int4 = 1;
	bool int5 = 1;
	int toClass2 = 5;
	asm volatile("push %%eax" : : "a" (int3));
	asm volatile("push %%eax" : : "a" (int2));
	asm volatile("push %%eax" : : "a" (int1));
	asm volatile("call *%%esi"
		: "=a" (_eax), "=d" (_edx), "=c" (_ecx)
		: "S" (0x4B11B8), "a" (this), "d" (toClass2), "c" (int4)
		: "cc", "memory");
}*/
}
