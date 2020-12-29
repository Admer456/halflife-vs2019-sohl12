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

#include "CGenericMonster.h"

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(monster_generic, CGenericMonster);

//=========================================================
// Save and Restore
//=========================================================
TYPEDESCRIPTION CGenericMonster::m_SaveData[] =
{
	DEFINE_FIELD(CGenericMonster, m_iszGibModel, FIELD_STRING),
};

IMPLEMENT_SAVERESTORE(CGenericMonster, CBaseMonster);

//=========================================================
// KeyValue
//=========================================================
void CGenericMonster::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "m_bloodColor"))
	{
		m_bloodColor = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iszGibModel"))
	{
		m_iszGibModel = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseMonster::KeyValue(pkvd);
}

//=========================================================
// Classify
//=========================================================
int CGenericMonster::Classify()
{
	return m_iClass ? m_iClass : CLASS_PLAYER_ALLY;
}

//=========================================================
// SetYawSpeed
//=========================================================
void CGenericMonster::SetYawSpeed()
{
	int ys;

	switch (m_Activity)
	{
	case ACT_IDLE:
	default:
		ys = 90;
	}

	pev->yaw_speed = ys;
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CGenericMonster::HandleAnimEvent(MonsterEvent_t* pEvent)
{
	switch (pEvent->event)
	{
	case 0:
	default:
		CBaseMonster::HandleAnimEvent(pEvent);
		break;
	}
}

//=========================================================
// ISoundMask - generic monster can't hear.
//=========================================================
int CGenericMonster::ISoundMask()
{
	return NULL;
}

//=========================================================
// Spawn
//=========================================================
void CGenericMonster::Spawn()
{
	// store the size, so we can use it to set up the hulls after Set_Model overwrites it.
	Vector vecSize = pev->size;

	//LRC - if the level designer forgets to set a model, don't crash!
	if (FStringNull(pev->model))
	{
		if (pev->targetname)
			ALERT(at_error, "No model specified for monster_generic \"%s\"\n", STRING(pev->targetname));
		else
			ALERT(at_error, "No model specified for monster_generic at %.2f %.2f %.2f\n", pev->origin.x, pev->origin.y,
			      pev->origin.z);

		pev->model = MAKE_STRING("models/player.mdl");
	}

	Precache();

	SET_MODEL(ENT(pev), STRING(pev->model));

	if (vecSize != g_vecZero)
	{
		Vector vecMax = vecSize / 2;
		Vector vecMin = -vecMax;
		if (!FBitSet(pev->spawnflags, SF_GENERICMONSTER_PLAYERMODEL))
		{
			vecMin.z = 0;
			vecMax.z = vecSize.z;
		}
		UTIL_SetSize(pev, vecMin, vecMax);
	}
	else if (
		pev->spawnflags & SF_GENERICMONSTER_PLAYERMODEL ||
		FStrEq(STRING(pev->model), "models/player.mdl") ||
		FStrEq(STRING(pev->model), "models/holo.mdl")
	)
		UTIL_SetSize(pev, VEC_HULL_MIN, VEC_HULL_MAX);
	else
		UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;

	if (!m_bloodColor)
		m_bloodColor = BLOOD_COLOR_RED;

	if (!pev->health)
		pev->health = 8;

	m_flFieldOfView = 0.5; // indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState = MONSTERSTATE_NONE;

	MonsterInit();

	if (pev->spawnflags & SF_GENERICMONSTER_NOTSOLID)
	{
		pev->solid = SOLID_NOT;
		pev->takedamage = DAMAGE_NO;
	}
	else if (pev->spawnflags & SF_GENERICMONSTER_INVULNERABLE)
	{
		pev->takedamage = DAMAGE_NO;
	}
}

//=========================================================
// Precache
//=========================================================
void CGenericMonster::Precache()
{
	PRECACHE_MODEL((char*)STRING(pev->model));

	if (m_iszGibModel)
		PRECACHE_MODEL((char*)STRING(m_iszGibModel)); //LRC
}
