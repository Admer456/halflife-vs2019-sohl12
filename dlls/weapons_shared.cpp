/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"


CWeaponRegistry* CWeaponRegistry::m_pHead = NULL;

//seems like an entirely new files added by solokiller
// most edits tagged with "solokiller" were added by IjaazCM3 on Aug 1 2020
// here is a list of all changes to this, EXCLUDING the changes Admer456 made to this to update it for VS2019

/*
unified github diff of all the stuff I added:

https://github.com/Solokiller/half-life-better-weapons/commit/b5f44678a40bfdb0c5c7eaf329aeba08b46b1d71?branch=b5f44678a40bfdb0c5c7eaf329aeba08b46b1d71&diff=unified

split github diff of all the stuff I added:

https://github.com/Solokiller/half-life-better-weapons/commit/b5f44678a40bfdb0c5c7eaf329aeba08b46b1d71?branch=b5f44678a40bfdb0c5c7eaf329aeba08b46b1d71&diff=split

*/