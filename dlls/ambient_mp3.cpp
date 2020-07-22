#include "extdll.h"
#include "util.h"
#include "cbase.h"

/*
=======================================
	CAmbientMP3
	Plays one song at a time
	If there are multiple instances of this entity,
	then playing another one while one is already playing,
	will stop the first one
=======================================
*/
class CAmbientMP3 : public CBaseEntity
{
public:
	void				Spawn();
	void				KeyValue( KeyValueData* pkvd );
	void				Use( CBaseEntity* activator, CBaseEntity* caller, USE_TYPE useType, float value );
	void				Think();

	void				SetVolume(); // Sets the MP3 volume in the console
	void				Stop(); // stops the MP3
	void				Play(); // plays the thing

	void				UpdateSongMode();
	bool				CanUpdateSongMode() { return entindex() == entityWhichCanUpdate; }

protected:
	string_t			songName;
	float				songVolumeMultiplier{1.f};

	enum class SongMode
	{
		Stopped,
		Playing,
		FadingIn,
		FadingOut,
		FadingOut_NewSong
	};
	
	// Since these static variables are being changed in UpdateSongMode,
	// every instance of this entity will update them
	// This variable will allow only one entity to update this
	static int			entityWhichCanUpdate;

	static SongMode		mode;
	static string_t		currentSongName;
	static float		songVolume;
	static float		originalSongVolume;
};

// Dang. Static class variables declared in classes gotta be defined outside of them
int						CAmbientMP3::entityWhichCanUpdate = -1;
CAmbientMP3::SongMode	CAmbientMP3::mode = SongMode::Stopped;
string_t				CAmbientMP3::currentSongName = 0;
float					CAmbientMP3::songVolume = 0.f;
float					CAmbientMP3::originalSongVolume = 1.f;

constexpr int SF_LOOP = 1;

LINK_ENTITY_TO_CLASS( ambient_mp3, CAmbientMP3 );

void CAmbientMP3::Spawn()
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	pev->model = NULL;

	mode = SongMode::Stopped;

	originalSongVolume = CVAR_GET_FLOAT( "MP3Volume" );
	songVolume = originalSongVolume;

	// If entityWhichCanUpdate hasn't been set, then claim it
	if ( entityWhichCanUpdate == -1 )
		entityWhichCanUpdate = entindex();

	SetNextThink( 0.1f );
}

void CAmbientMP3::KeyValue( KeyValueData* pkvd )
{
	if ( FStrEq( pkvd->szKeyName, "songName" ) )
	{
		songName = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq( pkvd->szKeyName, "songVolumeMul" ) )
	{
		songVolumeMultiplier = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
	{
		CBaseEntity::KeyValue( pkvd );
	}
}

void CAmbientMP3::Use( CBaseEntity* activator, CBaseEntity* caller, USE_TYPE useType, float value )
{
	// If a song is already being played, stop that song then play the new one
	if ( mode == SongMode::Playing )
	{
		mode = SongMode::FadingOut_NewSong;
	}
	
	currentSongName = songName;

	if ( mode == SongMode::Stopped )
	{
		UpdateSongMode();
	}
}

void CAmbientMP3::Think()
{
	if ( !CanUpdateSongMode() )
	{
		DontThink();
		return;
	}

	switch ( mode )
	{
	case SongMode::FadingIn:
	{
		songVolume += 0.05f;
	}
	break;

	case SongMode::FadingOut:
	{
		songVolume -= 0.05f;
	}
	break;

	case SongMode::FadingOut_NewSong:
	{
		songVolume -= 0.08f; // Fading out to a new song is faster
	}
	break;

	default:
	{
		SetNextThink( 1.0f );
		return;
	}
	}

	if ( songVolume < 0.f )
		songVolume = 0.f;

	else if ( songVolume > originalSongVolume )
		songVolume = originalSongVolume;

	UpdateSongMode();
	SetNextThink( 0.1f );
}

// Doesn't exactly work for some reason
void CAmbientMP3::SetVolume()
{
	//char command[64];
	//sprintf( command, "MP3Volume %1.3f\n", songVolume * songVolumeMultiplier );

	//CVAR_SET_FLOAT( "MP3Volume", songVolume * songVolumeMultiplier );

	//for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	//{
	//	auto player = UTIL_PlayerByIndex( i );
	//	if ( !player )
	//		continue;
	//
	//	edict_t* playerEdict = player->edict();
	//	CLIENT_COMMAND( playerEdict, command );
	//}
}

void CAmbientMP3::Stop()
{
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		auto player = UTIL_PlayerByIndex( i );
		if ( !player )
			continue;

		edict_t* playerEdict = player->edict();
		CLIENT_COMMAND( playerEdict, "mp3 stop\n" );
	}
}

void CAmbientMP3::Play()
{
	char command[64];

	if ( pev->spawnflags & SF_LOOP )
		sprintf( command, "mp3 loop %s\n", STRING( currentSongName ) );
	else 
		sprintf( command, "mp3 play %s\n", STRING( currentSongName ) );

	// Go thru' all clients and send them this command
	// This doesn't work on SERVER_COMMAND, unfortunately
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		auto player = UTIL_PlayerByIndex( i );
		if ( !player )
			continue;

		edict_t* playerEdict = player->edict();
		CLIENT_COMMAND( playerEdict, command );
	}
}

void CAmbientMP3::UpdateSongMode()
{
	// You didn't claim your entityWhichCanUpdate ID? NO SWEETS FOR YOU
	if ( !CanUpdateSongMode() )
		return;

	switch ( mode )
	{
	case SongMode::Stopped:
	{
		Play();
		mode = SongMode::FadingIn;
		songVolume = 0.0f;
	}
	break;

	case SongMode::FadingIn:
	{
		if ( songVolume > originalSongVolume * 0.95f )
		{
			mode = SongMode::Playing;
		}
	}
	break;

	case SongMode::Playing:
	{
		mode = SongMode::FadingOut;
	}
	break;

	case SongMode::FadingOut:
	{
		if ( songVolume < originalSongVolume * 0.05f )
		{
			mode = SongMode::Stopped;
			Stop();
		}
	}
	break;

	case SongMode::FadingOut_NewSong:
	{
		if ( songVolume < originalSongVolume * 0.05f )
		{
			mode = SongMode::FadingIn;
			Stop();
			Play();
		}
	}
	break;
	}

	ALERT( at_console, "Songvol: %1.2f entindex %i\n", songVolume, entindex() );
}
