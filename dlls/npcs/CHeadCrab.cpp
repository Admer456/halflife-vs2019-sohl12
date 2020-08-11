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
#include "schedule.h"
#include "game.h"

#include "CHeadCrab.h"

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(monster_headcrab, CHeadCrab);

//=========================================================
// Sounds
//=========================================================
const char* CHeadCrab::pIdleSounds[] =
{
	"headcrab/hc_idle1.wav",
	"headcrab/hc_idle2.wav",
	"headcrab/hc_idle3.wav",
};
const char* CHeadCrab::pAlertSounds[] =
{
	"headcrab/hc_alert1.wav",
};
const char* CHeadCrab::pPainSounds[] =
{
	"headcrab/hc_pain1.wav",
	"headcrab/hc_pain2.wav",
	"headcrab/hc_pain3.wav",
};
const char* CHeadCrab::pAttackSounds[] =
{
	"headcrab/hc_attack1.wav",
	"headcrab/hc_attack2.wav",
	"headcrab/hc_attack3.wav",
};

const char* CHeadCrab::pDeathSounds[] =
{
	"headcrab/hc_die1.wav",
	"headcrab/hc_die2.wav",
};

const char* CHeadCrab::pBiteSounds[] =
{
	"headcrab/hc_headbite.wav",
};

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int CHeadCrab::Classify()
{
	return m_iClass ? m_iClass : CLASS_ALIEN_PREY;
}

//=========================================================
// Center - returns the real center of the headcrab.  The 
// bounding box is much larger than the actual creature so 
// this is needed for targeting
//=========================================================
Vector CHeadCrab::Center()
{
	return Vector(pev->origin.x, pev->origin.y, pev->origin.z + 6);
}

//=========================================================
// BodyTarget
//=========================================================
Vector CHeadCrab::BodyTarget(const Vector& posSrc)
{
	return Center();
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CHeadCrab::SetYawSpeed()
{
	switch (m_Activity)
	{
	case ACT_RUN:
	case ACT_WALK:
		pev->yaw_speed = 20;
		break;
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:
		pev->yaw_speed = 60;
		break;
	case ACT_IDLE:
	case ACT_RANGE_ATTACK1:
	default:
		pev->yaw_speed = 30;
		break;
	}
}

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
Task_t tlHCRangeAttack1[] =
{
	{TASK_STOP_MOVING, static_cast<float>(0)},
	{TASK_FACE_IDEAL, static_cast<float>(0)},
	{TASK_RANGE_ATTACK1, static_cast<float>(0)},
	{TASK_SET_ACTIVITY, static_cast<float>(ACT_IDLE)},
	{TASK_FACE_IDEAL, static_cast<float>(0)},
	{TASK_WAIT_RANDOM, static_cast<float>(0.5)},
};

Schedule_t slHCRangeAttack1[] =
{
	{
		tlHCRangeAttack1,
		ARRAYSIZE(tlHCRangeAttack1),
		bits_COND_ENEMY_OCCLUDED |
		bits_COND_NO_AMMO_LOADED,
		0,
		"HCRangeAttack1"
	},
};

Task_t tlHCRangeAttack1Fast[] =
{
	{TASK_STOP_MOVING, static_cast<float>(0)},
	{TASK_FACE_IDEAL, static_cast<float>(0)},
	{TASK_RANGE_ATTACK1, static_cast<float>(0)},
	{TASK_SET_ACTIVITY, static_cast<float>(ACT_IDLE)},
};

Schedule_t slHCRangeAttack1Fast[] =
{
	{
		tlHCRangeAttack1Fast,
		ARRAYSIZE(tlHCRangeAttack1Fast),
		bits_COND_ENEMY_OCCLUDED |
		bits_COND_NO_AMMO_LOADED,
		0,
		"HCRAFast"
	},
};

DEFINE_CUSTOM_SCHEDULES(CHeadCrab)
{
	slHCRangeAttack1,
	slHCRangeAttack1Fast,
};

IMPLEMENT_CUSTOM_SCHEDULES(CHeadCrab, CBaseMonster);

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CHeadCrab::HandleAnimEvent(MonsterEvent_t* pEvent)
{
	switch (pEvent->event)
	{
		case HC_AE_JUMPATTACK:
		{
			ClearBits(pev->flags, FL_ONGROUND);

			UTIL_SetOrigin(this, pev->origin + Vector(0, 0, 1));
			// take him off ground so engine doesn't instantly reset onground 
			UTIL_MakeVectors(pev->angles);

			Vector vecJumpDir;
			if (m_hEnemy != NULL)
			{
				float gravity = g_psv_gravity->value;
				if (gravity <= 1)
					gravity = 1;

				// How fast does the headcrab need to travel to reach that height given gravity?
				float height = (m_hEnemy->pev->origin.z + m_hEnemy->pev->view_ofs.z - pev->origin.z);
				if (height < 16)
					height = 16;
				
				float speed = sqrt(2 * gravity * height);
				float time = speed / gravity;

				// Scale the sideways velocity to get there at the right time
				vecJumpDir = (m_hEnemy->pev->origin + m_hEnemy->pev->view_ofs - pev->origin);
				vecJumpDir = vecJumpDir * (1.0 / time);

				// Speed to offset gravity at the desired height
				vecJumpDir.z = speed;

				// Don't jump too far/fast
				float distance = vecJumpDir.Length();

				if (distance > 650)
				{
					vecJumpDir = vecJumpDir * (650.0 / distance);
				}
			}
			else
			{
				// jump hop, don't care where
				vecJumpDir = Vector(gpGlobals->v_forward.x, gpGlobals->v_forward.y, gpGlobals->v_up.z) * 350;
			}

			int iSound = RANDOM_LONG(0, 2);
			if (iSound != 0)
				EMIT_SOUND_DYN(edict(), CHAN_VOICE, pAttackSounds[iSound], GetSoundVolue(), ATTN_IDLE, 0,
				               GetVoicePitch());

			pev->velocity = vecJumpDir;
			m_flNextAttack = gpGlobals->time + 2;
		}
		break;
		default:
			CBaseMonster::HandleAnimEvent(pEvent);
		break;
	}
}

//=========================================================
// Spawn
//=========================================================
void CHeadCrab::Spawn()
{
	Precache();

	if (pev->model)
		SET_MODEL(ENT(pev), STRING(pev->model)); //LRC
	else
		SET_MODEL(ENT(pev), "models/headcrab.mdl");
	
	UTIL_SetSize(pev, Vector(-12, -12, 0), Vector(12, 12, 24));

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = BLOOD_COLOR_GREEN;
	
	pev->effects = 0;
	
	if (pev->health == 0)
		pev->health = gSkillData.headcrabHealth;
	
	pev->view_ofs = Vector(0, 0, 20); // position of the eyes relative to monster's origin.
	pev->yaw_speed = 5;
	
	//!!! should we put this in the monster's changeanim function since turn rates may vary with state/anim?
	m_flFieldOfView = 0.5; // indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState = MONSTERSTATE_NONE;

	MonsterInit();
}

//=========================================================
// Precache
//=========================================================
void CHeadCrab::Precache()
{
	if (pev->model)
		PRECACHE_MODEL((char*)STRING(pev->model)); //LRC
	else
		PRECACHE_MODEL("models/headcrab.mdl");

	PRECACHE_SOUND_ARRAY(pIdleSounds);
	PRECACHE_SOUND_ARRAY(pAlertSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
	PRECACHE_SOUND_ARRAY(pAttackSounds);
	PRECACHE_SOUND_ARRAY(pDeathSounds);
	PRECACHE_SOUND_ARRAY(pBiteSounds);
}


//=========================================================
// RunTask 
//=========================================================
void CHeadCrab::RunTask(Task_t* pTask)
{
	switch (pTask->iTask)
	{
		case TASK_RANGE_ATTACK1:
		case TASK_RANGE_ATTACK2:
		{
			if (m_fSequenceFinished)
			{
				TaskComplete();
				SetTouch(NULL);
				m_IdealActivity = ACT_IDLE;
			}
			break;
		}
		default:
		{
			CBaseMonster::RunTask(pTask);
		}
	}
}

//=========================================================
// LeapTouch - this is the headcrab's touch function when it
// is in the air
//=========================================================
void CHeadCrab::LeapTouch(CBaseEntity* pOther)
{
	if (!pOther->pev->takedamage)
		return;

	if (pOther->Classify() == Classify())
		return;

	// Don't hit if back on ground
	if (!FBitSet(pev->flags, FL_ONGROUND))
	{
		EMIT_SOUND_DYN(edict(), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pBiteSounds), GetSoundVolue(), ATTN_IDLE, 0,
		               GetVoicePitch());

		pOther->TakeDamage(pev, pev, GetDamageAmount(), DMG_SLASH);
	}

	SetTouch(NULL);
}

//=========================================================
// PrescheduleThink
//=========================================================
void CHeadCrab::PrescheduleThink()
{
	// make the crab coo a little bit in combat state
	if (m_MonsterState == MONSTERSTATE_COMBAT && RANDOM_FLOAT(0, 5) < 0.1)
	{
		IdleSound();
	}
}

//=========================================================
// StartTask
//=========================================================
void CHeadCrab::StartTask(Task_t* pTask)
{
	m_iTaskStatus = TASKSTATUS_RUNNING;

	switch (pTask->iTask)
	{
		case TASK_RANGE_ATTACK1:
		{
			EMIT_SOUND_DYN(edict(), CHAN_WEAPON, pAttackSounds[0], GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
			m_IdealActivity = ACT_RANGE_ATTACK1;
			SetTouch(&CHeadCrab::LeapTouch);
			break;
		}
		default:
		{
			CBaseMonster::StartTask(pTask);
		}
	}
}

//=========================================================
// CheckRangeAttack1
//=========================================================
BOOL CHeadCrab::CheckRangeAttack1(float flDot, float flDist)
{
	if (FBitSet(pev->flags, FL_ONGROUND) && flDist <= 256 && flDot >= 0.65)
		return TRUE;
	
	return FALSE;
}

//=========================================================
// TakeDamage
//=========================================================
int CHeadCrab::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	// Don't take any acid damage -- BigMomma's mortar is acid
	if (bitsDamageType & DMG_ACID)
		flDamage = 0;

	return CBaseMonster::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
}

//=========================================================
// IdleSound
//=========================================================
void CHeadCrab::IdleSound()
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pIdleSounds), GetSoundVolue(), ATTN_IDLE, 0,
	               GetVoicePitch());
}

//=========================================================
// AlertSound 
//=========================================================
void CHeadCrab::AlertSound()
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pAlertSounds), GetSoundVolue(), ATTN_IDLE, 0,
	               GetVoicePitch());
}

//=========================================================
// AlertSound 
//=========================================================
void CHeadCrab::PainSound()
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pPainSounds), GetSoundVolue(), ATTN_IDLE, 0,
	               GetVoicePitch());
}

//=========================================================
// DeathSound 
//=========================================================
void CHeadCrab::DeathSound()
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), GetSoundVolue(), ATTN_IDLE, 0,
	               GetVoicePitch());
}

//=========================================================
// GetScheduleOfType
//=========================================================
Schedule_t* CHeadCrab::GetScheduleOfType(int Type)
{
	switch (Type)
	{
		case SCHED_RANGE_ATTACK1:
			return &slHCRangeAttack1[0];
	}

	return CBaseMonster::GetScheduleOfType(Type);
}
