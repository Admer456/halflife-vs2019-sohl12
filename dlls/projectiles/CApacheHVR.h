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

#ifndef CAPACHEHVR_H
#define CAPACHEHVR_H

#ifndef WEAPONS_H
#include "weapons.h"
#endif

//=========================================================
// Class definition of CApacheHVR
//=========================================================
class CApacheHVR : public CGrenade
{
public:
	void Spawn() override;
	void Precache() override;
	void EXPORT IgniteThink();
	void EXPORT AccelerateThink();

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

private:
	int m_iTrail;
	Vector m_vecForward;
};

#endif // CAPACHEHVR_H