#pragma once


enum DebugWireframeModes
{
	DebugWireframe_Clipnodes,
	DebugWireframe_Movement,
	DebugWireframe_BoundingBox,

	DebugWireframe_Max,
	DebugWireframe_Unknown
};

DebugWireframeModes GetModeFromString( const char* cvarString );

bool GetStateFromString( const char* cvarString );

void RenderDebugWireframe_Clipnodes();
void RenderDebugWireframe_Movement();
void RenderDebugWireframe_BoundingBox();

void ThinkDebugWireframe();

bool DebugWireState( int state );

void RenderDebugWireframe();

extern bool g_DebugWireframeInitialisation;
