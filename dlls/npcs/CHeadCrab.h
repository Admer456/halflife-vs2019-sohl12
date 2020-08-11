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

#ifndef CHEADCRAB_H
#define CHEADCRAB_H

#define	HC_AE_JUMPATTACK ( 2 )

#define CRAB_ATTN_IDLE (float)1.5

//=========================================================
// Class definition of CHeadCrab
//=========================================================
class CHeadCrab : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void RunTask(Task_t* pTask) override;
	void StartTask(Task_t* pTask) override;
	void SetYawSpeed() override;
	void EXPORT LeapTouch(CBaseEntity* pOther);
	Vector Center() override;
	Vector BodyTarget(const Vector& posSrc) override;
	void PainSound() override;
	void DeathSound() override;
	void IdleSound() override;
	void AlertSound() override;
	void PrescheduleThink() override;
	int Classify() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	BOOL CheckRangeAttack1(float flDot, float flDist) override;
	BOOL CheckRangeAttack2(float flDot, float flDist) override
	{
		return FALSE;
	}
	
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;

	virtual float GetDamageAmount()
	{
		return gSkillData.headcrabDmgBite;
	}
	
	virtual int GetVoicePitch()
	{
		return 100;
	}
	
	virtual float GetSoundVolue()
	{
		return 1.0;
	}
	
	Schedule_t* GetScheduleOfType(int Type) override;

	CUSTOM_SCHEDULES;

	static const char* pIdleSounds[];
	static const char* pAlertSounds[];
	static const char* pPainSounds[];
	static const char* pAttackSounds[];
	static const char* pDeathSounds[];
	static const char* pBiteSounds[];
};

#endif // CHEADCRAB_H
