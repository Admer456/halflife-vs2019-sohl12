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

#ifndef CSENTRY_H
#define CSENTRY_H

#ifndef CBASETURRET_H
#include "CBaseTurret.h"
#endif

//=========================================================
// Class definition of CSentry
//=========================================================
class CSentry : public CBaseTurret
{
public:
	void Spawn() override;
	void Precache() override;

	// other functions
	void Shoot(Vector& vecSrc, Vector& vecDirToEnemy) override;
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;

	void EXPORT SentryTouch(CBaseEntity* pOther);
	void EXPORT SentryDeath();
};

#endif // CSENTRY_H
