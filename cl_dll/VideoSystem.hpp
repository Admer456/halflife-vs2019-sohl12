/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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

/*
============================================
	Video playback system, using ffmpeg
============================================
*/

#pragma once

namespace VideoSystem
{
	// Parses a video and stores stuff into a VideoObject
	class VideoParser final
	{
	public:
		VideoParser();

		AVFormatContext* formatContext{ nullptr };
	};

	// Holds video frames n stuf
	class VideoObject final
	{

	};

	// Does all the rendering stuff
	class VideoPlayer
	{

	};
}
