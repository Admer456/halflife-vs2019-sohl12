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
#include "weapons.h"
#include "squadmonster.h"

#include "npcs/CHGrunt.h"

#include "CDeadHGrunt.h"

const char* CDeadHGrunt::m_szPoses[] = {
	"deadstomach",
	"deadside",
	"deadsitting"
};

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(monster_hgrunt_dead, CDeadHGrunt);

//=========================================================
// KeyValue
//=========================================================
void CDeadHGrunt::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "pose"))
	{
		m_iPose = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseMonster::KeyValue(pkvd);
}

//=========================================================
// Spawn
//=========================================================
void CDeadHGrunt::Spawn()
{
	Precache();

	if (pev->model)
		SET_MODEL(ENT(pev), STRING(pev->model)); //LRC
	else
		SET_MODEL(ENT(pev), "models/hgrunt.mdl");

	pev->effects = 0;
	pev->yaw_speed = 8;
	pev->sequence = 0;
	m_bloodColor = BLOOD_COLOR_RED;

	pev->sequence = LookupSequence(m_szPoses[m_iPose]);
	if (pev->sequence == -1)
	{
		ALERT(at_debug, "Dead hgrunt with bad pose\n");
	}

	// Corpses have less health
	pev->health = 8;

	const int oldBody = pev->body;
	pev->body = 0;

	if (oldBody >= 5 && oldBody <= 7)
		pev->skin = 1;
	else
		pev->skin = 0;

	switch (pev->weapons)
	{
	case 0: // MP5
		SetBodygroup(GUN_GROUP, GUN_MP5);
		break;
	case 1: // Shotgun
		SetBodygroup(GUN_GROUP, GUN_SHOTGUN);
		break;
	case 2: // No gun
		SetBodygroup(GUN_GROUP, GUN_NONE);
		break;
	}

	switch (oldBody)
	{
	case 2: // Gasmask, no gun
		SetBodygroup(GUN_GROUP, GUN_NONE); //fall through
	case 0:
	case 6: // Gasmask (white/black)
		SetBodygroup(HEAD_GROUP, HEAD_GRUNT);
		break;
	case 3: // Commander, no gun
		SetBodygroup(GUN_GROUP, GUN_NONE); //fall through
	case 1: // Commander
		SetBodygroup(HEAD_GROUP, HEAD_COMMANDER);
		break;
	case 4:
	case 7: // Skimask (white/black)
		SetBodygroup(HEAD_GROUP, HEAD_SHOTGUN);
		break;
	case 5: // Commander
		SetBodygroup(HEAD_GROUP, HEAD_M203);
		break;
	}

	MonsterInitDead();
}

//=========================================================
// Precache
//=========================================================
void CDeadHGrunt::Precache()
{
	if (pev->model)
		PRECACHE_MODEL((char*)STRING(pev->model)); //LRC
	else
		PRECACHE_MODEL("models/hgrunt.mdl");
}