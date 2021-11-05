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

#ifndef CSCIENTIST_H
#define CSCIENTIST_H

#ifndef TALKMONSTER_H
#include "talkmonster.h"
#endif

#define		NUM_SCIENTIST_HEADS		4 // four heads available for scientist model

enum { HEAD_GLASSES = 0, HEAD_EINSTEIN = 1, HEAD_LUTHER = 2, HEAD_SLICK = 3 };

enum
{
	SCHED_HIDE = LAST_TALKMONSTER_SCHEDULE + 1,
	SCHED_FEAR,
	SCHED_PANIC,
	SCHED_STARTLE,
	SCHED_TARGET_CHASE_SCARED,
	SCHED_TARGET_FACE_SCARED,
};

enum
{
	TASK_SAY_HEAL = LAST_TALKMONSTER_TASK + 1,
	TASK_HEAL,
	TASK_SAY_FEAR,
	TASK_RUN_PATH_SCARED,
	TASK_SCREAM,
	TASK_RANDOM_SCREAM,
	TASK_MOVE_TO_TARGET_RANGE_SCARED,
};

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		SCIENTIST_AE_HEAL		( 1 )
#define		SCIENTIST_AE_NEEDLEON	( 2 )
#define		SCIENTIST_AE_NEEDLEOFF	( 3 )

//=========================================================
// Class definition of CScientist
//=========================================================
class CScientist : public CTalkMonster
{
public:
	CScientist();
	
	void Spawn() override;
	void Precache() override;

	void SetYawSpeed() override;
	int Classify() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	void RunTask(Task_t* pTask) override;
	void StartTask(Task_t* pTask) override;
	int ObjectCaps() override { return CTalkMonster::ObjectCaps() | FCAP_IMPULSE_USE; }
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
	int FriendNumber(int arrayNumber) override;
	void SetActivity(Activity newActivity) override;
	Activity GetStoppedActivity() override;
	int ISoundMask() override;
	void DeclineFollowing() override;

	float CoverRadius() override { return 1200; }
	// Need more room for cover because scientists want to get far away!
	BOOL DisregardEnemy(CBaseEntity* pEnemy) { return !pEnemy->IsAlive() || (gpGlobals->time - m_fearTime) > 15; }

	BOOL CanHeal();
	void Heal();
	void Scream();

	// Override these to set behavior
	Schedule_t* GetScheduleOfType(int Type) override;
	Schedule_t* GetSchedule() override;
	MONSTERSTATE GetIdealState() override;

	void DeathSound() override;
	void PainSound() override;

	void TalkInit() override;

	void Killed(entvars_t* pevAttacker, int iGib) override;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	CUSTOM_SCHEDULES;

private:
	float m_painTime;
	float m_healTime;
	float m_fearTime;
};

#endif // CSCIENTIST_H
