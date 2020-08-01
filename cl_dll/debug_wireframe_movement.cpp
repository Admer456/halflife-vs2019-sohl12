#include "wrect.h"
#include "cl_dll.h"
#include "cl_util.h"
#include "triangleapi.h"

#include "r_studioint.h"
#include "com_model.h"

#include "debug_wireframe.h"
#include "debug_wireframe_utils.h"

#include "pm_defs.h"

#include <vector>

/*
===========================================
	Debug wireframe - movement

	In essence, this draws the player's
	velocity and whenever the player
	touches something.

	TODO: Make the thing a bit nicer...
	You know, a namespace maybe, or a class,
	or whatever. Having global functions
	like this definitely looks like C...
===========================================
*/

extern engine_studio_api_t IEngineStudio;

cl_entity_t* player = nullptr;

playermove_t* playerData = nullptr;

Vector playerVelocity = Vector( 0, 0, 0 );

physent_t* ladders[32];

// Referenced in pm_shared.cpp
void DebugOverlay_UpdatePlayerMovementData( playermove_t* data )
{
	if ( !data )
		return;

	playerVelocity = data->velocity;

	playerData = data;
}

void RenderTouches( triangleapi_t* r )
{
	if ( !playerData )
		return;

	r->Begin( TRI_LINES );
	r->Color4ub( 64, 255, 64, 255 );

	for ( int i = 0; i < playerData->numtouch; i++ )
	{
		pmtrace_t* trace = playerData->touchindex;
		RenderPoint( trace->endpos );
	}

	r->End();
}

void RenderVelocity( triangleapi_t* r )
{
	Vector playerPosition = player->curstate.origin;
	Vector playerForward;
	AngleVectors( player->curstate.angles, playerForward, nullptr, nullptr );
	Vector playerWorldVelocity = playerPosition + playerVelocity;

	r->Begin( TRI_LINES );
	
	r->Color4ub( 255, 0, 0, 255 );
	RenderLine( playerPosition, playerWorldVelocity );
	RenderPoint( playerWorldVelocity );
	
	r->Color4ub( 120, 100, 0, 255 );
	RenderLine( playerPosition, playerPosition + playerForward * 64 );
	RenderPoint( playerPosition + playerForward * 64 );
	
	r->End();
}

void DebugOverlayInit()
{
	player = gEngfuncs.GetLocalPlayer();
}

void RenderDebugWireframe_Movement()
{
	triangleapi_t* r = gEngfuncs.pTriAPI;

	if ( !player )
		player = gEngfuncs.GetLocalPlayer();

	model_t* texture = IEngineStudio.Mod_ForName( "sprites/camnoise.spr", 0 );

	if ( texture )
		r->SpriteTexture( texture, 0 );

	RenderVelocity( r );
	RenderTouches( r );
}


