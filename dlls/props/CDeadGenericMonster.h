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

#ifndef CDEADGENERICMONSTER_H
#define CDEADGENERICMONSTER_H

//=========================================================
// Class definition of CDeadGenericMonster
//=========================================================
class CDeadGenericMonster : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	int Classify() override
	{
		return CLASS_PLAYER_ALLY;
	}
	
	void KeyValue(KeyValueData* pkvd) override;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	int HasCustomGibs() override { return m_iszGibModel; }

	int m_iszGibModel;
};

#endif // CDEADGENERICMONSTER_H
