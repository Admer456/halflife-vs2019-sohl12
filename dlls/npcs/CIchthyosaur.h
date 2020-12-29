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

#ifndef CICHTHYOSAUR_H
#define CICHTHYOSAUR_H

#ifndef SCRIPTED_H
#include "scripted.h"
#endif

#define SEARCH_RETRY	16

#define ICHTHYOSAUR_SPEED 150

#define EYE_MAD		0
#define EYE_BASE	1
#define EYE_CLOSED	2
#define EYE_BACK	3
#define EYE_LOOK	4

#define ICHTHYOSAUR_AE_SHAKE_RIGHT 1
#define ICHTHYOSAUR_AE_SHAKE_LEFT  2

//=========================================================
// Class definition of CIchthyosaur
//=========================================================
class CIchthyosaur : public CBaseFlyingMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	int Classify() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	CUSTOM_SCHEDULES;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	Schedule_t* GetSchedule() override;
	Schedule_t* GetScheduleOfType(int Type) override;

	void Killed(entvars_t* pevAttacker, int iGib) override;
	void BecomeDead() override;

	void EXPORT CombatUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
	void EXPORT BiteTouch(CBaseEntity* pOther);

	void StartTask(Task_t* pTask) override;
	void RunTask(Task_t* pTask) override;

	BOOL CheckMeleeAttack1(float flDot, float flDist) override;
	BOOL CheckRangeAttack1(float flDot, float flDist) override;

	float ChangeYaw(int speed) override;
	Activity GetStoppedActivity() override;

	void Move(float flInterval) override;
	void MoveExecute(CBaseEntity* pTargetEnt, const Vector& vecDir, float flInterval) override;
	void MonsterThink() override;
	void Stop() override;
	void Swim();
	Vector DoProbe(const Vector& Probe);

	float VectorToPitch(const Vector& vec);
	float FlPitchDiff();
	float ChangePitch(int speed);

	Vector m_SaveVelocity;
	float m_idealDist;

	float m_flBlink;

	float m_flEnemyTouched;
	BOOL m_bOnAttack;

	float m_flMaxSpeed;
	float m_flMinSpeed;
	float m_flMaxDist;

	CBeam* m_pBeam;

	float m_flNextAlert;

	static const char* pIdleSounds[];
	static const char* pAlertSounds[];
	static const char* pAttackSounds[];
	static const char* pBiteSounds[];
	static const char* pDieSounds[];
	static const char* pPainSounds[];

	void IdleSound() override;
	void AlertSound() override;
	void AttackSound();
	void BiteSound();
	void DeathSound() override;
	void PainSound() override;
};

#endif // CICHTHYOSAUR_H
