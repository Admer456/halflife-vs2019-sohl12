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

#include "CControllerZapBall.h"

#ifndef SKILL_H
#include "skill.h"
#endif

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(controller_energy_ball, CControllerZapBall);

//=========================================================
// Save and Restore
//=========================================================
TYPEDESCRIPTION CControllerZapBall::m_SaveData[] =
{
	DEFINE_FIELD(CControllerZapBall, m_maxFrame, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CControllerZapBall, CBaseEntity);

//=========================================================
// Spawn
//=========================================================
void CControllerZapBall::Spawn()
{
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pev), "sprites/xspark4.spr");
	
	pev->rendermode = kRenderTransAdd;
	pev->rendercolor.x = 255;
	pev->rendercolor.y = 255;
	pev->rendercolor.z = 255;
	pev->renderamt = 200;

	UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));
	
	UTIL_SetOrigin(this, pev->origin);

	m_maxFrame = static_cast<float>(MODEL_FRAMES(pev->modelindex)) - 1;
	
	m_hOwner = Instance(pev->owner);
	
	pev->dmgtime = gpGlobals->time; // keep track of when ball spawned
}

//=========================================================
// Precache
//=========================================================
void CControllerZapBall::Precache()
{
	PRECACHE_MODEL("sprites/xspark4.spr");
	
	PRECACHE_SOUND("weapons/electro4.wav");
}

//=========================================================
// Animate
//=========================================================
void CControllerZapBall::Animate()
{
	if (pev->frame++)
	{
		if (pev->frame > m_maxFrame)
		{
			pev->frame = 0;
		}
	}

	if (gpGlobals->time - pev->dmgtime > 5 || pev->velocity.Length() < 10)
	{
		SetTouch(NULL);
		UTIL_Remove(this);
	}

	SetNextThink(0.1);
}

//=========================================================
// ExplodeTouch
//=========================================================
void CControllerZapBall::ExplodeTouch(CBaseEntity* pOther)
{
	if (pOther->pev->takedamage)
	{
		TraceResult tr = UTIL_GetGlobalTrace();

		entvars_t* pevOwner;
		if (m_hOwner != NULL)
		{
			pevOwner = m_hOwner->pev;
		}
		else
		{
			pevOwner = pev;
		}

		ClearMultiDamage();
		
		pOther->TraceAttack(pevOwner, gSkillData.controllerDmgBall, pev->velocity.Normalize(), &tr, DMG_ENERGYBEAM);
		
		ApplyMultiDamage(pevOwner, pevOwner);

		UTIL_EmitAmbientSound(ENT(pev), tr.vecEndPos, "weapons/electro4.wav", 0.3, ATTN_NORM, 0, RANDOM_LONG(90, 99));

	}

	UTIL_Remove(this);
}

//=========================================================
// Shoot
//=========================================================
CControllerZapBall* CControllerZapBall::Shoot(entvars_t* baseOwner, EHANDLE m_hEnemy, Vector vecStart, Vector vecVelocity)
{
	CControllerZapBall* pBall = GetClassPtr(static_cast<CControllerZapBall*>(nullptr));

	pBall->Precache();
	pBall->Spawn();

	UTIL_SetOrigin(pBall, vecStart);

	pBall->m_hEnemy = m_hEnemy;
	pBall->pev->velocity = vecVelocity;
	pBall->pev->angles = baseOwner->angles;
	pBall->pev->owner = baseOwner->owner;
	pBall->pev->scale = 0.5;
	pBall->SetThink(&CControllerZapBall::Animate);
	pBall->SetTouch(&CControllerZapBall::ExplodeTouch);
	pBall->SetNextThink(0.1);

	return pBall;
}