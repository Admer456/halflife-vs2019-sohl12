#pragma once

extern void RenderLine( float* a, float* b );
extern void RenderLine( Vector& a, Vector& b );
extern void RenderPoint( Vector& a, float radius = 8.0f );
extern void RenderBox( Vector min, Vector max );
extern void RenderBoxCorners( Vector min, Vector max );
extern void RenderSphere( Vector pos, float radius, int sides );