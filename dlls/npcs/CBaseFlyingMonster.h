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

#ifndef CBASEFLYINGMONSTER_H
#define CBASEFLYINGMONSTER_H

#define FLYING_AE_FLAP		(8)
#define FLYING_AE_FLAPSOUND	(9)

//=========================================================
// Class definition of CBaseFlyingMonster
// Base class for flying monsters.
// This overrides the movement test & execution code from CBaseMonster
//=========================================================
class CBaseFlyingMonster : public CBaseMonster
{
public:
	int CheckLocalMove(const Vector& vecStart, const Vector& vecEnd, CBaseEntity* pTarget, float* pflDist) override;
	// check validity of a straight move through space
	BOOL FTriangulate(const Vector& vecStart, const Vector& vecEnd, float flDist, CBaseEntity* pTargetEnt, Vector* pApex) override;
	Activity GetStoppedActivity() override;
	void Killed(entvars_t* pevAttacker, int iGib) override;
	void Stop() override;
	float ChangeYaw(int speed) override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	void MoveExecute(CBaseEntity* pTargetEnt, const Vector& vecDir, float flInterval) override;
	void Move(float flInterval = 0.1) override;
	BOOL ShouldAdvanceRoute(float flWaypointDist) override;

	void SetFlyingMomentum(float momentum) { m_momentum = momentum; }
	void SetFlyingFlapSound(const char* pFlapSound) { m_pFlapSound = pFlapSound; }
	void SetFlyingSpeed(float speed) { m_flightSpeed = speed; }
	float CeilingZ(const Vector& position);
	float FloorZ(const Vector& position);
	BOOL ProbeZ(const Vector& position, const Vector& probe, float* pFraction);

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

protected:
	Vector m_vecTravel; // Current direction
	float m_flightSpeed; // Current flight speed (decays when not flapping or gliding)
	float m_stopTime; // Last time we stopped (to avoid switching states too soon)
	float m_momentum; // Weight for desired vs. momentum velocity
	const char* m_pFlapSound;
};


#endif	//CBASEFLYINGMONSTER_H
