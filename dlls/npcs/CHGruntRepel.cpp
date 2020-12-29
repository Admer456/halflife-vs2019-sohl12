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
#include "effects.h"

#include "customentity.h"

#include "CHGruntRepel.h"

 //=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(monster_grunt_repel, CHGruntRepel);

//=========================================================
// Spawn
//=========================================================
void CHGruntRepel::Spawn()
{
	Precache();
	pev->solid = SOLID_NOT;

	SetUse(&CHGruntRepel::RepelUse);
}

//=========================================================
// Precache
//=========================================================
void CHGruntRepel::Precache()
{
	UTIL_PrecacheOther("monster_human_grunt");

	m_iSpriteTexture = PRECACHE_MODEL("sprites/rope.spr");
}

//=========================================================
// RepelUse
//=========================================================
void CHGruntRepel::RepelUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	TraceResult tr;
	UTIL_TraceLine(pev->origin, pev->origin + Vector(0, 0, -4096.0), dont_ignore_monsters, ENT(pev), &tr);

	CBaseEntity* pEntity = Create("monster_human_grunt", pev->origin, pev->angles);
	CBaseMonster* pGrunt = pEntity->MyMonsterPointer();
	pGrunt->pev->movetype = MOVETYPE_FLY;
	pGrunt->pev->velocity = Vector(0, 0, RANDOM_FLOAT(-196, -128));
	pGrunt->SetActivity(ACT_GLIDE);
	// UNDONE: position?
	pGrunt->m_vecLastPosition = tr.vecEndPos;

	CBeam* pBeam = CBeam::BeamCreate("sprites/rope.spr", 10);
	pBeam->PointEntInit(pev->origin + Vector(0, 0, 112), pGrunt->entindex());
	pBeam->SetFlags(BEAM_FSOLID);
	pBeam->SetColor(255, 255, 255);
	pBeam->SetThink(&CBeam::SUB_Remove);
	pBeam->SetNextThink(-4096.0 * tr.flFraction / pGrunt->pev->velocity.z + 0.5);

	UTIL_Remove(this);
}