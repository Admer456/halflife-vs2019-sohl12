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

#ifndef CSQUIDSPIT_H
#define CSQUIDSPIT_H

//=========================================================
// Class definition of CSquidSpit
//=========================================================
class CSquidSpit : public CBaseEntity
{
public:
	static void Shoot(entvars_t* pevOwner, Vector vecStart, Vector vecVelocity);

private:
	void Spawn() override;
	void Precache() override;

	void Touch(CBaseEntity* pOther) override;
	void EXPORT Animate();

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	int m_maxFrame;
	int iSquidSpitSprite;
};

#endif // CSQUIDSPIT_H
