#include "hud.h"
#include "cl_util.h"
#include "debug_wireframe.h"

byte g_DebugWireframeStates = 0;

bool g_DebugWireframeInitialisation = true;

static_assert(DebugWireframe_Max < 8, "DebugWireframe_Max must NOT be higher than 8");

/*
==================================
	GetModeFromString

	Returns a debug wireframe
	mode from a CVar string 
	that the user has enter
==================================
*/
DebugWireframeModes GetModeFromString( const char* cvarString )
{
	char modeName[16];
	int modeNamePos = 0;

	// Check if the string is empty
	if ( !cvarString || !cvarString[0] )
	{
		gEngfuncs.Con_Printf( "ERROR: CVar string for debug_wireframe_set empty!\n" );
		return DebugWireframe_Unknown;
	}

	// Find the actual name of the mode
	for ( int i = 0; i <= strlen( cvarString ); i++ )
	{
		if ( cvarString[i] == ' ' )
			break;

		// Must not go further than 14 characters, gotta take into account the \0 at the end
		if ( modeNamePos > 14 )
			break;

		modeName[modeNamePos] = cvarString[i];
		modeNamePos++;
	}

	modeName[modeNamePos] = '\0';

	// Compare it
	if ( !strcmp( modeName, "everything" ) )
	{
		return DebugWireframe_Max;
	}
	else if ( !strcmp( modeName, "clipnodes" ) )
	{
		return DebugWireframe_Clipnodes;
	}
	else if ( !strcmp( modeName, "movement" ) )
	{
		return DebugWireframe_Movement;
	}
	else if ( !strcmp( modeName, "boundingbox" ) )
	{
		return DebugWireframe_BoundingBox;
	}

	return DebugWireframe_Unknown;
}

/*
==================================
	GetStateFromString

	Returns whether the selected
	debug wireframe state
	should be on or off,
	from the CVar string that
	the user entered
==================================
*/
bool GetStateFromString( const char* cvarString )
{
	char number = '0';

	if ( !cvarString || !cvarString[0] )
	{
		return false;
	}

	// Find the first occurence of a number
	// This way, the user of the command can do this:
	// debug_wireframe_set clipnodes0
	// OR
	// debug_wireframe_set "clipnodes 0"
	// Same outcome, really!
	for ( int i = 0; i <= strlen( cvarString ); i++ )
	{
		if ( cvarString[i] >= '0' && cvarString[i] <= '9' )
		{
			number = cvarString[i];
		}
	}

	return number != '0';
}

/*
==================================
	ThinkDebugWireframe

	Called in CHud::Think
	Updates and checks the CVar
	for debug wireframes
==================================
*/
void ThinkDebugWireframe()
{
	const char* cvarString = gHUD.m_pCvarDebugWireframe->string;

	DebugWireframeModes mode = GetModeFromString( cvarString );
	bool state = GetStateFromString( cvarString );

	if ( mode < DebugWireframe_Max )
	{
		if ( state )
			g_DebugWireframeStates |= 1 << mode;
		else
			g_DebugWireframeStates &= ~(1 << mode);
	}

	else if ( mode == DebugWireframe_Max )
	{
		// If state is true, then this is 
		// 255 (all bits set) else it's 0
		g_DebugWireframeStates = (state * 255);
	}
}

// Utility to see whether a certain
// mode is turned on or off
bool DebugWireState( int state )
{
	return g_DebugWireframeStates & (1 << state);
}

// Called in tri.cpp
void RenderDebugWireframe()
{
	// Clipnode stuff not implemented
	// It's not easy to accurately get data
	// from there...
	if ( DebugWireState( DebugWireframe_Clipnodes ) )
	{
		RenderDebugWireframe_Clipnodes();
	}

	if ( DebugWireState( DebugWireframe_BoundingBox ) )
	{
		RenderDebugWireframe_BoundingBox();
	}

	if ( DebugWireState( DebugWireframe_Movement ) )
	{
		RenderDebugWireframe_Movement();
	}

	// Force one call during initialisation
	if ( g_DebugWireframeInitialisation )
	{
		RenderDebugWireframe_Clipnodes();
		RenderDebugWireframe_BoundingBox();
		RenderDebugWireframe_Movement();

		g_DebugWireframeInitialisation = false;
	}
}