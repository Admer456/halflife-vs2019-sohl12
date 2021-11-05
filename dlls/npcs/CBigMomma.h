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

#ifndef CBIGMOMMA_H
#define CBIGMOMMA_H

#ifndef CINFOBM_H
#include "nodes/CInfoBM.h"
#endif

#ifndef CBMORTAR_H
#include "projectiles/CBMortar.h"
#endif

//=========================================================
// Monster's Anim Events Go Here
//=========================================================

//LRC brought in from animation.h
#define ACTIVITY_NOT_AVAILABLE		-1

#define	BIG_AE_STEP1				1		// Footstep left
#define	BIG_AE_STEP2				2		// Footstep right
#define	BIG_AE_STEP3				3		// Footstep back left
#define	BIG_AE_STEP4				4		// Footstep back right
#define BIG_AE_SACK					5		// Sack slosh
#define BIG_AE_DEATHSOUND			6		// Death sound

#define	BIG_AE_MELEE_ATTACKBR		8		// Leg attack
#define	BIG_AE_MELEE_ATTACKBL		9		// Leg attack
#define	BIG_AE_MELEE_ATTACK1		10		// Leg attack
#define BIG_AE_MORTAR_ATTACK1		11		// Launch a mortar
#define BIG_AE_LAY_CRAB				12		// Lay a headcrab
#define BIG_AE_JUMP_FORWARD			13		// Jump up and forward
#define BIG_AE_SCREAM				14		// alert sound
#define BIG_AE_PAIN_SOUND			15		// pain sound
#define BIG_AE_ATTACK_SOUND			16		// attack sound
#define BIG_AE_BIRTH_SOUND			17		// birth sound
#define BIG_AE_EARLY_TARGET			50		// Fire target early

// User defined conditions
#define bits_COND_NODE_SEQUENCE			( bits_COND_SPECIAL1 )		// pev->netname contains the name of a sequence to play

// Attack distance constants
#define	BIG_ATTACKDIST		170
#define BIG_MORTARDIST		800
#define BIG_MAXCHILDREN		20			// Max # of live headcrab children

#define bits_MEMORY_CHILDPAIR		(bits_MEMORY_CUSTOM1)
#define bits_MEMORY_ADVANCE_NODE	(bits_MEMORY_CUSTOM2)
#define bits_MEMORY_COMPLETED_NODE	(bits_MEMORY_CUSTOM3)
#define bits_MEMORY_FIRED_NODE		(bits_MEMORY_CUSTOM4)

#define BIG_CHILDCLASS	"monster_babycrab"

//=========================================================
// Class definition of CBigMomma
//=========================================================
class CBigMomma : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void Activate() override;
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;

	void RunTask(Task_t* pTask) override;
	void StartTask(Task_t* pTask) override;
	Schedule_t* GetSchedule() override;
	Schedule_t* GetScheduleOfType(int Type) override;
	void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr,
	                 int bitsDamageType) override;
	void SetActivity(Activity NewActivity) override;

	void NodeStart(int iszNextNode);
	void NodeReach();
	BOOL ShouldGoToNode();

	void SetYawSpeed() override;
	int Classify() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	void LayHeadcrab();

	int GetNodeSequence()
	{
		CBaseEntity* pTarget = m_hTargetEnt;
		if (pTarget)
		{
			return pTarget->pev->netname; // netname holds node sequence
		}
		return 0;
	}


	int GetNodePresequence()
	{
		CInfoBM* pTarget = static_cast<CInfoBM*>(static_cast<CBaseEntity*>(m_hTargetEnt));
		if (pTarget)
		{
			return pTarget->m_preSequence;
		}
		return 0;
	}

	float GetNodeDelay()
	{
		CBaseEntity* pTarget = m_hTargetEnt;
		if (pTarget)
		{
			return pTarget->pev->speed; // Speed holds node delay
		}
		return 0;
	}

	float GetNodeRange()
	{
		CBaseEntity* pTarget = m_hTargetEnt;
		if (pTarget)
		{
			return pTarget->pev->scale; // Scale holds node delay
		}
		return 1e6;
	}

	float GetNodeYaw()
	{
		CBaseEntity* pTarget = m_hTargetEnt;
		if (pTarget)
		{
			if (pTarget->pev->angles.y != 0)
				return pTarget->pev->angles.y;
		}
		return pev->angles.y;
	}

	// Restart the crab count on each new level
	void OverrideReset() override
	{
		m_crabCount = 0;
	}

	void DeathNotice(entvars_t* pevChild) override;

	BOOL CanLayCrab()
	{
		if (m_crabTime < gpGlobals->time && m_crabCount < BIG_MAXCHILDREN)
		{
			// Don't spawn crabs inside each other
			Vector mins = pev->origin - Vector(32, 32, 0);
			Vector maxs = pev->origin + Vector(32, 32, 0);

			CBaseEntity* pList[2];
			int count = UTIL_EntitiesInBox(pList, 2, mins, maxs, FL_MONSTER);
			for (int i = 0; i < count; i++)
			{
				if (pList[i] != this) // Don't hurt yourself!
					return FALSE;
			}
			return TRUE;
		}

		return FALSE;
	}

	void LaunchMortar();

	void SetObjectCollisionBox() override
	{
		pev->absmin = pev->origin + Vector(-95, -95, 0);
		pev->absmax = pev->origin + Vector(95, 95, 190);
	}

	BOOL CheckMeleeAttack1(float flDot, float flDist) override; // Slash
	BOOL CheckMeleeAttack2(float flDot, float flDist) override; // Lay a crab
	BOOL CheckRangeAttack1(float flDot, float flDist) override; // Mortar launch

	Vector VecCheckSplatToss(entvars_t* pev, const Vector& vecSpot1, Vector vecSpot2, float maxHeight) const;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	static const char* pChildDieSounds[];
	static const char* pSackSounds[];
	static const char* pDeathSounds[];
	static const char* pAttackSounds[];
	static const char* pAttackHitSounds[];
	static const char* pBirthSounds[];
	static const char* pAlertSounds[];
	static const char* pPainSounds[];
	static const char* pFootSounds[];

	CUSTOM_SCHEDULES;

private:
	float m_nodeTime;
	float m_crabTime;
	float m_mortarTime;
	float m_painSoundTime;
	int m_crabCount;
	int gSpitSprite;

};

#endif // CBIGMOMMA_H
