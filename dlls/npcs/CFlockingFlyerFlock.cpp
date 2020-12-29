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

//=========================================================
// Flyer - Inoffensive bird-like creature (Alien swarm)
//=========================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CFlockingFlyerFlock.h"

//=========================================================
// Save and Restore
//=========================================================
TYPEDESCRIPTION CFlockingFlyerFlock::m_SaveData[] =
{
	DEFINE_FIELD(CFlockingFlyerFlock, m_cFlockSize, FIELD_INTEGER),
	DEFINE_FIELD(CFlockingFlyerFlock, m_flFlockRadius, FIELD_FLOAT),
};

IMPLEMENT_SAVERESTORE(CFlockingFlyerFlock, CBaseMonster);

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(monster_flyer_flock, CFlockingFlyerFlock);

//=========================================================
// KeyValue
//=========================================================
void CFlockingFlyerFlock::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "iFlockSize"))
	{
		m_cFlockSize = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "flFlockRadius"))
	{
		m_flFlockRadius = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
}

//=========================================================
// Spawn
//=========================================================
void CFlockingFlyerFlock::Spawn()
{
	Precache();
	
	SpawnFlock();

	REMOVE_ENTITY(ENT(pev)); // dump the spawn ent
}

//=========================================================
// Precache
//=========================================================
void CFlockingFlyerFlock::Precache()
{
	PRECACHE_MODEL("models/boid.mdl");

	PrecacheFlockSounds();
}

void CFlockingFlyerFlock::PrecacheFlockSounds()
{
	PRECACHE_SOUND("boid/boid_alert1.wav");
	PRECACHE_SOUND("boid/boid_alert2.wav");

	PRECACHE_SOUND("boid/boid_idle1.wav");
	PRECACHE_SOUND("boid/boid_idle2.wav");
}

//=========================================================
// SpawnFlock
//=========================================================
void CFlockingFlyerFlock::SpawnFlock()
{
	float R = m_flFlockRadius;
	Vector vecSpot;
	CFlockingFlyer *pBoid;

	CFlockingFlyer* pLeader = pBoid = nullptr;

	for (int iCount = 0; iCount < m_cFlockSize; iCount++)
	{
		pBoid = GetClassPtr(static_cast<CFlockingFlyer*>(nullptr));

		if (!pLeader)
		{
			// make this guy the leader.
			pLeader = pBoid;

			pLeader->m_pSquadLeader = pLeader;
			pLeader->m_pSquadNext = nullptr;
		}

		vecSpot.x = RANDOM_FLOAT(-R, R);
		vecSpot.y = RANDOM_FLOAT(-R, R);
		vecSpot.z = RANDOM_FLOAT(0, 16);
		vecSpot = pev->origin + vecSpot;

		UTIL_SetOrigin(pBoid, vecSpot);
		
		pBoid->pev->movetype = MOVETYPE_FLY;
		pBoid->SpawnCommonCode();
		pBoid->pev->flags &= ~FL_ONGROUND;
		pBoid->pev->velocity = g_vecZero;
		pBoid->pev->angles = pev->angles;

		pBoid->pev->frame = 0;
		pBoid->SetNextThink(0.2);
		pBoid->SetThink(&CFlockingFlyer :: IdleThink);

		if (pBoid != pLeader)
		{
			pLeader->SquadAdd(pBoid);
		}
	}
}