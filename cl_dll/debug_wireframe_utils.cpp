#include "wrect.h"
#include "cl_dll.h"
#include "cl_util.h"
#include "triangleapi.h"

#include "r_studioint.h"
#include "com_model.h"

#include "debug_wireframe.h"
#include "debug_wireframe_utils.h"

void RenderLine( float* a, float* b )
{
	triangleapi_t* r = gEngfuncs.pTriAPI;

	r->TexCoord2f( 0, 0 );
	r->Vertex3fv( a );

	r->TexCoord2f( 1, 1 );
	r->Vertex3fv( b );
}

void RenderLine( Vector& a, Vector& b )
{
	triangleapi_t* r = gEngfuncs.pTriAPI;

	r->TexCoord2f( 0, 0 );
	r->Vertex3fv( a );

	r->TexCoord2f( 1, 1 );
	r->Vertex3fv( b );
}

void RenderPoint( Vector& a, float radius )
{
	// Draw 3 lines that intersect at the point
	RenderLine( a + Vector( -radius, 0.f, 0.f ), a + Vector( radius, 0.f, 0.f ) );
	RenderLine( a + Vector( 0.f, -radius, 0.f ), a + Vector( 0.f, radius, 0.f ) );
	RenderLine( a + Vector( 0.f, 0.f, -radius ), a + Vector( 0.f, 0.f, radius ) );
}

void RenderBox( Vector min, Vector max )
{
	triangleapi_t* r = gEngfuncs.pTriAPI;

	// Vector positions of a box
	// X = front-back
	// Y = left-right
	// Z = top-bottom
	float bottomLeftBack[3] = { min.x, min.y, min.z };
	float bottomLeftFront[3] = { max.x, min.y, min.z };
	float bottomRightFront[3] = { max.x, max.y, min.z };
	float bottomRightBack[3] = { min.x, max.y, min.z };

	float topLeftBack[3] = { min.x, min.y, max.z };
	float topLeftFront[3] = { max.x, min.y, max.z };
	float topRightFront[3] = { max.x, max.y, max.z };
	float topRightBack[3] = { min.x, max.y, max.z };

	// Draw bottom rectangle
	RenderLine( bottomLeftBack, bottomLeftFront );
	RenderLine( bottomLeftFront, bottomRightFront );
	RenderLine( bottomRightFront, bottomRightBack );
	RenderLine( bottomRightBack, bottomLeftBack );

	// Draw top rectangle
	RenderLine( topLeftBack, topLeftFront );
	RenderLine( topLeftFront, topRightFront );
	RenderLine( topRightFront, topRightBack );
	RenderLine( topRightBack, topLeftBack );

	// Draw vertical lines between the two rectangles
	RenderLine( bottomLeftBack, topLeftBack );
	RenderLine( bottomLeftFront, topLeftFront );
	RenderLine( bottomRightBack, topRightBack );
	RenderLine( bottomRightFront, topRightFront );
}

void RenderBoxCorners( Vector min, Vector max )
{
	triangleapi_t* r = gEngfuncs.pTriAPI;

	// Vector positions of a box
	// X = front-back
	// Y = left-right
	// Z = top-bottom
	Vector bottomLeftBack = Vector( min.x, min.y, min.z );
	Vector bottomLeftFront = Vector( max.x, min.y, min.z );
	Vector bottomRightFront = Vector( max.x, max.y, min.z );
	Vector bottomRightBack = Vector( min.x, max.y, min.z );

	Vector topLeftBack = Vector( min.x, min.y, max.z );
	Vector topLeftFront = Vector( max.x, min.y, max.z );
	Vector topRightFront = Vector( max.x, max.y, max.z );
	Vector topRightBack = Vector( min.x, max.y, max.z );

	// For each corner, 3 lines are required

	// bottomLeftBack
	{
		RenderLine( bottomLeftBack, bottomLeftBack + Vector( 8, 0, 0 ) );
		RenderLine( bottomLeftBack, bottomLeftBack + Vector( 0, 8, 0 ) );
		RenderLine( bottomLeftBack, bottomLeftBack + Vector( 0, 0, 8 ) );
	}

	// bottomLeftFront
	{
		RenderLine( bottomLeftFront, bottomLeftFront + Vector( -8, 0, 0 ) );
		RenderLine( bottomLeftFront, bottomLeftFront + Vector( 0, 8, 0 ) );
		RenderLine( bottomLeftFront, bottomLeftFront + Vector( 0, 0, 8 ) );
	}

	// bottomRightFront
	{
		RenderLine( bottomRightFront, bottomRightFront + Vector( -8, 0, 0 ) );
		RenderLine( bottomRightFront, bottomRightFront + Vector( 0, -8, 0 ) );
		RenderLine( bottomRightFront, bottomRightFront + Vector( 0, 0, 8 ) );
	}

	// bottomRightBack
	{
		RenderLine( bottomRightBack, bottomRightBack + Vector( 8, 0, 0 ) );
		RenderLine( bottomRightBack, bottomRightBack + Vector( 0, -8, 0 ) );
		RenderLine( bottomRightBack, bottomRightBack + Vector( 0, 0, 8 ) );
	}

	// topLeftBack
	{
		RenderLine( topLeftBack, topLeftBack + Vector( 8, 0, 0 ) );
		RenderLine( topLeftBack, topLeftBack + Vector( 0, 8, 0 ) );
		RenderLine( topLeftBack, topLeftBack + Vector( 0, 0, -8 ) );
	}

	// topLeftFront
	{
		RenderLine( topLeftFront, topLeftFront + Vector( -8, 0, 0 ) );
		RenderLine( topLeftFront, topLeftFront + Vector( 0, 8, 0 ) );
		RenderLine( topLeftFront, topLeftFront + Vector( 0, 0, -8 ) );
	}

	// topRightFront
	{
		RenderLine( topRightFront, topRightFront + Vector( -8, 0, 0 ) );
		RenderLine( topRightFront, topRightFront + Vector( 0, -8, 0 ) );
		RenderLine( topRightFront, topRightFront + Vector( 0, 0, -8 ) );
	}

	// topRightBack
	{
		RenderLine( topRightBack, topRightBack + Vector( 8, 0, 0 ) );
		RenderLine( topRightBack, topRightBack + Vector( 0, -8, 0 ) );
		RenderLine( topRightBack, topRightBack + Vector( 0, 0, -8 ) );
	}

}

Vector PolarCoords( float angle, float radius = 1.0f )
{
	return Vector( cos( angle ) * radius, sin( angle ) * radius, 0.f );
}

Vector SphericalCoords( float pitch, float yaw, float radius = 1.0f )
{
	return Vector( cos( yaw ) * sin( pitch ) * radius,
		sin( yaw ) * sin( pitch ) * radius,
		cos( pitch ) * radius );
}

void RenderHorizontalCircle( Vector pos, float radius, int sides )
{
	const float doublePi = 3.14159265359 * 2.0f;
	const float anglePiece = doublePi / (float)sides;

	for ( int i = 0; i < sides; i++ )
	{
		float angle = anglePiece * i;
		float nextAngle = anglePiece * (i+1);

		Vector firstPoint = PolarCoords( angle, radius );
		Vector secondPoint = PolarCoords( nextAngle, radius );

		RenderLine( pos + firstPoint, pos + secondPoint );
	}
}

void RenderVerticalCircle( Vector pos, float radius, int sides, float yaw )
{
	const float doublePi = 3.14159265359 * 2.0f;
	const float anglePiece = doublePi / (float)sides;

	// Must multiply these by 2, otherwise we get the wrong number of sides
	sides *= 2;
	yaw *= 2.0f;

	for ( int i = 0; i < sides; i++ )
	{
		float angle = anglePiece * i;
		float nextAngle = anglePiece * (i+1);

		Vector firstPoint = SphericalCoords( angle, yaw, radius );
		Vector secondPoint = SphericalCoords( nextAngle, yaw, radius );

		RenderLine( pos + firstPoint, pos + secondPoint );
	}
}

void RenderSphere( Vector pos, float radius, int sides )
{
	const float pi = 3.14159265359;
	const float anglePiece = pi / (float)sides;

	// First, render horizontal circles
	for ( int i = 1; i < sides; i++ )
	{
		float angle = anglePiece * (float)i;

		Vector verticalOffset = Vector( 0.f, 0.f, -cos(angle) ) * radius;

		RenderHorizontalCircle( pos + verticalOffset, radius * sin(angle), sides );

	}

	// Then the vertical ones
	for ( int i = 0; i < sides; i++ )
	{
		float angle = anglePiece * (float)i;

		RenderVerticalCircle( pos, radius, sides, angle );
	}
}