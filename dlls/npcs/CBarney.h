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

#ifndef CBARNEY_H
#define CBARNEY_H

#ifndef TALKMONSTER_H
#include "talkmonster.h"
#endif

#ifndef SCRIPTED_H
#include "scripted.h"
#endif

//=========================================================
// Monster's flags
//=========================================================
enum
{
	BARNEY_AE_DRAW = (2),
	BARNEY_AE_SHOOT = (3),
	BARNEY_AE_HOLSTER = (4),

	BARNEY_BODY_GUNHOLSTERED = 0,
	BARNEY_BODY_GUNDRAWN = 1,
	BARNEY_BODY_GUNGONE = 2
};

//=========================================================
// Class definition of CBarney
//=========================================================
class CBarney : public CTalkMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	int ISoundMask() override;
	void BarneyFirePistol();
	void AlertSound() override;
	int Classify() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;

	void RunTask(Task_t* pTask) override;
	void StartTask(Task_t* pTask) override;
	int ObjectCaps() override { return CTalkMonster::ObjectCaps() | FCAP_IMPULSE_USE; }
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
	BOOL CheckRangeAttack1(float flDot, float flDist) override;

	void DeclineFollowing() override;

	// Override these to set behavior
	Schedule_t* GetScheduleOfType(int Type) override;
	Schedule_t* GetSchedule() override;
	MONSTERSTATE GetIdealState() override;

	void DeathSound() override;
	void PainSound() override;

	void TalkInit() override;

	void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr,
	                 int bitsDamageType) override;
	void Killed(entvars_t* pevAttacker, int iGib) override;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	int m_iBaseBody; //LRC - for barneys with different bodies
	BOOL m_fGunDrawn;
	float m_painTime;
	float m_checkAttackTime;
	BOOL m_lastAttackCheck;

	// UNDONE: What is this for?  It isn't used?
	float m_flPlayerDamage; // how much pain has the player inflicted on me?

	CUSTOM_SCHEDULES;
};

#endif // CBARNEY
