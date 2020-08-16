#pragma once

#include "interface.h"

#define GAMEUI_INTERFACE "GameUI007"

#ifdef _WIN32
#define GAMEUI_DLLNAME "cl_dlls/gameui.dll"
#elif defined( OSX )
#define GAMEUI_DLLNAME "cl_dlls/gameui.dylib"
#elif defined( LINUX )
#define GAMEUI_DLLNAME "cl_dlls/gameui.so"
#else
#error "GAMEUI_DLLNAME must be set for this platform!"
#endif

class IGameUI : public IBaseInterface
{
public:
	virtual void Unknown1() = 0;
	virtual void Unknown2() = 0;
	virtual void Unknown3() = 0;
	virtual void Unknown4() = 0;
	virtual void Unknown5() = 0;
	virtual int IsGameUIVisible() = 0;
};

extern IGameUI *g_pGameUI;
