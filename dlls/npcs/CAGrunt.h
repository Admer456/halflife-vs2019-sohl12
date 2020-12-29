/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#ifndef CAGRUNT_H
#define CAGRUNT_H

#ifndef SCRIPTED_H
#include "scripted.h"
#endif

//=========================================================
// monster-specific schedule types
//=========================================================
enum
{
	SCHED_AGRUNT_SUPPRESS = LAST_COMMON_SCHEDULE + 1,
	SCHED_AGRUNT_THREAT_DISPLAY,
};

//=========================================================
// monster-specific tasks
//=========================================================
enum
{
	TASK_AGRUNT_SETUP_HIDE_ATTACK = LAST_COMMON_TASK + 1,
	TASK_AGRUNT_GET_PATH_TO_ENEMY_CORPSE,
};

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
enum
{
	AGRUNT_AE_HORNET1     = 1,
	AGRUNT_AE_HORNET2     = 2,
	AGRUNT_AE_HORNET3	  = 3,
	AGRUNT_AE_HORNET4     = 4,
	AGRUNT_AE_HORNET5     = 5,
	AGRUNT_AE_PUNCH	      = 6,
	AGRUNT_AE_BITE        = 7,
	AGRUNT_AE_LEFT_FOOT   = 10,
	AGRUNT_AE_RIGHT_FOOT  = 11,
	AGRUNT_AE_LEFT_PUNCH  = 12,
	AGRUNT_AE_RIGHT_PUNCH = 13
};

//=========================================================
// Monster options
//=========================================================
#define	AGRUNT_MELEE_DIST 100

//=========================================================
// LRC - body definitions for the Agrunt model
//=========================================================
enum
{
	AGRUNT_BODY_HASGUN = 0,
	AGRUNT_BODY_NOGUN  = 1
};

//=========================================================
// Class definition of CAGrunt
//=========================================================
class CAGrunt : public CSquadMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	int Classify() override;
	int ISoundMask() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;

	void SetObjectCollisionBox() override
	{
		pev->absmin = pev->origin + Vector(-32, -32, 0);
		pev->absmax = pev->origin + Vector(32, 32, 85);
	}

	Schedule_t* GetSchedule() override;
	Schedule_t* GetScheduleOfType(int Type) override;
	
	BOOL FCanCheckAttacks() override;
	BOOL CheckMeleeAttack1(float flDot, float flDist) override;
	BOOL CheckRangeAttack1(float flDot, float flDist) override;
	
	void StartTask(Task_t* pTask) override;
	void AlertSound() override;
	void DeathSound() override;
	void PainSound() override;
	void AttackSound();
	void StopTalking();

	BOOL ShouldSpeak();
	
	void PrescheduleThink() override;
	void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr,
		int bitsDamageType) override;
	
	int IRelationship(CBaseEntity* pTarget) override;
	void Killed(entvars_t* pevAttacker, int iGib) override;

	CUSTOM_SCHEDULES;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	static const char* pAttackHitSounds[];
	static const char* pAttackMissSounds[];
	static const char* pAttackSounds[];
	static const char* pDieSounds[];
	static const char* pPainSounds[];
	static const char* pIdleSounds[];
	static const char* pAlertSounds[];

	BOOL m_fCanHornetAttack;
	
	float m_flNextHornetAttackCheck;
	float m_flNextPainTime;

	// three hacky fields for speech stuff. These don't really need to be saved.
	float m_flNextSpeakTime;
	float m_flNextWordTime;
	
	int m_iLastWord;
	int iAgruntMuzzleFlash;
};

#endif // CAGRUNT_H