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

#ifndef COSPREY_H
#define COSPREY_H

#ifndef SCRIPTED_H
#include "scripted.h"
#endif

typedef struct
{
	int isValid;
	EHANDLE hGrunt;
	Vector vecOrigin;
	Vector vecAngles;
} t_ospreygrunt;

#define SF_WAITFORTRIGGER	0x40

#define MAX_CARRY	24

//=========================================================
// Class definition of COsprey
//=========================================================
class COsprey : public CBaseMonster
{
public:
	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];
	int ObjectCaps() override { return CBaseMonster::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

	void Spawn() override;
	void Precache() override;
	int Classify() override { return CLASS_MACHINE; };
	int BloodColor() override { return DONT_BLEED; }
	void Killed(entvars_t* pevAttacker, int iGib) override;

	void UpdateGoal();
	BOOL HasDead();
	void Flight();
	
	void EXPORT FlyThink();
	void EXPORT DeployThink();
	void EXPORT HitTouch(CBaseEntity* pOther);
	void EXPORT FindAllThink();
	void EXPORT HoverThink();
	
	CBaseMonster* MakeGrunt(Vector vecSrc);
	
	void EXPORT CrashTouch(CBaseEntity* pOther);
	void EXPORT DyingThink();
	void EXPORT CommandUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	// int  TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType );
	void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType) override;
	void ShowDamage();

	CBaseEntity* m_pGoalEnt;
	Vector m_vel1;
	Vector m_vel2;
	Vector m_pos1;
	Vector m_pos2;
	Vector m_ang1;
	Vector m_ang2;
	float m_startTime;
	float m_dTime;

	Vector m_velocity;

	float m_flIdealtilt;
	float m_flRotortilt;

	float m_flRightHealth;
	float m_flLeftHealth;

	int m_iUnits;
	
	EHANDLE m_hGrunt[MAX_CARRY];
	Vector m_vecOrigin[MAX_CARRY];
	EHANDLE m_hRepel[4];

	int m_iSoundState;
	int m_iSpriteTexture;

	int m_iPitch;

	int m_iExplode;
	int m_iTailGibs;
	int m_iBodyGibs;
	int m_iEngineGibs;

	int m_iDoLeftSmokePuff;
	int m_iDoRightSmokePuff;
};

#endif // COSPREY_H
