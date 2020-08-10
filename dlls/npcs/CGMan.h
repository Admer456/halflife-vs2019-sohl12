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

#ifndef CGMAN_H
#define CGMAN_H

#ifndef WEAPONS_H
#include "weapons.h"
#endif

//=========================================================
// Class definition of CGMan
//=========================================================
class CGMan : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	int Classify() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	int ISoundMask() override;

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	void StartTask(Task_t* pTask) override;
	void RunTask(Task_t* pTask) override;
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
	void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr,
	                 int bitsDamageType) override;

	void PlayScriptedSentence(const char* pszSentence, float duration, float volume, float attenuation,
	                          BOOL bConcurrent, CBaseEntity* pListener) override;

	EHANDLE m_hPlayer;
	EHANDLE m_hTalkTarget;
	float m_flTalkTime;
};

#endif // CGMAN_H
