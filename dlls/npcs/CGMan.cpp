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

#include "CGMan.h"

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(monster_gman, CGMan);

//=========================================================
// Save and Restore
//=========================================================
TYPEDESCRIPTION	CGMan::m_SaveData[] =
{
	DEFINE_FIELD(CGMan, m_hTalkTarget, FIELD_EHANDLE),
	DEFINE_FIELD(CGMan, m_flTalkTime, FIELD_TIME),
};

IMPLEMENT_SAVERESTORE(CGMan, CBaseMonster);

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CGMan::Classify()
{
	return m_iClass ? m_iClass : CLASS_NONE;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CGMan::SetYawSpeed()
{
	switch (m_Activity)
	{
		case ACT_IDLE:
		default:
			pev->yaw_speed = 90;
	}
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CGMan::HandleAnimEvent(MonsterEvent_t* pEvent)
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
int CGMan::ISoundMask()
{
	return NULL;
}

//=========================================================
// Spawn
//=========================================================
void CGMan::Spawn()
{
	Precache();

	if (pev->model)
		SET_MODEL(ENT(pev), STRING(pev->model)); //LRC
	else
		SET_MODEL(ENT(pev), "models/gman.mdl");
	
	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = DONT_BLEED;
	
	if (pev->health == 0) //LRC
		pev->health = 100;
	
	m_flFieldOfView = 0.5;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState = MONSTERSTATE_NONE;

	MonsterInit();
}

//=========================================================
// Precache
//=========================================================
void CGMan::Precache()
{
	if (pev->model)
		PRECACHE_MODEL((char*)STRING(pev->model)); //LRC
	else
		PRECACHE_MODEL("models/gman.mdl");
}

//=========================================================
// StartTask
//=========================================================
void CGMan::StartTask(Task_t* pTask)
{
	switch (pTask->iTask)
	{
		case TASK_WAIT:
			if (m_hPlayer == nullptr)
			{
				m_hPlayer = UTIL_FindEntityByClassname(nullptr, "player");
			}
		break;
	}
	
	CBaseMonster::StartTask(pTask);
}

//=========================================================
// RunTask
//=========================================================
void CGMan::RunTask(Task_t* pTask)
{
	switch (pTask->iTask)
	{
		case TASK_WAIT:
			// look at who I'm talking to
			if (m_flTalkTime > gpGlobals->time && m_hTalkTarget != nullptr)
			{
				float yaw = VecToYaw(m_hTalkTarget->pev->origin - pev->origin) - pev->angles.y;

				if (yaw > 180) yaw -= 360;
				if (yaw < -180) yaw += 360;

				// turn towards vector
				SetBoneController(0, yaw);
			}
			// look at player, but only if playing a "safe" idle animation
			else if (m_hPlayer != nullptr && pev->sequence == 0)
			{
				float yaw = VecToYaw(m_hPlayer->pev->origin - pev->origin) - pev->angles.y;

				if (yaw > 180) yaw -= 360;
				if (yaw < -180) yaw += 360;

				// turn towards vector
				SetBoneController(0, yaw);
			}
			else
			{
				SetBoneController(0, 0);
			}
			
			CBaseMonster::RunTask(pTask);
		break;
		default:
			SetBoneController(0, 0);
			CBaseMonster::RunTask(pTask);
		break;
	}
}

//=========================================================
// TakeDamage
//=========================================================
int CGMan::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	pev->health = pev->max_health / 2; // always trigger the 50% damage aitrigger

	if (flDamage > 0)
	{
		SetConditions(bits_COND_LIGHT_DAMAGE);
	}

	if (flDamage >= 20)
	{
		SetConditions(bits_COND_HEAVY_DAMAGE);
	}
	
	return TRUE;
}

//=========================================================
// TraceAttack
//=========================================================
void CGMan::TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType)
{
	UTIL_Ricochet(ptr->vecEndPos, 1.0);
	
	AddMultiDamage(pevAttacker, this, flDamage, bitsDamageType);
}

//=========================================================
// PlayScriptedSentence
//=========================================================
void CGMan::PlayScriptedSentence(const char* pszSentence, float duration, float volume, float attenuation, BOOL bConcurrent, CBaseEntity* pListener)
{
	CBaseMonster::PlayScriptedSentence(pszSentence, duration, volume, attenuation, bConcurrent, pListener);

	m_flTalkTime = gpGlobals->time + duration;
	m_hTalkTarget = pListener;
}
