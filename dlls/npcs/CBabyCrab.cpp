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
#include "monsters.h"

#include "CBabyCrab.h"

extern Schedule_t slHCRangeAttack1Fast[]; //From CHeadCrab

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(monster_babycrab, CBabyCrab);

//=========================================================
// Spawn
//=========================================================
void CBabyCrab::Spawn()
{
	CHeadCrab::Spawn();
	
	if (pev->model)
		SET_MODEL(ENT(pev), STRING(pev->model)); //LRC
	else
		SET_MODEL(ENT(pev), "models/baby_headcrab.mdl");
	
	pev->rendermode = kRenderTransTexture;
	pev->renderamt = 192;
	
	UTIL_SetSize(pev, Vector(-12, -12, 0), Vector(12, 12, 24));

	pev->health = gSkillData.headcrabHealth * 0.25;	// less health than full grown
}

//=========================================================
// Precache
//=========================================================
void CBabyCrab::Precache()
{
	if (pev->model)
		PRECACHE_MODEL((char*)STRING(pev->model)); //LRC
	else
		PRECACHE_MODEL("models/baby_headcrab.mdl");
	
	CHeadCrab::Precache();
}

//=========================================================
// SetYawSpeed
//=========================================================
void CBabyCrab::SetYawSpeed()
{
	pev->yaw_speed = 120;
}

//=========================================================
// CheckRangeAttack1
//=========================================================
BOOL CBabyCrab::CheckRangeAttack1(float flDot, float flDist)
{
	if (pev->flags & FL_ONGROUND)
	{
		if (pev->groundentity && (pev->groundentity->v.flags & (FL_CLIENT | FL_MONSTER)))
			return TRUE;

		// A little less accurate, but jump from closer
		if (flDist <= 180 && flDot >= 0.55)
			return TRUE;
	}

	return FALSE;
}

//=========================================================
// GetScheduleOfType
//=========================================================
Schedule_t* CBabyCrab::GetScheduleOfType(int Type)
{
	switch (Type)
	{
		case SCHED_FAIL:	// If you fail, try to jump!
			if (m_hEnemy != nullptr)
				return slHCRangeAttack1Fast;
		break;
		case SCHED_RANGE_ATTACK1:
			return slHCRangeAttack1Fast;
	}

	return CHeadCrab::GetScheduleOfType(Type);
}
