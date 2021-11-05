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

#ifndef CBULLSQUID_H
#define CBULLSQUID_H

#define	SQUID_SPRINT_DIST	256 // how close the squid has to get before starting to sprint and refusing to swerve
#define SQUID_ATTN_IDLE	(float)1.5

//=========================================================
// monster-specific schedule types
//=========================================================
enum
{
	SCHED_SQUID_HURTHOP = LAST_COMMON_SCHEDULE + 1,
	SCHED_SQUID_SMELLFOOD,
	SCHED_SQUID_SEECRAB,
	SCHED_SQUID_EAT,
	SCHED_SQUID_SNIFF_AND_EAT,
	SCHED_SQUID_WALLOW,
};

//=========================================================
// monster-specific tasks
//=========================================================
enum
{
	TASK_SQUID_HOPTURN = LAST_COMMON_TASK + 1,
};

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define	BSQUID_AE_SPIT		( 1 )
#define	BSQUID_AE_BITE		( 2 )
#define	BSQUID_AE_BLINK		( 3 )
#define	BSQUID_AE_TAILWHIP	( 4 )
#define	BSQUID_AE_HOP		( 5 )
#define	BSQUID_AE_THROW		( 6 )

//=========================================================
// Class definition of CBullsquid
//=========================================================
class CBullsquid : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	int ISoundMask() override;
	int Classify() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	void IdleSound() override;
	void PainSound() override;
	void DeathSound() override;
	void AlertSound() override;
	void AttackSound();
	void StartTask(Task_t* pTask) override;
	void RunTask(Task_t* pTask) override;
	BOOL CheckMeleeAttack1(float flDot, float flDist) override;
	BOOL CheckMeleeAttack2(float flDot, float flDist) override;
	BOOL CheckRangeAttack1(float flDot, float flDist) override;
	void RunAI() override;
	BOOL FValidateHintType(short sHint) override;
	Schedule_t* GetSchedule() override;
	Schedule_t* GetScheduleOfType(int Type) override;
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
	int IRelationship(CBaseEntity* pTarget) override;
	int IgnoreConditions() override;
	MONSTERSTATE GetIdealState() override;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];
	
	CUSTOM_SCHEDULES;

	BOOL m_fCanThreatDisplay; // this is so the squid only does the "I see a headcrab!" dance one time. 

	float m_flLastHurtTime;
	// we keep track of this, because if something hurts a squid, it will forget about its love of headcrabs for a while.
	float m_flNextSpitTime; // last time the bullsquid used the spit attack.
	int iSquidSpitSprite;
};

#endif // CBULLSQUID_H
