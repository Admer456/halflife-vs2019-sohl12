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

#ifndef CGARGANTUA_H
#define CGARGANTUA_H

#ifndef CSPIRAL_H
#include "effects/CSpiral.h"
#endif

#ifndef CSTOMP_H
#include "effects/CStomp.h"
#endif

const float GARG_ATTACKDIST = 80.0;

// Garg animation events
#define GARG_AE_SLASH_LEFT			1
//#define GARG_AE_BEAM_ATTACK_RIGHT	2		// No longer used
#define GARG_AE_LEFT_FOOT			3
#define GARG_AE_RIGHT_FOOT			4
#define GARG_AE_STOMP				5
#define GARG_AE_BREATHE				6


// Gargantua is immune to any damage but this
#define GARG_DAMAGE					(DMG_ENERGYBEAM|DMG_CRUSH|DMG_MORTAR|DMG_BLAST)
#define GARG_EYE_SPRITE_NAME		"sprites/gargeye1.spr"
#define GARG_BEAM_SPRITE_NAME		"sprites/xbeam3.spr"
#define GARG_BEAM_SPRITE2			"sprites/xbeam3.spr"
#define GARG_STOMP_SPRITE_NAME		"sprites/gargeye1.spr"
#define GARG_STOMP_BUZZ_SOUND		"weapons/mine_charge.wav"
#define GARG_FLAME_LENGTH			330
#define GARG_GIB_MODEL				"models/metalplategibs.mdl"

#define ATTN_GARG					(ATTN_NORM)

#define STOMP_SPRITE_COUNT			10

//=========================================================
// Class definition of CGargantua
//=========================================================
class CGargantua : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void UpdateOnRemove() override;
	void SetYawSpeed() override;
	int Classify() override;
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
	void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr,
	int bitsDamageType) override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;

	BOOL CheckMeleeAttack1(float flDot, float flDist) override; // Swipe
	BOOL CheckMeleeAttack2(float flDot, float flDist) override; // Flames
	BOOL CheckRangeAttack1(float flDot, float flDist) override; // Stomp attack
	void SetObjectCollisionBox() override
	{
		pev->absmin = pev->origin + Vector(-80, -80, 0);
		pev->absmax = pev->origin + Vector(80, 80, 214);
	}

	Schedule_t* GetScheduleOfType(int Type) override;
	void StartTask(Task_t* pTask) override;
	void RunTask(Task_t* pTask) override;

	void PrescheduleThink() override;

	void Killed(entvars_t* pevAttacker, int iGib) override;
	void DeathEffect();

	void EyeOff();
	void EyeOn(int level);
	void EyeUpdate();
	void StompAttack();
	void FlameCreate();
	void FlameUpdate();
	void FlameControls(float angleX, float angleY);
	void FlameDestroy();
	BOOL FlameIsOn() { return m_pFlame[0] != nullptr; }

	void FlameDamage(Vector vecStart, Vector vecEnd, entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage,
		int iClassIgnore, int bitsDamageType);

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	CUSTOM_SCHEDULES;

private:
	static const char* pAttackHitSounds[];
	static const char* pBeamAttackSounds[];
	static const char* pAttackMissSounds[];
	static const char* pRicSounds[];
	static const char* pFootSounds[];
	static const char* pIdleSounds[];
	static const char* pAlertSounds[];
	static const char* pPainSounds[];
	static const char* pAttackSounds[];
	static const char* pStompSounds[];
	static const char* pBreatheSounds[];

	CBaseEntity* GargantuaCheckTraceHullAttack(float flDist, int iDamage, int iDmgType);

	CSprite* m_pEyeGlow; // Glow around the eyes
	CBeam* m_pFlame[4]; // Flame beams

	int gStompSprite = 0;
	int gGargGibModel = 0;
	int m_eyeBrightness; // Brightness target
	
	float m_seeTime; // Time to attack (when I see the enemy, I set this)
	float m_flameTime; // Time of next flame attack
	float m_painSoundTime; // Time of next pain sound
	float m_streakTime; // streak timer (don't send too many)
	float m_flameX; // Flame thrower aim
	float m_flameY;
};

#endif // CGARGANTUA_H
