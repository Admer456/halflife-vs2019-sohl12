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

#ifndef CCONTROLLERZAPBALL_H
#define CCONTROLLERZAPBALL_H

#ifndef WEAPONS_H
#include "weapons.h"
#endif

//=========================================================
// Class definition of CControllerZapBall
//=========================================================
class CControllerZapBall : public CBaseMonster
{
public:
	static CControllerZapBall* Shoot(entvars_t* baseOwner, EHANDLE m_hEnemy, Vector vecStart, Vector vecVelocity);

private:
	void Spawn() override;
	void Precache() override;
	void EXPORT Animate();
	void EXPORT ExplodeTouch(CBaseEntity* pOther);

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	int m_maxFrame;
	EHANDLE m_hOwner;
};

#endif // CCONTROLLERZAPBALL_H
