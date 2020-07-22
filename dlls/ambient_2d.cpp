#include "extdll.h"
#include "util.h"
#include "cbase.h"

/*
=======================================
	CAmbient2D
	Plays 2D sounds on clients
=======================================
*/
class CAmbient2D : public CBaseEntity
{
public:
	void			Spawn() override;
	void			KeyValue( KeyValueData* pkvd ) override;
	void			Use( CBaseEntity* activator, CBaseEntity* caller, USE_TYPE useType, float value );

private:
	string_t		soundName;
	float			soundVolume;
	int				soundChannel;
	int				soundPitch;
};

extern int gmsg2DSound;

LINK_ENTITY_TO_CLASS( ambient_2d, CAmbient2D );

void CAmbient2D::Spawn()
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	pev->model = NULL;
}

void CAmbient2D::KeyValue( KeyValueData* pkvd )
{
	if ( FStrEq( pkvd->szKeyName, "soundName" ) )
	{
		soundName = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	else if ( FStrEq( pkvd->szKeyName, "soundVolume" ) )
	{
		soundVolume = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	else if ( FStrEq( pkvd->szKeyName, "soundChannel" ) )
	{
		soundChannel = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	else if ( FStrEq( pkvd->szKeyName, "soundPitch" ) )
	{
		soundPitch = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	else
	{
		CBaseEntity::KeyValue( pkvd );
	}
}

void CAmbient2D::Use( CBaseEntity* activator, CBaseEntity* caller, USE_TYPE useType, float value )
{
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		auto player = UTIL_PlayerByIndex( i );
		if ( !player )
			continue;

		MESSAGE_BEGIN( MSG_ONE, gmsg2DSound, g_vecZero, player->pev );
		WRITE_BYTE( soundVolume * 255 );
		WRITE_BYTE( soundChannel );
		WRITE_BYTE( soundPitch );
		WRITE_STRING( STRING( soundName ) );
		MESSAGE_END();
	}
}
