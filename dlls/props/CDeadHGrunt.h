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

#ifndef CDEADHGRUNT_H
#define CDEADHGRUNT_H

//=========================================================
// Class definition of CDeadHGrunt
//=========================================================
class CDeadHGrunt : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	int Classify() override
	{
		return CLASS_HUMAN_MILITARY;
	}

	void KeyValue(KeyValueData* pkvd) override;

	int m_iPose; // which sequence to display	-- temporary, don't need to save
	static const char* m_szPoses[3];
};

#endif // CDEADHGRUNT_H
