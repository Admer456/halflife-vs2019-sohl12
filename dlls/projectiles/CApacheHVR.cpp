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
#include "CApacheHVR.h"

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(hvr_rocket, CApacheHVR);

//=========================================================
// Save and Restore
//=========================================================
TYPEDESCRIPTION CApacheHVR::m_SaveData[] =
{
	DEFINE_FIELD(CApacheHVR, m_vecForward, FIELD_VECTOR),
};

IMPLEMENT_SAVERESTORE(CApacheHVR, CGrenade);

//=========================================================
// Spawn
//=========================================================
void CApacheHVR::Spawn()
{
	Precache();

	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_BBOX;

	if (pev->model)
		SET_MODEL(ENT(pev), STRING(pev->model)); //LRC
	else
		SET_MODEL(ENT(pev), "models/HVR.mdl");
	
	UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));
	UTIL_SetOrigin(this, pev->origin);

	SetThink(&CApacheHVR::IgniteThink);
	SetTouch(&CApacheHVR::ExplodeTouch);

	UTIL_MakeAimVectors(pev->angles);
	
	m_vecForward = gpGlobals->v_forward;
	pev->gravity = 0.5;

	SetNextThink(0.1);

	pev->dmg = 150;
}

//=========================================================
// Precache
//=========================================================
void CApacheHVR::Precache()
{
	if (pev->model)
		PRECACHE_MODEL((char*)STRING(pev->model)); //LRC
	else
		PRECACHE_MODEL("models/HVR.mdl");
	
	m_iTrail = PRECACHE_MODEL("sprites/smoke.spr");
	
	PRECACHE_SOUND("weapons/rocket1.wav");
}

//=========================================================
// IgniteThink
//=========================================================
void CApacheHVR::IgniteThink()
{
	pev->effects |= EF_LIGHT;

	// make rocket sound
	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/rocket1.wav", 1, 0.5);

	// rocket trail
	MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);

	WRITE_BYTE(TE_BEAMFOLLOW);
		WRITE_SHORT(entindex()); // entity
		WRITE_SHORT(m_iTrail); // model
		WRITE_BYTE(15); // life
		WRITE_BYTE(5); // width
		WRITE_BYTE(224); // r, g, b
		WRITE_BYTE(224); // r, g, b
		WRITE_BYTE(255); // r, g, b
		WRITE_BYTE(255); // brightness
	MESSAGE_END(); // move PHS/PVS data sending into here (SEND_ALL, SEND_PVS, SEND_PHS)

	// set to accelerate
	SetThink(&CApacheHVR::AccelerateThink);
	SetNextThink(0.1);
}

//=========================================================
// AccelerateThink
//=========================================================
void CApacheHVR::AccelerateThink()
{
	// check world boundaries
	if (pev->origin.x < -4096 || pev->origin.x > 4096 || pev->origin.y < -4096 || pev->origin.y > 4096 || pev->origin.z
		< -4096 || pev->origin.z > 4096)
	{
		UTIL_Remove(this);
		return;
	}

	// accelerate
	const float flSpeed = pev->velocity.Length();
	if (flSpeed < 1800)
	{
		pev->velocity = pev->velocity + m_vecForward * 200;
	}

	// re-aim
	pev->angles = UTIL_VecToAngles(pev->velocity);

	SetNextThink(0.1);
}
