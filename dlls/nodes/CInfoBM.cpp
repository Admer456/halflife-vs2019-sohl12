/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CInfoBM.h"

//=========================================================
// Link
//=========================================================
LINK_ENTITY_TO_CLASS(info_bigmomma, CInfoBM);

//=========================================================
// Save and Restore
//=========================================================
TYPEDESCRIPTION CInfoBM::m_SaveData[] =
{
	DEFINE_FIELD(CInfoBM, m_preSequence, FIELD_STRING),
};

IMPLEMENT_SAVERESTORE(CInfoBM, CPointEntity);

//=========================================================
// KeyValue
//=========================================================
void CInfoBM::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "radius"))
	{
		pev->scale = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "reachdelay"))
	{
		pev->speed = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "reachtarget"))
	{
		pev->message = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "reachsequence"))
	{
		pev->netname = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "presequence"))
	{
		m_preSequence = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CPointEntity::KeyValue(pkvd);
}