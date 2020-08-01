#include "wrect.h"
#include "cl_dll.h"
#include "triangleapi.h"

#include "r_studioint.h"
#include "com_model.h"

#include "debug_wireframe.h"
#include "debug_wireframe_utils.h"

extern engine_studio_api_t IEngineStudio;

cl_entity_t* localPlayer;

void RenderDebugWireframe_BoundingBox()
{
	triangleapi_t* r = gEngfuncs.pTriAPI;

	static model_t* texture = nullptr;

	if ( g_DebugWireframeInitialisation )
	{
		// The final Death of John Deepmind Pt2 Episode 1: The Fall of New York has this sprite, dunno about others
		texture = const_cast<model_t*>( gEngfuncs.GetSpritePointer( gEngfuncs.pfnSPR_Load( "sprites/camnoise.spr" ) ) ); 
		localPlayer = gEngfuncs.GetLocalPlayer();
	}

	// The user may not necessarily have this sprite, so don't crash if we don't find one
	if ( texture )
		r->SpriteTexture( texture, 0 );

	// For potentially every client entity
	for ( int i = 1; i < 2048; i++ )
	{
		cl_entity_t* ent = gEngfuncs.GetEntityByIndex( i );

		if ( !ent )
			continue;

		// If the entity has been destroyed or is not visible by the player, skip
		if ( ent->curstate.messagenum != localPlayer->curstate.messagenum )
			continue;

		// Gather the bounding boxes
		Vector mins = ent->curstate.mins;
		Vector maxs = ent->curstate.maxs;
		
		mins = mins + ent->curstate.origin;
		maxs = maxs + ent->curstate.origin;

		// Begin rendering
		r->Color4ub( 0, 255, 255, 255 );
		r->RenderMode( kRenderTransAdd );
		r->CullFace( TRI_NONE );
		r->Begin( TRI_LINES );
	
		if ( ent->model )
		{
			switch ( ent->model->type )
			{
			case mod_brush: // Render box corners if it's a brush
			{
				r->Color4ub( 0, 128, 200, 255 );
				RenderBoxCorners( mins, maxs );

				// Render a point at the "perceived" origin
				// Since brush models typically have their origin at 0,0,0
				// unless they have an ORIGIN brush, it would be aesthetically
				// pleasing to see an origin where we expect it
				r->Color4ub( 0, 140, 192, 255 );
				RenderPoint( (mins + maxs) / 2.0f ); 
			}
			break;
			
			case mod_studio: // Render a simple box if it's a studio model
			{
				RenderBox( mins, maxs );
			}
			break;

			case mod_sprite: // Render a 12-sided sphere if it's a sprite
			{
				r->Color4ub( 0, 100, 255, 255 );
				RenderSphere( ent->curstate.origin, 16.0f, 12 );
			}
			break;
			}
		}

		RenderPoint( ent->curstate.origin ); // In every case, render a point at the real origin

		// Stop rendering
		r->End();
	}
}
