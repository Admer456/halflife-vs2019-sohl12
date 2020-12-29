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

#include "CTentacleMaw.h"

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(monster_tentaclemaw, CTentacleMaw);

//=========================================================
// Spawn
//=========================================================
void CTentacleMaw::Spawn()
{
	Precache();

	if (pev->model)
		SET_MODEL(ENT(pev), STRING(pev->model)); //LRC
	else
		SET_MODEL(ENT(pev), "models/maw.mdl");
	
	UTIL_SetSize(pev, Vector(-32, -32, 0), Vector(32, 32, 64));

	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_STEP;
	pev->effects = 0;
	pev->health = 75;
	pev->yaw_speed = 8;
	pev->sequence = 0;

	pev->angles.x = 90;
	// ResetSequenceInfo( );
}

//=========================================================
// Precache
//=========================================================
void CTentacleMaw::Precache()
{
	if (pev->model)
		PRECACHE_MODEL((char*)STRING(pev->model)); //LRC
	else
		PRECACHE_MODEL("models/maw.mdl");
}
