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

#ifndef CINFOBM_H
#define CINFOBM_H

#define SF_INFOBM_RUN	0x0001
#define SF_INFOBM_WAIT	0x0002

//=========================================================
// Class definition of CInfoBM - AI Nodes for Big Momma
//=========================================================
class CInfoBM : public CPointEntity
{
public:
	void Spawn() override {};
	void KeyValue(KeyValueData* pkvd) override;

	// name in pev->targetname
	// next in pev->target
	// radius in pev->scale
	// health in pev->health
	// Reach target in pev->message
	// Reach delay in pev->speed
	// Reach sequence in pev->netname

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	int m_preSequence;
};


#endif // CINFOBM_H
