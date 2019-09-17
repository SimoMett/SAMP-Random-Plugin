#pragma once

#include <cstdio>
#include <fstream>
#include <Windows.h>

#include "plugin.h"
#include "Logger/Logger.h"
#include "raknet/RakClientInterface.h"
#include "detours/detours.h"
#include "NotMineStuff/hook_utils.h"

#include "game_sa/CProjectileInfo.h"
#include "game_sa/CCamera.h"

class RandomSampPlugin
{
public:
	RandomSampPlugin();

	~RandomSampPlugin();

private:

	static LPVOID WINAPI HookSamp(LPVOID* lpParam);
	void CloseHookSamp();

	HANDLE hSampHookingThread;

} _sampcustomhandlingwithSDK;