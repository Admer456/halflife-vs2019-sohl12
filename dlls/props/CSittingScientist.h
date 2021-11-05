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

#ifndef CSITTINGSCIENTIST_H
#define CSITTINGSCIENTIST_H

// animation sequence aliases 
enum SITTING_ANIM
{
	SITTING_ANIM_sitlookleft,
	SITTING_ANIM_sitlookright,
	SITTING_ANIM_sitscared,
	SITTING_ANIM_sitting2,
	SITTING_ANIM_sitting3
};

#define SF_SITTINGSCI_POSTDISASTER 1024

//=========================================================
// Class definition of CScientist
//=========================================================
class CSittingScientist : public CScientist // kdb: changed from public CBaseMonster so he can speak
{
public:
	void Spawn() override;
	void Precache() override;

	void EXPORT SittingThink();
	int Classify() override;
	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	void SetAnswerQuestion(CTalkMonster* pSpeaker) override;
	int FriendNumber(int arrayNumber) override;

	int FIdleSpeak();
	
	int m_baseSequence;
	int m_headTurn;
	float m_flResponseDelay;
};

#endif // CSITTINGSCIENTIST_H
