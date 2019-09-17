#include "RandomSampPlugin.h"

using namespace plugin;
using Logger::Debug;

void CreateConsole()
{
	AllocConsole();

	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

RandomSampPlugin::RandomSampPlugin()
{
	CreateConsole();

	Debug("Starting plugin");

	hSampHookingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RandomSampPlugin::HookSamp, (LPVOID)this, NULL, (LPDWORD)NULL);

	CProjectileInfo::Initialise();
}

RandomSampPlugin::~RandomSampPlugin()
{
	Debug("Stopping plugin..");

	CProjectileInfo::Shutdown();

	Logger::Close();
}

typedef Packet* (__thiscall* tReceive)(RakClientInterface* thisptr);
tReceive original_RakReceive = nullptr;

void TestFunction1(const Packet& pkt)// Sets Jester transmission
{
	char* jesterTransmission = (char*)0xc320b0;
	*jesterTransmission = pkt.data[1];
}

void TestFunction2(const Packet& pkt)// shoot missile
{
	CPed* player = FindPlayerPed();

	CVector dir(0,0,1);
	CVector start = player->GetPosition();

	//CProjectileInfo::AddProjectile((CEntity*)player, eWeaponType::WEAPON_ROCKET, start, 1.0, &dir, nullptr);
	//Debug("CVector( %f , %f , %f )", dir.x, dir.y, dir.z);

	//CWeapon::ProcessLineOfSight()
	float x, y, z ;

	CCamera* camera = (CCamera*)0xB6F028;
	camera->GetOrientation(x, y, z);
	CVector pCamera, pPoint;
	camera->Find3rdPersonCamTargetVector(10, camera->GetPosition(), &pCamera, &pPoint);
	
	Debug("CVector( %f , %f , %f )", pCamera.x, pCamera.y, pCamera.z);
}

Packet* __fastcall overridden_RakReceive(RakClientInterface* thisptr)
{
	Packet* pkt = original_RakReceive(thisptr);
	if (pkt)
	{
		if (pkt->data[0] == 252)//DEBUGONLY
		{
			Debug("Call to overridden RakClient->Receive() (Size: %d bytes)", pkt->bitSize/8);
			TestFunction2(*pkt);
		}
	}
	return pkt;
}

LPVOID WINAPI RandomSampPlugin::HookSamp(LPVOID* lpParam)//TODO
{
	Debug("Started Hooking Thread");
	unsigned int startTime = GetTickCount();
	unsigned int stopTime = startTime + 10 * 1000; //10 seconds
	DWORD dwSampModule = NULL;
	while (dwSampModule == NULL && GetTickCount() < stopTime)
	{
		dwSampModule = (DWORD)GetModuleHandleA("samp.dll");
		Sleep(100);
	}
	if (dwSampModule)
		Debug("SAMP module found at 0x%x", dwSampModule);
	else
		Error("SAMP module not found!");

	if (dwSampModule)
	{
		//These lines are gently stolen from https://github.com/dotSILENT/chandling/blob/master/chandling/Hooks.cpp
		DWORD dwAddr = FindPattern(dwSampModule, "\x6A\x04\x8B\xCE\xC7\x44\x24\x00\x00\x00\x00\x00", "xxxxxxx?????") - 60; // RakClient->Receive()
		Debug("RakClient()->Receive() address = 0x%x", dwAddr);
		original_RakReceive = (tReceive)DetourFunction((PBYTE)dwAddr, (PBYTE)overridden_RakReceive);
	}

	((RandomSampPlugin*)lpParam)->CloseHookSamp();
	return NULL;
}

void RandomSampPlugin::CloseHookSamp()
{
	if (hSampHookingThread)
	{
		TerminateThread(hSampHookingThread, 0);
		hSampHookingThread = NULL;
	}
}