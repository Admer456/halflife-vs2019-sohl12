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
#include "CSquidSpit.h"

#ifndef SKILL_H
#include "skill.h"
#endif

#ifndef DECALS_H
#include "decals.h"
#endif

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(squidspit, CSquidSpit);

//=========================================================
// Save and Restore
//=========================================================
TYPEDESCRIPTION CSquidSpit::m_SaveData[] =
{
	DEFINE_FIELD(CSquidSpit, m_maxFrame, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CSquidSpit, CBaseEntity);

//=========================================================
// Spawn 
//=========================================================
void CSquidSpit::Spawn()
{
	pev->movetype = MOVETYPE_FLY;
	pev->classname = MAKE_STRING("squidspit");

	pev->solid = SOLID_BBOX;
	pev->rendermode = kRenderTransAlpha;
	pev->renderamt = 255;

	SET_MODEL(ENT(pev), "sprites/bigspit.spr");
	
	pev->frame = 0;
	pev->scale = 0.5;

	UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));

	m_maxFrame = static_cast<float>(MODEL_FRAMES(pev->modelindex)) - 1;
}

//=========================================================
// Precache
//=========================================================
void CSquidSpit::Precache()
{
	PRECACHE_MODEL("sprites/bigspit.spr");

	iSquidSpitSprite = PRECACHE_MODEL("sprites/tinyspit.spr");

	PRECACHE_SOUND("bullchicken/bc_acid1.wav");
	PRECACHE_SOUND("bullchicken/bc_spithit1.wav");
	PRECACHE_SOUND("bullchicken/bc_spithit2.wav");
}

//=========================================================
// Animate 
//=========================================================
void CSquidSpit::Animate()
{
	SetNextThink(0.1);

	if (pev->frame++)
	{
		if (pev->frame > m_maxFrame)
		{
			pev->frame = 0;
		}
	}
}

//=========================================================
// Shoot 
//=========================================================
void CSquidSpit::Shoot(entvars_t* pevOwner, Vector vecStart, Vector vecVelocity)
{
	CSquidSpit* pSpit = GetClassPtr(static_cast<CSquidSpit*>(nullptr));
	
	pSpit->Precache();
	pSpit->Spawn();

	UTIL_SetOrigin(pSpit, vecStart);
	
	pSpit->pev->velocity = vecVelocity;
	pSpit->pev->owner = ENT(pevOwner);

	pSpit->SetThink(&CSquidSpit::Animate);
	pSpit->SetNextThink(0.1);
}

//=========================================================
// Touch 
//=========================================================
void CSquidSpit::Touch(CBaseEntity* pOther)
{
	TraceResult tr;

	// splat sound
	const int iPitch = RANDOM_FLOAT(90, 110);

	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "bullchicken/bc_acid1.wav", 1, ATTN_NORM, 0, iPitch);

	switch (RANDOM_LONG(0, 1))
	{
		case 0:
			EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "bullchicken/bc_spithit1.wav", 1, ATTN_NORM, 0, iPitch);
		break;
		case 1:
			EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "bullchicken/bc_spithit2.wav", 1, ATTN_NORM, 0, iPitch);
		break;
	}

	if (!pOther->pev->takedamage)
	{
		// make a splat on the wall
		UTIL_TraceLine(pev->origin, pev->origin + pev->velocity * 10, dont_ignore_monsters, ENT(pev), &tr);
		UTIL_DecalTrace(&tr, DECAL_SPIT1 + RANDOM_LONG(0, 1));

		// make some flecks
		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, tr.vecEndPos);
			WRITE_BYTE(TE_SPRITE_SPRAY);
			WRITE_COORD(tr.vecEndPos.x); // pos
			WRITE_COORD(tr.vecEndPos.y);
			WRITE_COORD(tr.vecEndPos.z);
			WRITE_COORD(tr.vecPlaneNormal.x); // dir
			WRITE_COORD(tr.vecPlaneNormal.y);
			WRITE_COORD(tr.vecPlaneNormal.z);
			WRITE_SHORT(iSquidSpitSprite); // model
			WRITE_BYTE(5); // count
			WRITE_BYTE(30); // speed
			WRITE_BYTE(80); // noise ( client will divide by 100 )
		MESSAGE_END();
	}
	else
		pOther->TakeDamage(pev, pev, gSkillData.bullsquidDmgSpit, DMG_GENERIC);

	SetThink(&CSquidSpit::SUB_Remove);
	SetNextThink(0);
}
