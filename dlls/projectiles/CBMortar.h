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

#ifndef CBMORTAR_H
#define CBMORTAR_H

#ifndef SKILL_H
#include "skill.h"
#endif

#ifndef DECALS_H
#include "decals.h"
#endif

#ifndef WEAPONS_H
#include "weapons.h"
#endif

//=========================================================
// Class definition of CBMortar
//=========================================================
class CBMortar : public CBaseEntity
{
public:
	static CBMortar* Shoot(edict_t* pOwner, Vector vecStart, Vector vecVelocity);
	void MortarSpray(const Vector& position, const Vector& direction, int spriteModel, int count) const;
	
private:
	void Spawn() override;
	void Precache() override;
	
	void Touch(CBaseEntity* pOther) override;
	void EXPORT Animate();

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	int m_maxFrame = 0;
	int gSpitSprite;
};

#endif // CBMORTAR_H
