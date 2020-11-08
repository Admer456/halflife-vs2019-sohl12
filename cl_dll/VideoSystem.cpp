extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "libavformat/avformat.h"
	#include "libavutil/imgutils.h"
	#include "libswscale/swscale.h"
}

#include "VideoSystem.hpp"

using namespace VideoSystem;

VideoParser::VideoParser()
{
	// Let's use a hardcoded path for now keklmao
	avformat_open_input( &formatContext, "KEA1989/media/myFile.mp4", nullptr, nullptr );
}
