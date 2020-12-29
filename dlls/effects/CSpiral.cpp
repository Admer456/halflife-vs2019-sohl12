/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CSpiral.h"

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(streak_spiral, CSpiral);

//=========================================================
// Spawn
//=========================================================
void CSpiral::Spawn()
{
	pev->movetype = MOVETYPE_NONE;
	
	SetNextThink(0);
	
	pev->solid = SOLID_NOT;
	
	UTIL_SetSize(pev, g_vecZero, g_vecZero);
	
	pev->effects |= EF_NODRAW;
	
	pev->angles = g_vecZero;
}

//=========================================================
// Create
//=========================================================
CSpiral* CSpiral::Create(const Vector& origin, float height, float radius, float duration)
{
	if (duration <= 0)
		return nullptr;

	CSpiral* pSpiral = GetClassPtr((CSpiral*)nullptr);
	pSpiral->Spawn();
	pSpiral->pev->dmgtime = pSpiral->m_fNextThink;
	pSpiral->pev->origin = origin;
	pSpiral->pev->scale = radius;
	pSpiral->pev->dmg = height;
	pSpiral->pev->speed = duration;
	pSpiral->pev->health = 0;
	pSpiral->pev->angles = g_vecZero;

	return pSpiral;
}

//=========================================================
// Think
//=========================================================
void CSpiral::Think()
{
	float time = gpGlobals->time - pev->dmgtime;

	while (time > SPIRAL_INTERVAL)
	{
		Vector position = pev->origin;
		Vector direction = Vector(0, 0, 1);

		float fraction = 1.0 / pev->speed;

		float radius = (pev->scale * pev->health) * fraction;

		position.z += (pev->health * pev->dmg) * fraction;
		pev->angles.y = (pev->health * 360 * 8) * fraction;
		UTIL_MakeVectors(pev->angles);
		position = position + gpGlobals->v_forward * radius;
		direction = (direction + gpGlobals->v_forward).Normalize();

		StreakSplash(position, Vector(0, 0, 1), RANDOM_LONG(8, 11), 20, RANDOM_LONG(50, 150), 400);

		// Jeez, how many counters should this take ? :)
		pev->dmgtime += SPIRAL_INTERVAL;
		pev->health += SPIRAL_INTERVAL;
		time -= SPIRAL_INTERVAL;
	}

	SetNextThink(0);

	if (pev->health >= pev->speed)
		UTIL_Remove(this);
}

//=========================================================
// StreakSplash
//=========================================================
void CSpiral::StreakSplash(const Vector& origin, const Vector& direction, int color, int count, int speed, int velocityRange)
{
	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, origin);
		WRITE_BYTE(TE_STREAK_SPLASH);
		WRITE_COORD(origin.x);		// origin
		WRITE_COORD(origin.y);
		WRITE_COORD(origin.z);
		WRITE_COORD(direction.x);	// direction
		WRITE_COORD(direction.y);
		WRITE_COORD(direction.z);
		WRITE_BYTE(color);	// Streak color 6
		WRITE_SHORT(count);	// count
		WRITE_SHORT(speed);
		WRITE_SHORT(velocityRange);	// Random velocity modifier
	MESSAGE_END();
}
