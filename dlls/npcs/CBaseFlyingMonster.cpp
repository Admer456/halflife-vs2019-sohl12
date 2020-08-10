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
#include "monsters.h"

#include "CBaseFlyingMonster.h"

extern DLL_GLOBAL edict_t* g_pBodyQueueHead;

//=========================================================
// Save and Restore
//=========================================================
TYPEDESCRIPTION CBaseFlyingMonster::m_SaveData[] =
{
	DEFINE_FIELD(CBaseFlyingMonster, m_vecTravel, FIELD_VECTOR),
	DEFINE_FIELD(CBaseFlyingMonster, m_flightSpeed, FIELD_FLOAT),
	DEFINE_FIELD(CBaseFlyingMonster, m_stopTime, FIELD_FLOAT),
	DEFINE_FIELD(CBaseFlyingMonster, m_momentum, FIELD_FLOAT),
};

IMPLEMENT_SAVERESTORE(CBaseFlyingMonster, CBaseMonster);

//=========================================================
// CheckLocalMove
//=========================================================
int CBaseFlyingMonster::CheckLocalMove(const Vector& vecStart, const Vector& vecEnd, CBaseEntity* pTarget,
                                       float* pflDist)
{
	// UNDONE: need to check more than the endpoint
	if (FBitSet(pev->flags, FL_SWIM) && (UTIL_PointContents(vecEnd) != CONTENTS_WATER))
	{
		// ALERT(at_aiconsole, "can't swim out of water\n");
		return FALSE;
	}

	TraceResult tr;

	UTIL_TraceHull(vecStart + Vector(0, 0, 32), vecEnd + Vector(0, 0, 32), dont_ignore_monsters, large_hull, edict(),
	               &tr);

	// ALERT( at_console, "%.0f %.0f %.0f : ", vecStart.x, vecStart.y, vecStart.z );
	// ALERT( at_console, "%.0f %.0f %.0f\n", vecEnd.x, vecEnd.y, vecEnd.z );

	if (pflDist)
	{
		*pflDist = ((tr.vecEndPos - Vector(0, 0, 32)) - vecStart).Length(); // get the distance.
	}

	// ALERT( at_console, "check %d %d %f\n", tr.fStartSolid, tr.fAllSolid, tr.flFraction );
	if (tr.fStartSolid || tr.flFraction < 1.0)
	{
		if (pTarget && pTarget->edict() == gpGlobals->trace_ent)
			return LOCALMOVE_VALID;
		return LOCALMOVE_INVALID;
	}

	return LOCALMOVE_VALID;
}

//=========================================================
// FTriangulate
//=========================================================
BOOL CBaseFlyingMonster::FTriangulate(const Vector& vecStart, const Vector& vecEnd, float flDist,
                                      CBaseEntity* pTargetEnt, Vector* pApex)
{
	return CBaseMonster::FTriangulate(vecStart, vecEnd, flDist, pTargetEnt, pApex);
}

//=========================================================
// GetStoppedActivity
//=========================================================
Activity CBaseFlyingMonster::GetStoppedActivity()
{
	if (pev->movetype != MOVETYPE_FLY) // UNDONE: Ground idle here, IDLE may be something else
		return ACT_IDLE;

	return ACT_HOVER;
}

//=========================================================
// Stop
//=========================================================
void CBaseFlyingMonster::Stop()
{
	Activity stopped = GetStoppedActivity();
	if (m_IdealActivity != stopped)
	{
		m_flightSpeed = 0;
		m_IdealActivity = stopped;
	}
	
	pev->angles.z = 0;
	pev->angles.x = 0;
	m_vecTravel = g_vecZero;
}

//=========================================================
// ChangeYaw
//=========================================================
float CBaseFlyingMonster::ChangeYaw(int speed)
{
	if (pev->movetype == MOVETYPE_FLY)
	{
		float diff = FlYawDiff();
		float target = 0;

		if (m_IdealActivity != GetStoppedActivity())
		{
			if (diff < -20)
				target = 90;
			else if (diff > 20)
				target = -90;
		}
		pev->angles.z = UTIL_Approach(target, pev->angles.z, 220.0 * gpGlobals->frametime);
	}
	return CBaseMonster::ChangeYaw(speed);
}

//=========================================================
// Killed
//=========================================================
void CBaseFlyingMonster::Killed(entvars_t* pevAttacker, int iGib)
{
	pev->movetype = MOVETYPE_STEP;
	ClearBits(pev->flags, FL_ONGROUND);
	pev->angles.z = 0;
	pev->angles.x = 0;
	CBaseMonster::Killed(pevAttacker, iGib);
}

//=========================================================
// HandleAnimEvent
//=========================================================
void CBaseFlyingMonster::HandleAnimEvent(MonsterEvent_t* pEvent)
{
	switch (pEvent->event)
	{
	case FLYING_AE_FLAP:
		m_flightSpeed = 400;
		break;

	case FLYING_AE_FLAPSOUND:
		if (m_pFlapSound)
			EMIT_SOUND(edict(), CHAN_BODY, m_pFlapSound, 1, ATTN_NORM);
		break;

	default:
		CBaseMonster::HandleAnimEvent(pEvent);
		break;
	}
}

//=========================================================
// Move
//=========================================================
void CBaseFlyingMonster::Move(float flInterval)
{
	if (pev->movetype == MOVETYPE_FLY)
		m_flGroundSpeed = m_flightSpeed;
	
	CBaseMonster::Move(flInterval);
}

//=========================================================
// ShouldAdvanceRoute
//=========================================================
BOOL CBaseFlyingMonster::ShouldAdvanceRoute(float flWaypointDist)
{
	// Get true 3D distance to the goal so we actually reach the correct height
	if (m_Route[m_iRouteIndex].iType & bits_MF_IS_GOAL)
		flWaypointDist = (m_Route[m_iRouteIndex].vecLocation - pev->origin).Length();

	if (flWaypointDist <= 64 + (m_flGroundSpeed * gpGlobals->frametime))
		return TRUE;

	return FALSE;
}

//=========================================================
// MoveExecute
//=========================================================
void CBaseFlyingMonster::MoveExecute(CBaseEntity* pTargetEnt, const Vector& vecDir, float flInterval)
{
	if (pev->movetype == MOVETYPE_FLY)
	{
		if (gpGlobals->time - m_stopTime > 1.0)
		{
			if (m_IdealActivity != m_movementActivity)
			{
				m_IdealActivity = m_movementActivity;
				m_flGroundSpeed = m_flightSpeed = 200;
			}
		}
		
		Vector vecMove = pev->origin + ((vecDir + (m_vecTravel * m_momentum)).Normalize() * (m_flGroundSpeed *
			flInterval));

		if (m_IdealActivity != m_movementActivity)
		{
			m_flightSpeed = UTIL_Approach(100, m_flightSpeed, 75 * gpGlobals->frametime);
			if (m_flightSpeed < 100)
				m_stopTime = gpGlobals->time;
		}
		else
			m_flightSpeed = UTIL_Approach(20, m_flightSpeed, 300 * gpGlobals->frametime);

		if (CheckLocalMove(pev->origin, vecMove, pTargetEnt, nullptr))
		{
			m_vecTravel = (vecMove - pev->origin);
			m_vecTravel = m_vecTravel.Normalize();
			UTIL_MoveToOrigin(ENT(pev), vecMove, (m_flGroundSpeed * flInterval), MOVE_STRAFE);
		}
		else
		{
			m_IdealActivity = GetStoppedActivity();
			m_stopTime = gpGlobals->time;
			m_vecTravel = g_vecZero;
		}
	}
	else
		CBaseMonster::MoveExecute(pTargetEnt, vecDir, flInterval);
}

//=========================================================
// CeilingZ
//=========================================================
float CBaseFlyingMonster::CeilingZ(const Vector& position)
{
	TraceResult tr;

	Vector minUp = position;
	Vector maxUp = position;
	maxUp.z += 4096.0;

	UTIL_TraceLine(position, maxUp, ignore_monsters, nullptr, &tr);
	if (tr.flFraction != 1.0)
		maxUp.z = tr.vecEndPos.z;

	if ((pev->flags) & FL_SWIM)
	{
		return UTIL_WaterLevel(position, minUp.z, maxUp.z);
	}
	return maxUp.z;
}

//=========================================================
// ProbeZ
//=========================================================
BOOL CBaseFlyingMonster::ProbeZ(const Vector& position, const Vector& probe, float* pFraction)
{
	int conPosition = UTIL_PointContents(position);
	if ((((pev->flags) & FL_SWIM) == FL_SWIM) ^ (conPosition == CONTENTS_WATER))
	{
		//    SWIMING & !WATER
		// or FLYING  & WATER
		//
		*pFraction = 0.0;
		return TRUE; // We hit a water boundary because we are where we don't belong.
	}
	int conProbe = UTIL_PointContents(probe);
	if (conProbe == conPosition)
	{
		// The probe is either entirely inside the water (for fish) or entirely
		// outside the water (for birds).
		//
		*pFraction = 1.0;
		return FALSE;
	}

	Vector ProbeUnit = (probe - position).Normalize();
	float ProbeLength = (probe - position).Length();
	float maxProbeLength = ProbeLength;
	float minProbeLength = 0;

	float diff = maxProbeLength - minProbeLength;
	while (diff > 1.0)
	{
		float midProbeLength = minProbeLength + diff / 2.0;
		Vector midProbeVec = midProbeLength * ProbeUnit;
		if (UTIL_PointContents(position + midProbeVec) == conPosition)
		{
			minProbeLength = midProbeLength;
		}
		else
		{
			maxProbeLength = midProbeLength;
		}
		diff = maxProbeLength - minProbeLength;
	}
	
	*pFraction = minProbeLength / ProbeLength;

	return TRUE;
}

//=========================================================
// FloorZ
//=========================================================
float CBaseFlyingMonster::FloorZ(const Vector& position)
{
	TraceResult tr;

	Vector down = position;
	down.z -= 2048;

	UTIL_TraceLine(position, down, ignore_monsters, nullptr, &tr);

	if (tr.flFraction != 1.0)
		return tr.vecEndPos.z;

	return down.z;
}
