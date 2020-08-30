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

#ifndef CTENTACLE_H
#define CTENTACLE_H

#ifndef SCRIPTED_H
#include "scripted.h"
#endif

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define ACT_T_IDLE		1010
#define ACT_T_TAP		1020
#define ACT_T_STRIKE	1030
#define ACT_T_REARIDLE	1040

#define TE_NONE -1
#define TE_SILO 0
#define TE_DIRT 1
#define TE_WATER 2

//=========================================================
// Class definition of CTentacle
//=========================================================
class CTentacle : public CBaseMonster
{
public:
	CTentacle();

	void Spawn() override;
	void Precache() override;
	void KeyValue(KeyValueData* pkvd) override;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	// Don't allow the tentacle to go across transitions!!!
	int ObjectCaps() override { return CBaseMonster::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

	void SetObjectCollisionBox() override
	{
		pev->absmin = pev->origin + Vector(-400, -400, 0);
		pev->absmax = pev->origin + Vector(400, 400, 850);
	}

	void EXPORT Cycle();
	void EXPORT CommandUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
	void EXPORT Start();
	void EXPORT DieThink();

	void EXPORT Test();

	void EXPORT HitTouch(CBaseEntity* pOther);

	float HearingSensitivity() override { return 2.0; };

	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	void Killed(entvars_t* pevAttacker, int iGib) override;

	MONSTERSTATE GetIdealState() override { return MONSTERSTATE_IDLE; };
	//	int CanPlaySequence( BOOL fDisregardState ) { return TRUE; };
	int CanPlaySequence(int interruptFlags) override { return TRUE; };

	int Classify() override;

	int Level(float dz);
	int MyLevel();
	float MyHeight();

	float m_flInitialYaw;
	int m_iGoalAnim;
	int m_iLevel;
	int m_iDir;
	float m_flFramerateAdj;
	float m_flSoundYaw;
	int m_iSoundLevel;
	float m_flSoundTime;
	float m_flSoundRadius;
	int m_iHitDmg;
	float m_flHitTime;

	float m_flTapRadius;

	float m_flNextSong;
	static int g_fFlySound;
	static int g_fSquirmSound;

	float m_flMaxYaw;
	int m_iTapSound;

	Vector m_vecPrevSound;
	float m_flPrevSoundTime;

	static const char* pHitSilo[];
	static const char* pHitDirt[];
	static const char* pHitWater[];
};

#endif // CTENTACLE_H
