/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#ifndef CGENERICMONSTER_H
#define CGENERICMONSTER_H

// For holograms, make them not solid so the player can walk through them
//LRC- this seems to interfere with SF_MONSTER_CLIP
#define	SF_GENERICMONSTER_NOTSOLID					4
#define SF_GENERICMONSTER_PLAYERMODEL				 8
#define SF_GENERICMONSTER_INVULNERABLE				32
//Not implemented:
#define SF_GENERICMONSTER_CORPSE					64

//=========================================================
// Class definition of CGenericMonster
//=========================================================
class CGenericMonster : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	int Classify() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	int ISoundMask() override;
	void KeyValue(KeyValueData* pkvd) override;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	int HasCustomGibs() override
	{
		return m_iszGibModel;
	}

	int m_iszGibModel;
};

#endif // CGENERICMONSTER_H
