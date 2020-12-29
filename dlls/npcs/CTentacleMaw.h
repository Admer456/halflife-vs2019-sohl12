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

#ifndef CTENTACLEMAW_H
#define CTENTACLEMAW_H

#ifndef SCRIPTED_H
#include "scripted.h"
#endif

//=========================================================
// Class definition of CTentacleMaw
//=========================================================
class CTentacleMaw : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
};

#endif // CTENTACLEMAW_H
