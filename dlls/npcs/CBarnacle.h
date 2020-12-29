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

#ifndef CBARNACLE_H
#define CBARNACLE_H

//=========================================================
// Monster's flags
//=========================================================
#define	BARNACLE_AE_PUKEGIB			2
#define	BARNACLE_BODY_HEIGHT		44 // how 'tall' the barnacle's model is.
#define BARNACLE_PULL_SPEED			8
#define BARNACLE_KILL_VICTIM_DELAY	5 // how many seconds after pulling prey in to gib them.
#define BARNACLE_CHECK_SPACING		8

//=========================================================
// Class definition of CBarnacle
//=========================================================
class CBarnacle : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	int Classify() override;

	void EXPORT BarnacleThink();
	void EXPORT WaitTillDead();

	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	void Killed(entvars_t* pevAttacker, int iGib) override;
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;

	CBaseEntity* TongueTouchEnt(float* pflLength);
	
	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	float m_flAltitude;
	float m_flKillVictimTime;
	float m_flTongueAdj;
	
	int m_cGibs; // barnacle loads up on gibs each time it kills something.
	
	BOOL m_fTongueExtended;
	BOOL m_fLiftingPrey;
};

#endif // CBARNACLE_H