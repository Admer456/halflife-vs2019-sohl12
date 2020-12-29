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

#ifndef CFLOCKINGFLYERFLOCK_H
#define CFLOCKINGFLYERFLOCK_H

#ifndef CFLOCKINGFLYER_H
#include "npcs/CFlockingFlyer.h"
#endif

//=========================================================
// Class definition of CFlockingFlyerFlock
//=========================================================

class CFlockingFlyerFlock : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void KeyValue(KeyValueData* pkvd) override;
	void SpawnFlock();

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	// Sounds are shared by the flock
	static void PrecacheFlockSounds();

	int m_cFlockSize = 8; //from game definition file
	float m_flFlockRadius = 128; //from game definition file
};

#endif // CFLOCKINGFLYERFLOCK_H